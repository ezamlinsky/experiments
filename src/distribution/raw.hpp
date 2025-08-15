/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#               CALCULATE A RAW CDF FUNCTION FOR A DISTRIBUTION                #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"../python_helpers.hpp"
# include	"../observations/observations.hpp"

//****************************************************************************//
//      Class "RawCDF"                                                        //
//****************************************************************************//
class RawCDF
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	vector <double> values;				// Unique values
	vector <double> cdf;				// Computed values of a CDF function

//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Calculate empirical discrete CDF values                               //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void Init (
		const vector <double> &data		// Empirical data for the calculation
	){
		// Calculate empirical discrete PDF and CDF values
		size_t count = 0;
		size_t total = 0;
		double last = data [0];
		const size_t size = data.size();
		for (const auto x : data) {

			// Compare current value with the last one checked
			if (x != last) {

				// Found another unique value
				total += count;
				const double sum = double (total) / double (size);
				values.push_back (last);
				cdf.push_back (sum);
				count = 1;
			}

			// The same value
			else count++;

			// Update the last checked value
			last = x;
		}

		// Finalize the PDF and CDF tables
		total += count;
		const double sum = double (total) / double (size);
		values.push_back (last);
		cdf.push_back (sum);
	}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors                                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	RawCDF (
		const Observations &data		// Observations of a random value
	){
		// Extract the ranked dataset
		const vector <double> &temp = data.Data();

		// Check if the dataset is not empty
		if (temp.empty())
			throw invalid_argument ("RawDistribution: There are no empirical observations to calculate PDF and CDF functions");

		// Calculate empirical discrete PDF and CDF values
		Init (temp);
	}

	RawCDF (
		vector <double> &&data			// Empirical dataset
	){
		// Check if the dataset is not empty
		if (data.empty())
			throw invalid_argument ("RawDistribution: There are no empirical observations to calculate PDF and CDF functions");

		// Sort the dataset
		sort (data.begin(), data.end());

		// Calculate empirical discrete PDF and CDF values
		Init (data);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Unique values in the dataset                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const vector <double>& Values (void) const {
		return values;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Values of the CDF function for the dataset                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const vector <double>& CDF (void) const {
		return cdf;
	}
};
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
