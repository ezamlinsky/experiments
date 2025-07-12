/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                   BASE CLASS TO STORE A CDF FUNCTION DATA                    #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	<vector>
# include	<algorithm>
# include	<boost/python.hpp>

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
//      Class "BaseCDF"                                                       //
//****************************************************************************//
class BaseCDF
{
//============================================================================//
//      Members                                                               //
//============================================================================//
protected:
	vector <double> values;	// Unique values
	vector <double> cdf;	// An empirical or a theoretical CDF

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Default constructor                                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	BaseCDF (void) = default;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Partial constructor                                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	BaseCDF (
		const vector <double> &values	// Unique values
	) :	values (values)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor with full initialization                                  //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	BaseCDF (
		vector <double> &&data			// Empirical data for the calculation
	){
		// Check if the data vector is not empty
		if (data.empty())
			throw invalid_argument ("BaseCDF: There are no empirical observations to calculate a CDF");

		// Sort the sample
		sort (data.begin(), data.end());

		// Calculate empirical CDF values
		size_t count = 0;
		size_t total = 0;
		double last = data [0];
		size_t size = data.size();
		for (size_t i = 0; i < size; i++) {
			const double cur = data [i];
			if (cur != last) {
				total += count;
				values.push_back (last);
				cdf.push_back (double (total) / double (size));
				count = 1;
			}
			else
				count++;
			last = cur;
		}
		total += count;
		values.push_back (last);
		cdf.push_back (double (total) / double (size));
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
