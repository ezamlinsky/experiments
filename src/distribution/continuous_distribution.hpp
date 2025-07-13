/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#       CALCULATE PDF AND CDF FOR A CONTINUOUS PROBABILITY DISTRIBUTION        #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"base_discrete.hpp"
# include	"../filters/smooth.hpp"

//****************************************************************************//
//      Class "ContinuousDistribution"                                        //
//****************************************************************************//
class ContinuousDistribution
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	Range range;						// Values range
	vector <double> values;				// Unique values
	vector <double> pdf;				// Computed values of a PDF function
	vector <double> cdf;				// Computed values of a CDF function

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Default constructor                                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	ContinuousDistribution (void)
	:	range (Range (0, 0))
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors for empirical data                                       //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	ContinuousDistribution (
		const vector <double> &data,	// Empirical dataset
		size_t bins						// Bins count for a histogram
	) : range (Range (min (data), max (data)))
	{
		// Split the range into sub-ranges (bins)
		values = range.Split (bins);

		// Compute raw CDF
		BaseDiscrete cdf_data (move (vector <double> (data)));
		const vector <double> &raw_values = cdf_data.Values();
		const vector <double> &raw_cdf = cdf_data.CDF();

		// Compute the PDF and CDF functions using created bins
		// We do linear interpolation when a value resides between
		// two empirical points
		double last = raw_cdf [0];
		pdf.push_back (NAN);
		cdf.push_back (last);

		size_t j = 0;
		size_t size = values.size();
		for (size_t i = 1; i < size; i++) {

			// Get the range split value
			const double x = values [i];

			// Skip all the values that are less than the X value
			while (raw_values [j] < x) ++j;

			// Get the last CDF point where the skip condition was correct
			const double less_x = raw_values [j - 1];
			const double less_y = raw_cdf [j - 1];

			// Calculate the gain value for the linear interpolation
			const double gain = (x - less_x) / (raw_values [j] - less_x);

			// Compute interpolated value of the CDF function at the target point
			const double cur = (1.0 - gain) * less_y + gain * raw_cdf [j];

			// Save computed PDF and CDF values
			pdf.push_back (cur - last);
			cdf.push_back (cur);

			// Update the last computed CDF point
			last = cur;
		}
	}

	ContinuousDistribution (
		const list &py_list,			// Empirical dataset
		size_t bins						// Bins count for a histogram
	) : ContinuousDistribution (to_vector (py_list), bins)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor for theoretical model                                     //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	ContinuousDistribution (
		const BaseModel &model,			// Theoretical model of the CDF
		const Range &range,				// Values range
		size_t bins						// Bins count for a histogram
	) : range (range)
	{
		// Split the range into sub-ranges (bins)
		values = range.Split (bins);

		// Fill the theoretical PDF and CDF tables
		double last = NAN;
		for (const auto x : values) {
			const double cur = model.CDF (x);
			pdf.push_back (cur - last);
			cdf.push_back (cur);
			last = cur;
		}
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Domain of the CDF                                                     //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const Range& Domain (void) const {
		return range;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Count of stored CDF data points                                       //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	int Size (void) const {
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
//      Smoothed values of the CDF function for the dataset                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	vector <double> SmoothedPDF (
		size_t points					// Count of neighbor points to smooth by
	) const {
		const SmoothFilter filter = SmoothFilter (points);
		return filter.Apply (pdf);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Values of the CDF function for the dataset                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const vector <double>& CDF (void) const {
		return cdf;
	}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const ContinuousDistribution &object)
{
	auto restore = stream.precision();
	stream.precision (PRECISION);
	stream << "\nCONTINUOUS DISTRIBUTION:" << endl;
	stream << "========================" << endl;
	stream << "Bins\t\t\t\t\t= " << object.Size() << endl;
	stream << object.Domain();
	stream.precision (restore);
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
