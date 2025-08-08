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
# define	QUARTILE_EPSILON	1e-10

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
	size_t min_index;		// The minimum index where the PDF value is still different from zero
	size_t max_index;		// The maximum index where the PDF value is still different from zero

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
		return fabs (CMF (value) - level) <= QUARTILE_EPSILON ? value + 0.5 : value;
	}

//============================================================================//
//      Protected methods                                                     //
//============================================================================//
protected:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Init the cache of the CMF values used for quantile estimates          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void Init (void) {

		// Initial position to look for significant CDF points to cache
		double pos = Mode();
		if (isnan (pos)) pos = Mean();
		if (isinf (pos)) pos = Domain().Min();

		// Find the minimum index where the PDF value is still different from zero
		int64_t first_index = pos;
		while (1.0 - PDF (first_index) < 1.0) --first_index;
		min_index = first_index + 1;

		// Find the maximum index where the PDF value is still different from zero
		int64_t last_index = pos;
		while (1.0 - PDF (last_index) < 1.0) ++last_index;
		max_index = last_index - 1;

		// Fill the cache with CMF values for quick further estimates
		double sum = 0.0;
		for (size_t i = min_index; i <= max_index; i++) {
			sum += PDF (i);
			cmf.push_back (sum);
		}
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Return the cached value of the Cumulative Mass Function (CMF)         //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double CMF (
		size_t x			// Argument value
	) const try {
		return cmf.at (x - min_index);
	} catch (const out_of_range &ex) {
		return NAN;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Trivial way to calculate the Cumulative Distribution Function (CDF)   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double TrivialCDF (
		size_t x			// Argument value
	) const {
		double sum = 0.0;
		for (size_t i = min_index; i <= x; i++)
			sum += PDF (i);
		return sum;
	}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	BaseDiscrete (void) : min_index (0), max_index (0) {}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Location where the PDF function is distinguishable from zero          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual Range DistLocation (void) const override final {
		return Range (min_index, max_index);
	}

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
			return min_index + left;
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
