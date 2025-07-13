/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                      CLEAR THE DATASET FROM NAN VALUES                       #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	<cmath>
# include	<vector>

// Use shortenings
using namespace std;

//****************************************************************************//
//      Class "DataCleaner"                                                   //
//****************************************************************************//
class DataCleaner
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	const vector <double> dirty;		// Dirty (original) data
	vector <double> clean;				// Clean data

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	DataCleaner (
		const vector <double> &data		// Dirty dataset to clear from NaNs
	) : dirty (data)
	{
		// Create a copy of the dirty dataset
		clean = dirty;

		// Get data size
		size_t size = clean.size();

		// Replace all the beginning NaN values with the first not NAN
		for (size_t i = 0; i < size; i++) {
			const size_t index = i;
			const double value = clean [index];
			if (!isnan (value)) {
				for (size_t j = 0; j < index; j++)
					clean[j] = value;
				break;
			}
		}

		// Replace all the ending NaN values with the first not NAN
		for (size_t i = 0; i < size; i++) {
			const size_t index = size - 1 - i;
			const double value = clean [index];
			if (!isnan (value)) {
				for (size_t j = index + 1; j < size; j++)
					clean [j] = value;
				break;
			}
		}
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Dirty (original) data                                                 //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const vector <double> DirtyData (void) const {
		return dirty;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Clean data                                                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const vector <double> CleanData (void) const {
		return clean;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Clean data expanded to the left/right with the outermost element      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const vector <double> ExpandedData (
		size_t left_expansion,			// Copy count for the left-most element
		size_t right_expansion			// Copy count for the right-most element
	) const {

		// Clean dataset (expanded)
		vector <double> result;

		// Expand the clean dataset to the left with the left-most element
		result.insert (result.end(), left_expansion, clean.at (0));

		// Copy all the elements from the clean dataset
		result.insert (result.end(), clean.begin(), clean.end());

		// Expand the clean dataset to the right with the right-most element
		result.insert (result.end(), right_expansion, clean.at (clean.size() - 1));

		// Return expanded dataset
		return result;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Copy NaN values from the original (dirty) data to the target dataset  //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	vector <double>& Restore_NaNs (
		vector <double> &data			// The dataset to copy NaN values
	) const {

		// Use the size of the shortest dataset for NaN restoration
		const size_t size = min (data.size(), dirty.size());

		// Restore the NaN values from the original dataset
		for (size_t i = 0; i < size; i++) {
			if (isnan (dirty[i]))
				data[i] = NAN;
		}

		// Return the data after processing
		return data;
	}
};
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
