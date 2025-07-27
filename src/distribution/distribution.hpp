/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#              CALCULATE PDF AND CDF FUNCTIONS FOR A DISTRIBUTION              #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"../python_helpers.hpp"
# include	"../models/base.hpp"
# include	"../observations/observations.hpp"
# include	"../filters/smooth.hpp"

//****************************************************************************//
//      Class "Distribution"                                                  //
//****************************************************************************//
class Distribution
{
//============================================================================//
//      Members                                                               //
//============================================================================//
public:

// Distribution type
enum DistType {
	NONE,
	EMPIRICAL,							// Empirical distribution
	THEORETICAL_DISCRETE,				// Theoretical discrete distribution
	THEORETICAL_CONTINUOUS,				// Theoretical continuous distribution
};

private:
	DistType type;						// Distribution type
	Range range;						// Values range
	vector <double> values;				// Unique values
	vector <double> pdf;				// Computed values of a PDF function
	vector <double> cdf;				// Computed values of a CDF function

//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Calculate theoretical PDF and CDF values for a distribution           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void InitModel (
		const BaseModel &model,			// Theoretical model
		const vector <double> &data,	// Points to calculate the model values for
		double last_cdf					// Initial point of the CDF function
	){
		// Fill the theoretical CDF table
		for (const auto x : data) {
			const double cur_cdf = model.CDF (x);
			pdf.push_back (cur_cdf - last_cdf);
			cdf.push_back (cur_cdf);
			last_cdf = cur_cdf;
		}
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Calculate empirical discrete PDF and CDF values                       //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void InitDiscrete (
		const vector <double> &data		// Empirical data for the calculation
	){
		// Calculate empirical discrete PDF and CDF values
		size_t count = 0;
		size_t total = 0;
		double last_val = data [0];
		double last_cdf = 0.0;
		const size_t size = data.size();
		for (const auto x : data) {

			// Compare current value with the last one checked
			if (x != last_val) {

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
			last_val = x;
		}

		// Finalize the PDF and CDF tables
		total += count;
		const double curr_cdf = double (total) / double (size);
		values.push_back (last_val);
		pdf.push_back (curr_cdf - last_cdf);
		cdf.push_back (curr_cdf);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Calculate empirical continuous PDF and CDF values                     //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void InitContinuous (
		const vector <double> &data,	// Empirical data for the calculation
		const vector <double> &dcdf		// Empirical discrete CDF values
	){
		// The first value is trivial because we have the fixed minimum value
		pdf.push_back (NAN);
		cdf.push_back (0.0);

		// Compute the PDF and CDF functions using created bins
		// We do linear interpolation when a value resides between
		// two empirical points
		double last_cdf = 0.0;
		size_t j = 0;
		const size_t size = values.size();
		for (size_t i = 1; i < size; i++) {

			// Get the X value
			const double x = values [i];

			// Skip all the values that are less than the X value
			while (data [j] < x) ++j;

			// Get the last CDF point where the skip condition was correct
			const double less_x = data [j - 1];
			const double less_y = dcdf [j - 1];

			// Calculate the gain value for the linear interpolation
			const double gain = (x - less_x) / (data [j] - less_x);

			// Compute interpolated value of the CDF function at the target point
			const double cur_cdf = (1.0 - gain) * less_y + gain * dcdf [j];

			// Save computed PDF and CDF values
			pdf.push_back (cur_cdf - last_cdf);
			cdf.push_back (cur_cdf);

			// Update the last computed CDF point
			last_cdf = cur_cdf;
		}
	}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Default constructor                                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Distribution (void) : type (NONE) {}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors from a theoretical model                                 //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

	// Universal constructor
	Distribution (
		const BaseModel &model,			// Theoretical model
		const vector <double> &values	// Unique values
	) :	range (values),
		values (values)
	{
		// Calculate theoretical PDF and CDF values depending on the distribution type
		if (model.Type() == BaseModel::DISCRETE) {
			type = THEORETICAL_DISCRETE;
			InitModel (model, values, 0.0);
		}
		else {
			type = THEORETICAL_CONTINUOUS;
			InitModel (model, values, NAN);
		}
	}

	// Continuous distribution
	Distribution (
		const BaseModel &model,			// Theoretical model
		const Range &range,				// Values range
		size_t bins						// Bins count for a histogram
	) :	type (THEORETICAL_CONTINUOUS),
		range (range),
		values (range.Split (bins))
	{
		// Calculate theoretical PDF and CDF values for a Continuous model
		InitModel (model, values, NAN);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors from empirical data                                      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

	// Discrete distribution
	Distribution (
		const Observations &data		// Observations of a random value
	) :	type (EMPIRICAL),
		range (data.Domain())
	{
		// Extract the ranked dataset
		const vector <double> &temp = data.Data();

		// Check if the dataset is not empty
		if (temp.empty())
			throw invalid_argument ("BaseDiscrete: There are no empirical observations to calculate PDF and CDF functions");

		// Calculate empirical discrete PDF and CDF values
		InitDiscrete (temp);
	}

	// Discrete distribution
	Distribution (
		vector <double> &&data			// Empirical dataset
	) :	type (EMPIRICAL),
		range (data)
	{
		// Check if the dataset is not empty
		if (data.empty())
			throw invalid_argument ("BaseDiscrete: There are no empirical observations to calculate PDF and CDF functions");

		// Sort the dataset
		sort (data.begin(), data.end());

		// Calculate empirical discrete PDF and CDF values
		InitDiscrete (data);
	}

	// Discrete distribution
	Distribution (
		const vector <double> &data		// Empirical dataset
	) :	Distribution (move (vector <double> (data)))
	{}

	// Discrete distribution
	Distribution (
		const list &py_list				// Empirical dataset
	) : Distribution (to_vector (py_list))
	{}

	// Continuous distribution
	Distribution (
		const Observations &data,		// Observations of a random value
		size_t bins						// Bins count for a histogram
	) :	type (EMPIRICAL),
		range (data.Domain()),
		values (range.Split (bins))
	{
		// Compute raw discrete distribution
		Distribution temp (data);

		// Calculate empirical continuous PDF and CDF values
		InitContinuous (temp.Values(), temp.CDF());
	}

	// Continuous distribution
	Distribution (
		const vector <double> &data,	// Empirical dataset
		size_t bins						// Bins count for a histogram
	) :	type (EMPIRICAL),
		range (data),
		values (range.Split (bins))
	{
		// Compute raw discrete distribution
		Distribution temp (move (vector <double> (data)));

		// Calculate empirical continuous PDF and CDF values
		InitContinuous (temp.Values(), temp.CDF());
	}

	// Continuous distribution
	Distribution (
		const list &py_list,			// Empirical dataset
		size_t bins						// Bins count for a histogram
	) : Distribution (to_vector (py_list), bins)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Destructor                                                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	~Distribution (void) = default;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Distribution type                                                     //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	DistType Type (void) const {
		return type;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Domain of the distribution                                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const Range& Domain (void) const {
		return range;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Bins count the distribution function is split into                    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	size_t Bins (void) const {
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
//      Values of the PDF function for the dataset                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const vector <double>& PDF (void) const {
		return pdf;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Smoothed values of the CDF function for the dataset                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	vector <double> SmoothedPDF (
		size_t points					// Count of neighbor points to smooth by
	) const {
		const SmoothFilter filter = SmoothFilter (points);
		return filter.Apply (pdf);
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
//      Find a value of the PDF function for an arbitrary Х                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double GetPDF (
		double x						// Argument value
	) const {

		// Find the greatest element that is still less than or equal to the target value
		const size_t index = Array::BinSearchLessOrEqual (values.data(), values.size(), x);
		return index != static_cast <size_t> (-1) ? pdf [index] : 0.0;
	}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const Distribution &object)
{
	switch (object.Type())
	{
		case Distribution::EMPIRICAL:
			stream << "\nEMPIRICAL DISTRIBUTION:" << endl;
			stream << "=======================" << endl;
			break;

		case Distribution::THEORETICAL_DISCRETE:
			stream << "\nTHEORETICAL DISCRETE DISTRIBUTION:" << endl;
			stream << "==================================" << endl;
			break;

		case Distribution::THEORETICAL_CONTINUOUS:
			stream << "\nTHEORETICAL CONTINUOUS DISTRIBUTION:" << endl;
			stream << "====================================" << endl;
			break;

		default:
			stream << "\nDISTRIBUTION:" << endl;
			stream << "=============" << endl;
			break;
	}
	stream << "    Bins\t\t\t\t= " << object.Bins() << endl;
	stream << object.Domain();
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
