/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                   DIFFERENTIATOR FILTER FOR A TIME SERIES                    #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"fir_filter.hpp"

// Length of the tail of the filter oscillations
# define	DIFF_EXTRA_PERIOD	10

//****************************************************************************//
//      Class "DiffFilter"                                                    //
//****************************************************************************//
class DiffFilter : public FIR_Filter
{
//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Compute values of the differentiator filter                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	static vector <double> Filter (
		size_t points				// Count of neighbor points to smooth by
	){
		// Filter period and size
		const size_t period = points + 1;
		const size_t size = (4 * DIFF_EXTRA_PERIOD * period - points) / 2;

		// Compute the impulse response
		vector <double> impulse;
		impulse.push_back (0.0);
		for (size_t i = 1; i < size; i++) {

			// Sinc derivative function
			const double x = M_PI * i / period;
			const double p = sin (x) - x * cos (x);
			const double q = x * x;
			impulse.push_back (p / q);
		}

		// Return the impulse response
		return impulse;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Normalize the filter impulse response                                 //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	static void Normalize (
		double impulse[],			// Filter impulse response
		size_t size,				// Size of impulse response
		double scale				// Scale factor for the coefficients
	){
		// Find the sum of weighted filter coefficients
		double sum = 0;
		for (size_t i = 1; i < size; i++)
			sum += i * impulse [i];

		// Normalize all the filter coefficients with a scale factor
		Array::Div (impulse, size, 2.0 * sum * scale);
	}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	DiffFilter (
		size_t points,				// Count of neighbor points to smooth by
		double step					// Discretization step
	) :	FIR_Filter (Filter, Blackman, Normalize, +1.0, -1.0, step, points)
	{}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const DiffFilter &object)
{
	stream << object.Summary ("Differentiator filter");
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
