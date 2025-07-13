/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#           BASE CLASS AND ALGORITHMS TO IMPLEMENT A LOW-PASS FILTER           #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	<cmath>
# include	<vector>

// Length of the tail of sics function oscillations
# define	EXTRA_PERIOD	10

//****************************************************************************//
//      Compute values of the windowed Sinc function                          //
//****************************************************************************//
static vector <double> sinc_filter (
	size_t points				// Count of neighbor points to smooth by
){
	// Filter period and size
	const size_t period = points + 1;
	const size_t size = 0.5 * (4 * EXTRA_PERIOD * period - points);

	// Blackman window properties
	const double alpha = 0.16;
	const double a0 = 0.5 * (1.0 - alpha);
	const double a1 = 0.5;
	const double a2 = 0.5 * alpha;

	// Compute the impulse response function
	vector <double> impulse;
	impulse.push_back (1.0);
	for (size_t i = 1; i < size; i++) {

		// Sinc function
		const double x = M_PI * i / period;
		const double sinc = sin (x) / x;

		// Blackman window
		const double temp = M_PI * i / size;
		const double win = a0 + a1 * cos (temp) + a2 * cos (2 * temp);

		// Final impulse response with applied window function
		impulse.push_back (sinc * win);
	}

	// Return the impulse response function
	return impulse;
}

//****************************************************************************//
//      Normalize the vector coefficients                                     //
//****************************************************************************//
static vector <double>& normalize (
	vector <double> &data		// The vector to normalize
){
	// Find the sum value
	double sum = 0;
	const size_t size = data.size();
	for (size_t i = 0; i < size; i++)
		sum += data [i];

	// Normalize all the vector values to make the total sum equal to 1.0
	for (size_t i = 0; i < size; i++)
		data [i] /= sum;

	// Return the same but normalized vector
	return data;
}

//****************************************************************************//
//      Class "Filter"                                                        //
//****************************************************************************//
class Filter
{
//============================================================================//
//      Members                                                               //
//============================================================================//
protected:
	vector <double> impulse;

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Default constructor                                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Filter (void) = default;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Size of the impulse response function                                 //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	size_t Size (void) const {
		return impulse.size();
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Values of the impulse response function                               //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const vector <double>& Impulse (void) const {
		return impulse;
	}
};
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
