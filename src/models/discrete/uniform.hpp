/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#             DISTRIBUTION MODEL FOR DISCRETE UNIFORM DISTRIBUTION             #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"discrete.hpp"

//****************************************************************************//
//      Class "DiscreteUniform"                                               //
//****************************************************************************//
class DiscreteUniform final : public BaseDiscrete
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	static const size_t params;		// Count of distribution parameters
	const Range range;				// Function domain where the distribution exists

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	DiscreteUniform (
		int64_t min,				// Min value of the range
		int64_t max					// Max value of the range
	) :	range (min, max)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor for empirical data                                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	DiscreteUniform (
		const Observations &data	// Empirical observations
	) : range (data.Domain())
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Function domain where the distribution exists                         //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual const Range& Domain (void) const override final {
		return range;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Number of distribution parameters to describe the population          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual size_t Parameters (void) const override final {
		return params;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Probability Density Function (PDF)                                    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double PDF (
		double x					// Argument value
	) const override final {

		// Outside the distribution domain
		if (x != range) return 0.0;

		// Common case
		return 1.0 / (range.Length() + 1);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Cumulative Distribution Function (CDF)                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double CDF (
		double x					// Argument value
	) const override final {

		// Below the range
		if (x < range) return 0.0;

		// Above the range
		if (x >= range) return 1.0;

		// Common case
		const size_t arg = floor (x);
		return (arg - range.Min() + 1) / (range.Length() + 1);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Mode of the distribution                                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Mode (void) const override final {
		return NAN;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Mean of the distribution                                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Mean (void) const override final {
		return range.MidRange();
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Variance of the distribution                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Variance (void) const override final {
		const double temp = (range.Length() + 1);
		return (temp * temp - 1.0) / 12.0;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Skewness of the distribution                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Skewness (void) const override final {
		return 0.0;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Kurtosis of the distribution                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Kurtosis (void) const override final {
		const double length = range.Length() + 1;
		const double temp = length * length;
		const double p = 6.0 * (1.0 + temp);
		const double q = 5.0 * (1.0 - temp);
		return p / q;
	}
};

//****************************************************************************//
//      Internal constants used by the class                                  //
//****************************************************************************//
const size_t DiscreteUniform::params = 2;

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const DiscreteUniform &model)
{
	auto restore = stream.precision();
	stream.precision (PRECISION);
	stream << "\nUNIFORM DISTRIBUTION:" << std::endl;
	stream << "=====================" << std::endl;
	stream << static_cast <const BaseDiscrete&> (model);
	stream << static_cast <const BaseModel&> (model);
	stream.precision (restore);
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
