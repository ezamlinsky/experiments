/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                 MINIMUM GROUP DELAY FILTER FOR A TIME SERIES                 #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"fir_filter.hpp"

// Length of the tail of the filter oscillations
# define	MINDELAY_EXTRA_PERIOD	10

//****************************************************************************//
//      Class "MinDelayFilter"                                                //
//****************************************************************************//
class MinDelayFilter : public Filter
{
//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Compute values of the windowed Sinc function                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	static vector <double> SincFilter (
		size_t points				// Count of neighbor points to smooth by
	){
		// Filter period and size
		const size_t period = points + 1;
		const size_t size = (4 * MINDELAY_EXTRA_PERIOD * period - points) / 2;

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

			// The impulse response with applied window function
			impulse.push_back (sinc * win);
		}

		// Return the impulse response
		return impulse;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Normalize the filter impulse response                                 //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	static void Normalize (
		double impulse[],			// Filter impulse response
		size_t size					// Size of impulse response
	){
		// Find the sum of the filter coefficients
		const double sum = Array::Sum (impulse, size);

		// Normalize all the filter coefficients to make the total sum equal to 1.0
		Array::Div (impulse, size, sum);
	}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	MinDelayFilter (
		size_t points				// Count of neighbor points to filter by
	) :	Filter (points)
	{
		// Calculate windowed sinc function
		vector <double> sinc_half = SincFilter (points);

		// Reserve space for the full impulse response function
		const size_t size = sinc_half.size();
		vector <double> buffer (size + points, 0.0);

		// Fill the delay part of the impulse response function
		for (size_t i = 0; i < points; i++) {
			const size_t index = i + 1;
			buffer [points - index] = sinc_half [index];
		}

		// Fill the oscillation part of the impulse response function
		for (size_t i = 0; i < size; i++)
			buffer [i + points] = sinc_half [i];

		// Normalize the filter impulse response
		Normalize (buffer.data(), buffer.size());

		// Reverse the impulse response function
		reverse (buffer.begin(), buffer.end());

		// Set the impulse response
		impulse = buffer;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Apply the filter to the target time series                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual vector <double> Apply (
		const vector <double> &data	// The time series to filter
	) const override final {

		// Clear the dataset from NaNs
		DataCleaner cleaner = DataCleaner (data);

		// Expand the clean dataset to the left
		const size_t impulse_size = impulse.size();
		const vector <double> &clean = cleaner.ExpandedData (impulse_size - 1, 0);

		// Filter response
		vector <double> response;

		// Compute the filter response
		const size_t size = data.size();
		for (size_t i = 0; i < size; i++) {

			// Find the filter response for the moving window
			const double conv = Array::SumMul (clean.data() + i, impulse.data(), impulse_size);
			response.push_back (conv);
		}

		// Restore NaN values in the impulse response
		return cleaner.Restore_NaNs (response);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Summary of the object                                                 //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	ObjectSummary Summary (void) const {

		// Create the summary storage
		ObjectSummary summary ("Minimum delay filter");

		// General info
		PropGroup info;
		info.Append ("Delay points", Points());
		info.Append ("Impulse response length", Size());
		summary.Append (info);

		// Return the summary
		return summary;
	}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const MinDelayFilter &object)
{
	stream << object.Summary();
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
