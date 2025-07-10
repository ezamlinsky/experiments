/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#      CALCULATE A CUMULATIVE DISTRIBUTION FUNCTION USING DATA OR A MODEL      #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	<vector>
# include	<algorithm>
# include	<boost/python.hpp>
# include	"../models/base.hpp"
# include	"../models/kolmogorov.hpp"

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
//      Compute the CDF for empirical data set                                //
//****************************************************************************//
static bool compute_cdf (
	const vector <double> &data,	// Empirical data for the calculation
	vector <double> &values,		// Location to store unique values
	vector <double> &cdf			// Location to store the CDF
){
	// Check if the data vector is not empty
	if (!data.empty()) {

		// Copy the original data vector for RW access
		vector <double> temp = data;

		// Sort the sample
		sort (temp.begin(), temp.end());

		// Calculate empirical CDF values
		size_t count = 0;
		size_t total = 0;
		double last = temp [0];
		size_t size = temp.size();
		for (size_t i = 0; i < size; i++) {
			const double cur = temp [i];
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
		return true;
	}

	// The data set is empty
	else return false;
}

//****************************************************************************//
//      Class "CumulativeFunction"                                            //
//****************************************************************************//
class CumulativeFunction
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	Range range;			// Values range
	vector <double> values;	// Unique values
	vector <double> cdf;	// An empirical or a theoretical CDF
	bool theoretical;		// If set, then the CDF is calculated theoretically

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Default constructor                                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	CumulativeFunction (void)
	:	range (Range (0, 0)),
		theoretical (false)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor for empirical data                                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	CumulativeFunction (
		const vector <double> &data		// Empirical dataset
	) : range (Range (min (data), max (data))),
		theoretical (false)
	{
		// Compute the empirical CDF table
		compute_cdf (data, values, cdf);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor for a theoretical model                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	CumulativeFunction (
		const Range &range,				// Values range
		const vector <double> &values,	// Unique values
		const BaseModel &model			// Theoretical model of the CDF
	) : range (range),
		values (values),
		theoretical (true)
	{
		// Fill the theoretical CDF table
		for (const auto x : values)
			cdf.push_back (model.CDF (x));
	}

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
//      Values of the CDF function for the dataset                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const vector <double>& CDF (void) const {
		return cdf;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Find a value for the CDF function for an arbitrary Х                  //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double GetCDF (
		double x						// Argument value
	) const {

		// The argument is less than the minimum value
		if (x < values[0])
			return 0.0;

		// Binary search for the greater value: v[i] <= x
		size_t left = 0;
		size_t right = values.size();
		while (left < right) {
			const size_t mid = (left + right) / 2;
			if (values [mid] <= x)
				left = mid + 1;
			else
				right = mid;
		}
		return cdf [left - 1];
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Indicate that the CDF function has been calculated theoretically      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	bool IsTheoretical (void) const {
		return theoretical;
	}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const CumulativeFunction &object)
{
	auto restore = stream.precision();
	stream.precision (PRECISION);
	if (object.IsTheoretical()) {
		stream << "\nTHEORETICAL CDF VALUES:" << endl;
		stream << "=======================" << endl;
	}
	else {
		stream << "\nEMPIRICAL CDF VALUES:" << endl;
		stream << "=====================" << endl;
	}
	stream << "Data points\t\t\t\t= " << object.Size() << endl;
	stream << object.Domain();
	stream.precision (restore);
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
