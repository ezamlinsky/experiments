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
# include	"../filters/smooth.hpp"
# include	"raw.hpp"

// Bins count to instantiate a continuous theoretical model
# define	BINS	1000

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
	NONE,								// Type is unknown
	EMPIRICAL,							// Empirical distribution
	THEORETICAL_DISCRETE,				// Theoretical discrete distribution
	THEORETICAL_CONTINUOUS				// Theoretical continuous distribution
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
		double last_cdf					// Initial point of the CDF function
	){
		// Fill the theoretical CDF table
		for (const auto x : values) {
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
		const vector <double> &data,	// Empirical values for the calculation
		const vector <double> &func		// Empirical discrete CDF function
	){
		// Compute the PDF and CDF functions using discrete points
		double last_cdf = 0.0;
		size_t j = 0;
		const size_t size = values.size() - 1;
		for (size_t i = 0; i < size; i++) {

			// Get the X value
			const double x = values [i];

			// Skip all the values that are less than or equal to the X value
			while (floor (data [j]) <= x) ++j;

			// Get the last CDF point where the skip condition was correct
			const double cur_cdf = func [j - 1];

			// Save computed PDF and CDF values
			pdf.push_back (cur_cdf - last_cdf);
			cdf.push_back (cur_cdf);

			// Update the last computed CDF point
			last_cdf = cur_cdf;
		}

		// The last value
		const double cur_cdf = func [func.size() - 1];
		pdf.push_back (cur_cdf - last_cdf);
		cdf.push_back (cur_cdf);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Calculate empirical continuous PDF and CDF values                     //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// INFO:	We do linear interpolation when a value resides between
//			two empirical points
	void InitContinuous (
		const vector <double> &data,	// Empirical values for the calculation
		const vector <double> &func		// Empirical discrete CDF function
	){
		// Compute the PDF and CDF functions using created bins
		double last_cdf = NAN;
		size_t j = 0;
		const size_t size = values.size() - 1;
		for (size_t i = 0; i < size; i++) {

			// Get the X value
			const double x = values [i];

			// Skip all the values that are less than or equal to the X value
			while (data [j] <= x) ++j;

			// Get the last CDF point where the skip condition was correct
			const double less_x = data [j - 1];
			const double less_y = func [j - 1];

			// Calculate the gain value for the linear interpolation
			const double gain = (x - less_x) / (data [j] - less_x);

			// Compute interpolated value of the CDF function at the target point
			const double cur_cdf = (1.0 - gain) * less_y + gain * func [j];

			// Save computed PDF and CDF values
			pdf.push_back (cur_cdf - last_cdf);
			cdf.push_back (cur_cdf);

			// Update the last computed CDF point
			last_cdf = cur_cdf;
		}

		// The last value
		const double cur_cdf = func [func.size() - 1];
		pdf.push_back (cur_cdf - last_cdf);
		cdf.push_back (cur_cdf);
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

	// Discrete distribution
	Distribution (
		const BaseDiscrete &model,		// Theoretical model
		const vector <double> &values	// Unique values
	) :	type (THEORETICAL_DISCRETE),
		range (values),
		values (values)
	{
		// Calculate theoretical PDF and CDF values for a discrete model
		InitModel (model, model.CDF (range.Min() - 1.0));
	}

	// Discrete distribution
	Distribution (
		const BaseDiscrete &model		// Theoretical model
	) :	type (THEORETICAL_DISCRETE),
		range (model.DistLocation()),
		values (range.Linear())
	{
		// Calculate theoretical PDF and CDF values for a discrete model
		InitModel (model, model.CDF (range.Min() - 1.0));
	}

	// Continuous distribution
	Distribution (
		const BaseContinuous &model,	// Theoretical model
		const vector <double> &values	// Unique values
	) :	type (THEORETICAL_CONTINUOUS),
		range (values),
		values (values)
	{
		// Calculate theoretical PDF and CDF values for a continuous model
		InitModel (model, NAN);
	}

	// Continuous distribution
	Distribution (
		const BaseContinuous &model		// Theoretical model
	) :	type (THEORETICAL_CONTINUOUS),
		range (model.DistLocation()),
		values (range.Split (BINS))
	{
		// Calculate theoretical PDF and CDF values for a continuous model
		InitModel (model, NAN);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors from empirical data                                      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

	// Discrete distribution
	Distribution (
		const Observations &data		// Observations of a random value
	) :	type (EMPIRICAL),
		range (data.Domain()),
		values (range.Linear())
	{
		// Compute raw discrete distribution
		RawCDF raw (data);

		// Calculate empirical discrete PDF and CDF values
		InitDiscrete (raw.Values(), raw.CDF());
	}

	// Discrete distribution
	Distribution (
		const vector <double> &data		// Empirical dataset
	) :	type (EMPIRICAL),
		range (data),
		values (range.Linear())
	{
		// Compute raw discrete distribution
		RawCDF raw (move (vector <double> (data)));

		// Calculate empirical discrete PDF and CDF values
		InitDiscrete (raw.Values(), raw.CDF());
	}

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
		RawCDF raw (data);

		// Calculate empirical continuous PDF and CDF values
		InitContinuous (raw.Values(), raw.CDF());
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
		RawCDF raw (move (vector <double> (data)));

		// Calculate empirical continuous PDF and CDF values
		InitContinuous (raw.Values(), raw.CDF());
	}

	// Continuous distribution
	Distribution (
		const list &py_list,			// Empirical dataset
		size_t bins						// Bins count for a histogram
	) : Distribution (to_vector (py_list), bins)
	{}

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
