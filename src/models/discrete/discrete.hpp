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

// Finite range values for infinite domains of discrete functions
# define	FINITE_MIN	-1e10
# define	FINITE_MAX	+1e10

//****************************************************************************//
//      Class "BaseDiscrete"                                                  //
//****************************************************************************//
class BaseDiscrete : public BaseModel
{
//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Estimation of the distribution quartile                               //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Quartile (
		double level			// Quantile level to estimate
	) const override final {

		// Adjust the raw value of the target quantile when required
		const double value = Quantile (level);
		const double cdf = CDF (value);
		return fabs (cdf - level) <= EPSILON ? value + 0.5 : value;
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
		double level			// Quantile level to estimate
	) const override final {

		// Check if the level is correct
		if (0.0 <= level and level <= 1.0) {

			// Get the distribution domain to clamp the argument value
			const Range &range = Domain();

			// Binary search of argument value for the CDF function
			int64_t left = max (range.Min(), FINITE_MIN);
			int64_t right = min (range.Max(), FINITE_MAX);
			while (left < right) {
				const int64_t x = (left + right) / 2;
				if (CDF (x) < level)
					left = x + 1;
				else
					right = x;
			}
			return left;
		}
		else
			throw invalid_argument ("Quantile: Level must be in the range [0..1]");
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
