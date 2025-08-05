/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                 BASE CLASS FOR DISCRETE DISTRIBUTION MODELS                  #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"../base.hpp"

// Epsilon value for quartile estimation
# define	EPSILON		1e-10

//****************************************************************************//
//      Class "BaseDiscrete"                                                  //
//****************************************************************************//
class BaseDiscrete : public BaseModel
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	vector <double> cmf;	// Cached values of the CMF function for quick calculations
	size_t location;		// Location of the distribution

//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Estimation of the distribution quartile                               //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Quartile (
		double level		// Quantile level to estimate
	) const override final {

		// Adjust the raw value of the target quantile when required
		const size_t value = Quantile (level);
		return fabs (CMF (value) - level) <= EPSILON ? value + 0.5 : value;
	}

//============================================================================//
//      Protected methods                                                     //
//============================================================================//
protected:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Init the cache of the CMF values used for quantile estimates          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void Init (
		size_t range		// The argument range we are looking for the CMF values
	){
		// Get the distribution location
		location = Domain().Min();

		// If the range is not set manually, then estimate it automatically
		if (!range) {

			// The distribution mode is the first index where the PMF starts decreasing
			const double mode = Mode();
			range = isnan (mode) ? location : mode;

			// We are looking for the last PDF value that is indistinguishable
			// from zero if subtracted from 1.0
			while (1.0 - PDF (range) < 1.0) range++;
			range -= location;
		}

		// Calculate the CMF values for the target range for quick further estimates
		double sum = 0.0;
		for (size_t i = 0; i < range; i++) {
			sum += PDF (location + i);
			cmf.push_back (sum);
		}
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Return the cache of the Cumulative Mass Function (CMF)                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double CMF (
		size_t x			// Argument value
	) const try {
		return cmf.at (x - location);
	} catch (const out_of_range &ex) {
		return NAN;
	}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	BaseDiscrete (void) = default;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Quantile value for the target level                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Quantile (
		double level		// Quantile level to estimate
	) const override final {

		// Check if the level is correct
		if (0.0 <= level and level <= 1.0) {

			// Binary search of argument value for the CMF function
			int64_t left = 0;
			int64_t right = cmf.size();
			while (left < right) {
				const size_t median = (left + right) / 2;
				if (cmf.at (median) < level)
					left = median + 1;
				else
					right = median;
			}

			// Return the quantile value
			return location + left;
		}
		else throw invalid_argument ("Quantile: Level must be in the range [0..1]");
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Convert an instance of a derived class to the base class              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual const BaseDiscrete& data (void) const override final {
		return *this;
	}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const BaseDiscrete &model)
{
	stream << "\nDiscrete distribution:" << endl;
	stream << "~~~~~~~~~~~~~~~~~~~~~~" << endl;
	stream << "    Parameters count\t\t\t= " << model.Parameters() << endl;
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
