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
# include	<vector>
# include	<algorithm>
# include	<boost/python.hpp>
# include	"../models/range.hpp"

// Use shortenings
using namespace boost::python;
using boost::python::list;
using boost::python::stl_input_iterator;

//****************************************************************************//
//      Return the minimum value among the array elements                     //
//****************************************************************************//
static double min (
	const vector <double> &array	// The array to scan for the minimum value
){
	// Check if the array is not empty
	if (array.size())
		return *min_element (array.begin(), array.end());

	// The Range class expects this value for an empty array
	else return 0.0;
}

//****************************************************************************//
//      Return the maximum value among the array elements                     //
//****************************************************************************//
static double max (
	const vector <double> &array	// The array to scan for the maximum value
){
	// Check if the array is not empty
	if (array.size())
		return *max_element (array.begin(), array.end());

	// The Range class expects this value for an empty array
	else return 0.0;
}

//****************************************************************************//
//      Convert a Python list to a standard array                             //
//****************************************************************************//
static vector <double> to_vector (
	const list &py_list				// The Python list to convert
){
	return vector <double> (
		stl_input_iterator <double> (py_list),
		stl_input_iterator <double> ()
	);
}

//****************************************************************************//
//      Class "BaseDiscrete"                                                  //
//****************************************************************************//
class BaseDiscrete
{
//============================================================================//
//      Members                                                               //
//============================================================================//
protected:
	Range range;			// Values range
	vector <double> values;	// Unique values
	vector <double> pdf;	// Computed values of a PDF function
	vector <double> cdf;	// Computed values of a CDF function

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Default constructor                                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	BaseDiscrete (void)
	:	range (Range (0, 0))
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Partial constructor                                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	BaseDiscrete (
		const Range &range,				// Values range
		const vector <double> &values	// Unique values
	) :	range (range),
		values (values)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor with full initialization                                  //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	BaseDiscrete (
		vector <double> &&data			// Empirical data for the calculation
	) : range (Range (min (data), max (data)))
	{
		// Check if the data vector is not empty
		if (data.empty())
			throw invalid_argument ("BaseDiscrete: There are no empirical observations to calculate PDF and CDF functions");

		// Sort the sample
		sort (data.begin(), data.end());

		// Calculate empirical discrete PDF and CDF values
		size_t count = 0;
		size_t total = 0;
		double last_val = data [0];
		double last_cdf = 0.0;
		size_t size = data.size();
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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Destructor                                                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual ~BaseDiscrete (void) = default;

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
