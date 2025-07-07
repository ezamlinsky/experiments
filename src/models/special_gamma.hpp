/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                  BASE CLASS FOR GAMMA-RELATED DISTRIBUTIONS                  #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"base.hpp"

//****************************************************************************//
//      Special class for PDF and CDF for gamma-related distributions         //
//****************************************************************************//
class SpecialGamma : public BaseModel
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	const size_t gamma_shape;	// Shape of the gamma distribution
	const double gamma_log;		// Logarithm of the gamma function

protected:
	const double gamma_scale;	// Scale of the gamma distribution

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	SpecialGamma (
		size_t shape,			// Shape of the gamma distribution
		double scale			// Scale of the gamma distribution
	) : BaseModel (Range (0, INFINITY)),
		gamma_shape (shape),
		gamma_log (lgamma (0.5 * shape)),
		gamma_scale (scale)
	{
		if (shape == 0.0)
			throw invalid_argument ("SpecialGamma: The shape value must be positive");
		if (scale <= 0.0)
			throw invalid_argument ("SpecialGamma: The scale value must be positive");
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Probability Density Function (PDF)                                    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double PDF (
		double x				// Argument value
	) const override {

		// Normalize the argument
		const double arg = x / gamma_scale;

		// Negative argument
		if (arg < 0.0)
			return NAN;

		// Handle cases where log(0) would occur
		if (arg == 0.0) {
			if (gamma_shape == 1)
				return INFINITY;
			else if (gamma_shape == 2)
				return 0.5;
			else
				return 0.0;
		}

		// Common case
		else {
			const double temp = (0.5 * gamma_shape) * log (arg) - arg - gamma_log;
			return exp (temp) / x;
		}
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Cumulative Distribution Function (CDF)                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double CDF (
		double x				// Argument value
	) const override {

		// Normalize the argument
		const double arg = x / gamma_scale;

		// Negative argument
		if (arg < 0.0)
			return NAN;

		// Zero argument
		else if (arg == 0.0)
			return 0.0;

		// Common case
		else {
			double sum = 0.0;
			const double arg_log = log (arg);
			const size_t count = gamma_shape / 2;
			if (gamma_shape % 2) {

				// Shape is odd
				for (size_t i = 0; i < count; i++) {
					const double temp = (i + 0.5) * arg_log - arg - lgamma (i + 1.5);
					sum += exp (temp);
				}
				return erf (sqrt (arg)) - sum;
			}
			else {

				// Shape is even
				for (size_t i = 0; i < count; i++) {
					const double temp = i * arg_log - arg - lgamma (i + 1.0);
					sum += exp (temp);
				}
				return 1.0 - sum;
			}
		}
	}
};
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
