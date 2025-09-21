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
# include	<stdexcept>
# include	"data_cleaner.hpp"
# include	"filter.hpp"
# include	"../object_summary.hpp"
# include	"../python_helpers.hpp"

//****************************************************************************//
//      Class "SmoothFilter"                                                  //
//****************************************************************************//
class SmoothFilter : public Filter
{
//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	SmoothFilter (
		size_t points				// Count of neighbor points to smooth by
	) :	Filter (points)
	{
		if (points == 0)
			throw invalid_argument ("SmoothFilter: The number of neighbor points must be positive");

		// Calculate windowed sinc function
		vector <double> sinc_half = sinc_filter (points);

		// Reserve space for the full impulse response function
		const size_t size = sinc_half.size();
		vector <double> buffer (2 * size - 1, 0.0);

		// Reflect symmetrically the windowed sinc function to the left and right
		const size_t center_pos = size - 1;
		for (size_t i = 0; i < size; i++) {
			buffer [center_pos + i] = sinc_half [i];
			buffer [center_pos - i] = sinc_half [i];
		}

		// Set the impulse response
		impulse = normalize (buffer);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Apply the filter to the target time series                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	vector <double> Apply (
		const vector <double> &data	// The time series to smooth with the filter
	) const {

		// Clear the dataset from NaNs
		DataCleaner cleaner = DataCleaner (data);

		// Expand the clean dataset to the left and right
		const size_t impulse_size = impulse.size();
		const vector <double> &clean = cleaner.ExpandedData (impulse_size / 2, impulse_size / 2);

		// Filter response
		vector <double> response;

		// Compute the filter response
		const size_t size = data.size();
		for (size_t i = 0; i < size; i++) {

			// Find the filter response for the moving window
			double sum = 0.0;
			for (size_t j = 0; j < impulse_size; j++)
				sum += clean [i + j] * impulse [j];
			response.push_back (sum);
		}

		// Restore NaN values in the impulse response
		return cleaner.Restore_NaNs (response);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Apply the filter to the target python list                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	vector <double> Apply (
		const pylist &py_list		// The python list to smooth with the filter
	) const {
		return Apply (to_vector (py_list));
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Summary of the object                                                 //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	ObjectSummary Summary (void) const {

		// Create the summary storage
		ObjectSummary summary ("Smooth filter");

		// General info
		PropGroup info;
		info.Append ("Smoothing points", Points());
		info.Append ("Impulse response length", Size());
		summary.Append (info);

		// Return the summary
		return summary;
	}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const SmoothFilter &object)
{
	stream << object.Summary();
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
