/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#              CALCULATE THE REGULARIZED INCOMPLETE BETA FUNCTION              #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	<cmath>
# include	<stdexcept>

// Number of iterations to calculate the continued fraction
# define	BETA_N		52

//****************************************************************************//
//      Name space "Model"                                                    //
//****************************************************************************//
namespace Model
{
//****************************************************************************//
//      Special beta function                                                 //
//****************************************************************************//
class SpecialBeta
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	const double beta_a;	// A argument of the incomplete beta function
	const double beta_b;	// B argument of the incomplete beta function
	const double beta_log;	// Logarithm of the beta function

//============================================================================//
//      Private methods                                                       //
//============================================================================//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      The helper function to calculate the trimmed continued fraction       //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// Arguments are always correct because of preceding checks in the caller class
	static double continued_fraction (
		double x,			// Argument value
		double a,			// A argument of the incomplete beta function
		double b			// B argument of the incomplete beta function
	){
		double res = 0.0;
		for (size_t i = BETA_N; i > 0; i--) {
			const double temp1 = a + 2.0 * i;
			const double temp2 = temp1 - 1.0;
			const double temp3 = temp1 + 1.0;
			const double temp4 = a + i;
			const double temp5 = b - i;
			const double temp6 = x / temp2;

			// Intermediate P and Q for the loop
			const double p = (temp4 - 1.0) * (temp4 + b - 1.0) * temp5 * i * temp6 * temp6;
			const double q = temp1 + (i * temp5 / temp2 - temp4 * (temp4 + b) / temp3) * x;

			// Update the intermediate result
			res = p / (q + res);
		}

		// Finalize the calculation of the continued fraction
		const double p = 1.0;
		const double q = a * (1.0 - (a + b) / (a + 1.0) * x) + res;
		return p / q;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Incomplete beta function                                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	static double incomplete (
		double x,			// Argument value
		double a,			// A argument of the incomplete beta function
		double b,			// B argument of the incomplete beta function
		double denominator	// The denominator value for the incomplete beta function
	){
		const double temp = a * log (x) + b * log (1.0 - x);
		const double scale = exp (temp - denominator);
		if (x <= a / (a + b))
			return continued_fraction (x, a, b) * scale;
		else
			return 1.0 - continued_fraction (1.0 - x, b, a) * scale;
	}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	SpecialBeta (
		double a,			// A argument of the beta function
		double b			// B argument of the beta function
	) : beta_a (a),
		beta_b (b),
		beta_log (lgamma (a) + lgamma (b) - lgamma (a + b))
	{
		if (a <= 0.0)
			throw invalid_argument ("SpecialBeta: The alpha value must be positive");
		if (b <= 0.0)
			throw invalid_argument ("SpecialBeta: The beta value must be positive");
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Incomplete beta function                                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double IncompleteBeta (
		double x			// Argument value
	) const {

		// Check the argument range
		if (0.0 <= x && x <= 1.0) {

			// Special cases
			if (x == 0.0) return 0.0;
			if (x == 1.0) return 1.0;

			// Common case
			return incomplete (x, beta_a, beta_b, 0.0);
		}
		else throw invalid_argument ("IncompleteBeta: The argument must be in the range [0..1]");
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Regularized incomplete beta function                                  //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double RegIncompleteBeta (
		double x			// Argument value
	) const {

		// Check the argument range
		if (0.0 <= x && x <= 1.0) {

			// Special cases
			if (x == 0.0) return 0.0;
			if (x == 1.0) return 1.0;

			// Common case
			return incomplete (x, beta_a, beta_b, beta_log);
		}
		else throw invalid_argument ("RegIncompleteBeta: The argument must be in the range [0..1]");
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Beta function                                                         //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double Beta (void) const {
		return exp (beta_log);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Logarithm of the beta function                                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double BetaLog (void) const {
		return beta_log;
	}
};
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
