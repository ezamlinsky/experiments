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
# include	"data_cleaner.hpp"
# include	"base_filter.hpp"
# include	"../templates/array.hpp"
# include	"../object_summary.hpp"

//****************************************************************************//
//      Class "MedianFilter"                                                  //
//****************************************************************************//
class MedianFilter : public BaseFilter
{
//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	MedianFilter (
		size_t points				// Count of neighbor points to filter by
	) : BaseFilter (points)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Apply the filter to the target time series                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual vector <double> Apply (
		const vector <double> &data	// The time series to filter
	) const override final {

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
			const double median = Array::Median (temp.data(), temp.size());
			response.push_back (median);
		}

		// Restore NaN values in the impulse response
		return cleaner.Restore_NaNs (response);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Summary of the object                                                 //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	ObjectSummary Summary (void) const {

		// Create the summary storage
		ObjectSummary summary ("Median filter");

		// General info
		PropGroup info;
		info.Append ("Neighbor smoothing points", Points());
		summary.Append (info);

		// Return the summary
		return summary;
	}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const MedianFilter &object)
{
	stream << object.Summary();
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
