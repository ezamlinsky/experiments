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
# define	NEWTON_ITERATIONS	8

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
//      Distribution type                                                     //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual DistType Type (void) const override final {
		return CONTINUOUS;
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

			// Find a solution using the Newton solve method
			int i = NEWTON_ITERATIONS;
			while (i--) {

				// Calculate the function and its derivative
				const double func = CDF (x);
				const double der = PDF (x);

				// Check the distance between the function and the target value
				const double diff = func - level;

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
			}
			return x;
		}
		else
			throw invalid_argument ("Quantile: Level must be in the range [0..1]");
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Convert an instance of a derived class to the base class              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual const BaseContinuous& data (void) const override final {
		return *this;
	}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const BaseContinuous &model)
{
	stream << "\nContinuous distribution:" << endl;
	stream << "~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	stream << "    Parameters count\t\t\t= " << model.Parameters() << endl;
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
