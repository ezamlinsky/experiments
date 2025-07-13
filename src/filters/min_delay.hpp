/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                 MINIMUM GROUP DELAY FILTER FOR A TIME SERIES                 #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# include	"smooth.hpp"

//****************************************************************************//
//      Class "MinDelayFilter"                                                //
//****************************************************************************//
class MinDelayFilter : public Filter
{
//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	MinDelayFilter (
		size_t points				// Count of neighbor points to filter by
	){
		if (points == 0)
			throw invalid_argument ("MinDelayFilter: The number of neighbor points must be positive");

		// Calculate windowed sinc function
		vector <double> sinc_half = sinc_filter (points);

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

		// Set the impulse response
		impulse = normalize (buffer);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Apply the filter to the target time series                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	vector <double> Apply (
		const vector <double> &data	// The time series to filter
	) const {

		// Clear the dataset from NaNs
		DataCleaner cleaner = DataCleaner (data);

		// Expand the clean dataset to the left and right
		const size_t impulse_size = impulse.size();
		const vector <double> &clean = cleaner.ExpandedData (impulse_size - 1, 0);

		// Filter response
		vector <double> response;

		// Compute the filter response
		const size_t size = data.size();
		for (size_t i = 0; i < size; i++) {

			// Find the filter response for the moving window
			double sum = 0.0;
			for (size_t j = 0; j < impulse_size; j++) {
				const size_t index = impulse_size - 1 - j;
				sum += clean [i + index] * impulse [j];
			}
			response.push_back (sum);
		}

		// Restore NaN values in the impulse response
		return cleaner.Restore_NaNs (response);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Apply the filter to the target python list                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	vector <double> Apply (
		const list &py_list			// The python list to filter
	) const {
		return Apply (to_vector (py_list));
	}
};
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
