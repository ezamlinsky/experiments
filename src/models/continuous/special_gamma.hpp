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
# include	"../functions/gamma.hpp"
# include	"continuous.hpp"

//****************************************************************************//
//      Special class for PDF and CDF for gamma-related distributions         //
//****************************************************************************//
class SpecialGamma : public BaseContinuous
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	static const Range range;	// Function domain where the distribution exists
	const double gamma_log;		// Logarithm of the gamma function

protected:
	const size_t gamma_shape;	// Shape of the gamma distribution
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
	) : gamma_log (lgamma (0.5 * shape)),
		gamma_shape (shape),
		gamma_scale (scale)
	{
		if (shape == 0.0)
			throw invalid_argument ("SpecialGamma: The shape value must be positive");
		if (scale <= 0.0)
			throw invalid_argument ("SpecialGamma: The scale value must be positive");
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Check if the range is inside the model domain                         //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	static bool InDomain (
		const Range &subrange	// Testing range
	){
		return range >= subrange;
	}

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
		double x				// Argument value
	) const override final {

		// Below the range
		if (x < range) return 0.0;

		// Handle a case where log(0) would occur
		if (x == range.Min()) {
			if (gamma_shape == 1) return INFINITY;
			if (gamma_shape == 2) return 1.0 / gamma_scale;
			return 0.0;
		}

		// Common case
		const double arg = x / gamma_scale;
		const double temp = (0.5 * gamma_shape) * log (arg) - arg - gamma_log;
		return exp (temp) / x;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Cumulative Distribution Function (CDF)                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double CDF (
		double x				// Argument value
	) const override final {

		// Below the range
		if (x <= range) return 0.0;

		// Common case
		const double arg = x / gamma_scale;
		const size_t count = gamma_shape / 2;
		if (gamma_shape % 2)
			return NormalizedLowerIncompleteGammaHalf (arg, count);
		else
			return NormalizedLowerIncompleteGamma (arg, count);
	}
};

//****************************************************************************//
//      Internal constants used by the class                                  //
//****************************************************************************//
const Range SpecialGamma::range = Range (0.0, INFINITY);
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
