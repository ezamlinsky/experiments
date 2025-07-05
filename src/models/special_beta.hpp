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
# define	BETA_N		26

//****************************************************************************//
//      The helper function to calculate the trimmed continued fraction       //
//****************************************************************************//
// Arguments are always correct because of preceding checks in the caller class
static void continued_fraction (
	double &p,		// Where to return the P value of continued fraction
	double &q,		// Where to return the Q value of continued fraction
	double x,		// Argument value
	double a,		// A argument of the incomplete beta function
	double b		// B argument of the incomplete beta function
){
	// Internal variable are immutable in the loop
	const double ab = a + b;
	const double ap = a + 1.0;
	const double am = a - 1.0;

	// Intermediate P and Q for the loop
	double p1 = 1.0;
	double q1 = 1.0;

	// Calculate the trimmed continued fraction
	int j = BETA_N;
	while (j) {

		// We do it from the tail
		const int j2 = 2 * j;

		// Odd term
		const double odd1 = -(a + j) * (ab + j) * x;
		const double odd2 = (a + j2) * (ap + j2);
		const double odd = odd1 / odd2;
		const double p2 = p1 + odd * q1;
		const double q2 = p1;

		// Even term
		const double even1 = j * (b - j) * x;
		const double even2 = (am + j2) * (a + j2);
		const double even = even1 / even2;
		p1 = p2 + even * q2;
		q1 = p2;

		// Decrement the counter
		--j;
	}

	// Finalize the calculation of the continued fraction
	const double odd = ab * x / ap;
	p = p1 - odd * q1;
	q = p1;
}

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
	double core (
		double x,	// Argument value
		double a,	// A argument of the incomplete beta function
		double b	// B argument of the incomplete beta function
	) const {

		// Evaluate the continued fraction part
		double p, q;
		continued_fraction (p, q, x, a, b);

		// Combine the parts to get the result
		const double temp = a * log (x) + b * log (1.0 - x);
		return exp (temp - beta_log) * q / (p * a);
	}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	SpecialBeta (
		double a,	// A argument of the beta function
		double b	// B argument of the beta function
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
//      Regularized incomplete beta function                                  //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double RegIncompleteBeta (
		double x	// Argument value
	) const {

		// Check the argument range
		if (0.0 <= x && x <= 1.0) {

			// Special cases
			if (x == 0.0) return 0.0;
			if (x == 1.0) return 1.0;

			// The threshold value
			const double threshold = (beta_a + 1.0) / (beta_a + beta_b + 2.0);

			// Return the result or its complement based on the initial check
			if (x > threshold)
				return 1.0 - core (1.0 - x, beta_b, beta_a);
			else
				return core (x, beta_a, beta_b);
		}
		else
			throw invalid_argument ("RegIncompleteBeta: The argument must be in the range [0..1]");
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Logarithm of the beta function                                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double BetaLog (void) const {
		return beta_log;
	}
};
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
