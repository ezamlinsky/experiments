/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                       MEDIAN FILTER FOR A TIME SERIES                        #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	<stdexcept>
# include	"data_cleaner.hpp"
# include	"../python_helpers.hpp"

//****************************************************************************//
//      Class "MedianFilter"                                                  //
//****************************************************************************//
class MedianFilter
{
//============================================================================//
//      Members                                                               //
//============================================================================//
protected:
	size_t points;

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	MedianFilter (
		size_t points				// Count of neighbor points to filter by
	) : points (points)
	{
		if (points == 0)
			throw invalid_argument ("MedianFilter: The number of neighbor points must be positive");
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
		const vector <double> &clean = cleaner.ExpandedData (points, points);

		// Filter response
		vector <double> response;

		// Compute the filter response
		const size_t size = data.size();
		for (size_t i = 0; i < size; i++) {

			// Create a slice from the dataset
			const auto start_pos = clean.begin() + i;
			vector <double> temp = vector <double> (start_pos, start_pos + 2 * points + 1);

			// Find the median value of the moving window
			sort (temp.begin(), temp.end());
			response.push_back (temp [points]);
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
