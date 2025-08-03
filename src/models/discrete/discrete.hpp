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
	vector <double> cdf;	// Cached values of the CDF function for quick calculations

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
		const size_t index = Quantile (level);
		return fabs (cdf [index] - level) <= EPSILON ? index + 0.5 : index;
	}

//============================================================================//
//      Protected methods                                                     //
//============================================================================//
protected:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Init the cache of the CDF values used for quantile estimates          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void Init (
		size_t range		// The argument range we are looking for the CDF values
	){
		// If the range is not set manually, then estimate it automatically
		if (range == 0) {

			// The distribution mode is the first index where the PDF starts decreasing
			const double mode = Mode();
			range = isnan (mode) ? 0 : mode;

			// We are looking for the last PDF value that is indistinguishable
			// from zero if subtracted from 1.0
			while (1.0 - PDF (range) < 1.0) range++;
		}

		// Calculate the CDF values for the target range for quick further estimates
		for (size_t i = 0; i < range; i++)
			cdf.push_back (CDF (i));
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
//      Distribution type                                                     //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual DistType Type (void) const override final {
		return DISCRETE;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Quantile value for the target level                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Quantile (
		double level		// Quantile level to estimate
	) const override final {

		// Check if the level is correct
		if (0.0 <= level and level <= 1.0) {

			// Binary search of argument value for the CDF function
			int64_t left = 0;
			int64_t right = cdf.size();
			while (left < right) {
				const size_t median = (left + right) / 2;
				if (cdf [median] < level)
					left = median + 1;
				else
					right = median;
			}
			return left;
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
