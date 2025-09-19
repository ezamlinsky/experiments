/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                BASE CLASS FOR CONTINUOUS DISTRIBUTION MODELS                 #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"../base.hpp"

// Value of the step out of a point where the derivative is infinite
# define	STEP_OUTSIDE		0.001

// The number of iterations for the Newton solve method
# define	NEWTON_ITERATIONS	52

// Quantiles to locate theoretical models
# define	EPSILON				1e-6
# define	MIN_LEVEL			(0.0 + EPSILON)
# define	MAX_LEVEL			(1.0 - EPSILON)

//****************************************************************************//
//      Name space "Model"                                                    //
//****************************************************************************//
namespace Model
{
//****************************************************************************//
//      Class "BaseContinuous"                                                //
//****************************************************************************//
class BaseContinuous : public BaseModel
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
		return Quantile (level);
	}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	BaseContinuous (void) = default;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Location where the PDF function is distinguishable from zero          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual Range DistLocation (void) const override final {
		return Range (Quantile (MIN_LEVEL), Quantile (MAX_LEVEL));
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

			// The first approximation is the mode value (if it exists),
			// then the mean value
			double x = Mode();
			if (isnan (x)) x = Mean();
			if (isinf (x)) x = 0.0;

			// Get the difference between the function and the target value
			double diff = CDF (x) - level;

			// Find a solution using the Newton solve method
			int i = NEWTON_ITERATIONS;
			while (diff && i--) {

				// Calculate the function derivative
				const double der = PDF (x);

				// Calculate a step value to move for the next point
				double step = diff / der;

				// If the derivative is infinite in the target point
				// then do a step outside the pit
				if (der == INFINITY)
					step = +STEP_OUTSIDE * diff;
				else if (der == -INFINITY)
					step = -STEP_OUTSIDE * diff;

				// A new approximation
				x -= step;

				// Clamp the value inside the distribution domain
				x = range.Clamp (x);

				// Check the difference for the new function value
				diff = CDF (x) - level;
			}

			// Return the quantile value
			return x;
		}
		else
			throw invalid_argument ("Quantile: Level must be in the range [0..1]");
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Trivial information about the object                                  //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	PropGroup Info (void) const {

		// Trivial info
		PropGroup info ("Continuous distribution");
		info.Append ("Parameters count", Parameters());

		// Return the info
		return info;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Convert an instance of a derived class to the base class              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual const BaseContinuous& data (void) const override final {
		return *this;
	}
};
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
