/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#          BASE CLASS TO CALCULATE THE DISCRETE PDF AND CDF FUNCTIONS          #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"../python_helpers.hpp"
# include	"../models/range.hpp"
# include	"../observations/observations.hpp"

//****************************************************************************//
//      Class "BaseDistribution"                                              //
//****************************************************************************//
class BaseDistribution
{
//============================================================================//
//      Members                                                               //
//============================================================================//
protected:
	Range range;						// Values range
	vector <double> values;				// Unique values
	vector <double> pdf;				// Computed values of a PDF function
	vector <double> cdf;				// Computed values of a CDF function

//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:
	void Init (
		const vector <double> &data		// Empirical data for the calculation
	){
		// Calculate empirical discrete PDF and CDF values
		size_t count = 0;
		size_t total = 0;
		double last_val = data [0];
		double last_cdf = 0.0;
		const size_t size = data.size();
		for (size_t i = 0; i < size; i++) {

			// Get the current value to compare with the last one checked
			const double cur_val = data [i];
			if (cur_val != last_val) {

				// Found another unique value
				total += count;
				const double curr_cdf = double (total) / double (size);
				values.push_back (last_val);
				pdf.push_back (curr_cdf - last_cdf);
				cdf.push_back (curr_cdf);

				// Remember the last value of the CDF function for the next PDF value
				last_cdf = curr_cdf;
				count = 1;
			}

			// The same value
			else count++;

			// Update the last checked value
			last_val = cur_val;
		}

		// Finalize the PDF and CDF tables
		total += count;
		const double curr_cdf = double (total) / double (size);
		values.push_back (last_val);
		pdf.push_back (curr_cdf - last_cdf);
		cdf.push_back (curr_cdf);
	}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Default constructor                                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	BaseDistribution (void) = default;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Partial constructor                                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	BaseDistribution (
		const Range &range,				// Values range
		const vector <double> &values	// Unique values
	) :	range (range),
		values (values)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors with full initialization                                 //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	BaseDistribution (
		const Observations &sample		// Observations of a random value
	) : range (sample.Domain())
	{
		// Extract the ranked dataset
		const vector <double> &data = sample.Data();

		// Check if the dataset is not empty
		if (data.empty())
			throw invalid_argument ("BaseDiscrete: There are no empirical observations to calculate PDF and CDF functions");

		// Calculate empirical discrete PDF and CDF values
		Init (data);
	}

	BaseDistribution (
		vector <double> &&data			// Empirical data for the calculation
	) : range (data)
	{
		// Check if the dataset is not empty
		if (data.empty())
			throw invalid_argument ("BaseDiscrete: There are no empirical observations to calculate PDF and CDF functions");

		// Sort the sample
		sort (data.begin(), data.end());

		// Calculate empirical discrete PDF and CDF values
		Init (data);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Destructor                                                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual ~BaseDistribution (void) = default;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Domain of the CDF                                                     //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const Range& Domain (void) const {
		return range;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Count of CDF data points for which we have values                     //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	size_t Size (void) const {
		return cdf.size();
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Unique values in the dataset                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const vector <double>& Values (void) const {
		return values;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Values of the PDF function for the dataset                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const vector <double>& PDF (void) const {
		return pdf;
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
