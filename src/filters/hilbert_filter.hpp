/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                  HILBERT TRANSFORM FILTER FOR A TIME SERIES                  #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"fir_filter.hpp"

// Length of the tail of the filter oscillations
# define	HILBERT_EXTRA_PERIOD	10

//****************************************************************************//
//      Class "HilbertFilter"                                                 //
//****************************************************************************//
class HilbertFilter : public FIR_Filter
{
//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Compute values of the Hilbert transform filter                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	static vector <double> Filter (
		size_t size					// Size of impulse response
	){
		// Compute the impulse response
		vector <double> impulse;
		impulse.push_back (0.0);
		for (size_t i = 1; i < size; i++) {

			// Hilbert transformation
			const double x = M_PI * i;
			const double p = 1.0 - cos (x);
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
		// Find the sum of weighted filter coefficients
		double sum = 0;
		for (size_t i = 1; i < size; i++)
			sum += sin (0.5 * M_PI * i) * impulse [i];

		// Normalize all the filter coefficients
		Array::Div (impulse, size, 2.0 * sum);
	}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	HilbertFilter (
		size_t size					// Set size of the dataset for the Hilbert transformation
	) :	FIR_Filter (Filter, Blackman, Normalize, -1.0, +1.0, 1.0, size / 2)
	{}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const HilbertFilter &object)
{
	stream << object.Summary ("Hilbert filter");
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
