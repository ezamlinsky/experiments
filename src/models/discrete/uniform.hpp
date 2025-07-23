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
# include	<cstdint>
# include	"discrete.hpp"

Distribution model for discrete uniform distribution

//****************************************************************************//
//      Class "DiscreteUniform"                                               //
//****************************************************************************//
class DiscreteUniform final : public BaseDiscrete
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
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
//      Probability Density Function (PDF)                                    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double PDF (
		double x					// Argument value
	) const override final {
		return range.IsInside (x) ? 1.0 / (range.Length() + 1) : 0.0;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Cumulative Distribution Function (CDF)                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double CDF (
		double x					// Argument value
	) const override final {

		// Less than min value
		if (x < range.Min())
			return 0.0;

		// Greater than or equal to max value
		if (range.Max() <= x)
			return 1.0;

		// Common case
		return (floor (x) - range.Min() + 1) / (range.Length() + 1);
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
		return (temp * temp - 1.0)  / 12.0;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Clone the distribution model                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual unique_ptr <const BaseModel> clone (void) const override final {
		return unique_ptr <const BaseModel> (new DiscreteUniform (*this));
	}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const DiscreteUniform &model)
{
	auto restore = stream.precision();
	stream.precision (PRECISION);
	stream << "\nDISCRETE UNIFORM DISTRIBUTION:" << std::endl;
	stream << "==============================" << std::endl;
	stream << static_cast <const BaseModel&> (model);
	stream.precision (restore);
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
