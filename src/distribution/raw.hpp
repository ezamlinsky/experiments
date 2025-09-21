/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                         CALCULATE A RAW CDF FUNCTION                         #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"../python_helpers.hpp"
# include	"../models/discrete/discrete.hpp"
# include	"../models/continuous/continuous.hpp"
# include	"../observations/observations.hpp"
# include	"../object_summary.hpp"

// Bins count to instantiate a continuous theoretical model
# define	BINS	1000

//****************************************************************************//
//      Class "RawCDF"                                                        //
//****************************************************************************//
class RawCDF
{
//============================================================================//
//      CDF function type                                                     //
//============================================================================//
public:
	enum DistType {
		NONE,							// Type is unknown
		EMPIRICAL,						// Empirical CDF function
		THEORETICAL						// Theoretical CDF function
	};

//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	DistType type;						// CDF function type
	Model::Range range;					// Values range
	vector <double> values;				// Unique values
	vector <double> cdf;				// Computed values of a CDF function

//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Calculate theoretical CDF values                                      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void Init (
		const Model::BaseModel &model	// Theoretical model
	){
		// Fill the theoretical CDF table
		for (const auto x : values)
			cdf.push_back (model.CDF (x));
	}

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
//      Default constructor                                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	RawCDF (void) : type (NONE) {}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors from a theoretical model                                 //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	RawCDF (
		const Model::BaseModel &model,		// Theoretical model
		const vector <double> &values		// Unique values
	) :	type (THEORETICAL),
		range (values),
		values (values)
	{
		// Calculate theoretical CDF values
		Init (model);
	}

	// Discrete model
	RawCDF (
		const Model::BaseDiscrete &model	// Theoretical model
	) :	type (THEORETICAL),
		range (model.DistLocation()),
		values (range.Linear())
	{
		// Calculate theoretical CDF values for a discrete model
		Init (model);
	}

	// Continuous model
	RawCDF (
		const Model::BaseContinuous &model	// Theoretical model
	) :	type (THEORETICAL),
		range (model.DistLocation()),
		values (range.Split (BINS))
	{
		// Calculate theoretical CDF values for a continuous model
		Init (model);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors from empirical data                                      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	RawCDF (
		const Observations &data		// Observations of a random value
	) :	type (EMPIRICAL),
		range (data.Domain())
	{
		// Extract the ranked dataset
		const vector <double> &temp = data.Data();

		// Check if the dataset is not empty
		if (temp.empty())
			throw invalid_argument ("RawCDF: There are no empirical observations to calculate the CDF function");

		// Calculate empirical CDF values
		Init (temp);
	}

	RawCDF (
		vector <double> &&data			// Empirical dataset
	) :	type (EMPIRICAL),
		range (data)
	{
		// Check if the dataset is not empty
		if (data.empty())
			throw invalid_argument ("RawCDF: There are no empirical observations to calculate the CDF function");

		// Sort the dataset
		sort (data.begin(), data.end());

		// Calculate empirical CDF values
		Init (data);
	}

	RawCDF (
		const vector <double> &data		// Empirical dataset
	) :	RawCDF (move (vector <double> (data)))
	{}

	RawCDF (
		const pylist &py_list			// Empirical dataset
	) : RawCDF (to_vector (py_list))
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      CDF function type                                                     //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	DistType Type (void) const {
		return type;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Domain of the raw CDF values                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const Model::Range& Domain (void) const {
		return range;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Count of collected CDF values                                         //
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
//      Find a value of the CDF function for an arbitrary Х                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double GetCDF (
		double x						// Argument value
	) const {

		// Find the greatest element that is still less than or equal to the target value
		const size_t index = Array::BinSearchLessOrEqual (values.data(), values.size(), x);
		return index != static_cast <size_t> (-1) ? cdf [index] : 0.0;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Summary of the object                                                 //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	ObjectSummary Summary (void) const {

		// Create the summary storage
		ObjectSummary summary = Domain().Summary();

		// Check the distribution type
		switch (Type())
		{
			case RawCDF::EMPIRICAL:
				summary.Name ("Empirical CDF function");
				break;

			case RawCDF::THEORETICAL:
				summary.Name ("Theoretical CDF function");
				break;

			default:
				summary.Name ("CDF function");
				break;
		}

		// Data range
		summary.Groups()[0].Name ("Data range");

		// Data size
		PropGroup size;
		size.Append ("Values", Size());
		summary.Prepend (size);

		// Return the summary
		return summary;
	}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const RawCDF &object)
{
	stream << object.Summary();
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
