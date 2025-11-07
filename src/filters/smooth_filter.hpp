/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                       SMOOTH FILTER FOR A TIME SERIES                        #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"fir_filter.hpp"

// Length of the tail of the filter oscillations
# define	SMOOTH_EXTRA_PERIOD	10

//****************************************************************************//
//      Class "SmoothFilter"                                                  //
//****************************************************************************//
class SmoothFilter : public FIR_Filter
{
//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Compute values of the low-pass filter                                 //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	static vector <double> Filter (
		size_t points				// Count of neighbor points to smooth by
	){
		// Filter period and size
		const size_t period = points + 1;
		const size_t size = (4 * SMOOTH_EXTRA_PERIOD * period - points) / 2;

		// Compute the impulse response
		vector <double> impulse;
		impulse.push_back (1.0);
		for (size_t i = 1; i < size; i++) {

			// Sinc function
			const double x = M_PI * i / period;
			const double p = sin (x);
			const double q = x;
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
		// Find the sum of the filter coefficients, excluding the first one
		const double sum = Array::Sum (impulse + 1, size - 1);

		// Normalize all the filter coefficients to make the total sum equal to 1.0
		Array::Div (impulse, size, 1.0 + 2.0 * sum);
	}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	SmoothFilter (
		size_t points				// Count of neighbor points to smooth by
	) :	FIR_Filter (Filter, Blackman, Normalize, +1.0, +1.0, 1.0, points)
	{}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const SmoothFilter &object)
{
	stream << object.Summary ("Smooth filter");
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
