/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                  COMPARE TWO CDF FUNCTIONS WITH ONE ANOTHER                  #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# include	<cmath>
# include	"cdf_values.hpp"

//****************************************************************************//
//      Class "CDFs"                                                          //
//****************************************************************************//
class CDFs
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	shared_ptr <const CDFValues> sample;
	shared_ptr <const CDFValues> reference;

//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      One-sample Kolmogorov-Smirnov test                                    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	bool OneSampleTest (
		double alpha		// Rejection level of the null hypothesis
	) const {

		// Get both CDF functions
		const vector <double> &src = sample -> CDF();
		const vector <double> &ref = reference -> CDF();

		// Find the supremum between two CDFs
		double max_diff = 0.0;
		const size_t size = src.size();
		for (size_t i = 0; i < size; i++) {
			const double diff = abs (src[i] - ref[i]);
			if (max_diff < diff) max_diff = diff;
		}

		// Calculate the criteria function with the correction value
		const double criteria = sqrt (size) * max_diff;
		const double corrected = criteria + 1.0 / (6.0 * sqrt (size)) + 0.25 * (criteria - 1.0) / size;

		// Find the quantile of the Kolmogorov distribution
		const Kolmogorov &dist = Kolmogorov();
		const double quantile = dist.Quantile (alpha);

		// Check if we can accept the null hypothesis about the distribution type
		return corrected <= quantile;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Two-sample Kolmogorov-Smirnov test                                    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	bool TwoSampleTest (
		double alpha		// Rejection level of the null hypothesis
	) const {

		// Merge all the values from both samples
		vector <double> values;
		const vector <double> &v1 = sample -> Values();
		const vector <double> &v2 = reference -> Values();
		merge (v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter (values));

		// Remove consecutive (adjacent) duplicates
		auto last = unique (values.begin(), values.end());
		values.erase (last, values.end());

		// Find the supremum between two CDFs
		double max_diff = 0.0;
		const size_t size = values.size();
		for (size_t i = 0; i < size; i++) {
			const double x = values[i];
			const double diff = abs (sample -> GetCDF (x) - reference -> GetCDF (x));
			if (max_diff < diff) max_diff = diff;
		}

		// Calculate the criteria function
		const size_t size1 = sample -> Size();
		const size_t size2 = reference -> Size();
		const double criteria = sqrt (size1 * size2 / double (size1 + size2)) * max_diff;

		// Find the quantile of the Kolmogorov distribution
		const Kolmogorov &dist = Kolmogorov();
		const double quantile = dist.Quantile (alpha);

		// Check if we can accept the null hypothesis about the distribution type
		return criteria <= quantile;
	}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors for empirical data                                       //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	CDFs (
		const vector <double> &data			// Empirical data
	) : sample (new CDFValues (data)),
		reference (new CDFValues())
	{}

	CDFs (
		const list &py_list					// Empirical data
	) : CDFs (to_vector (py_list))
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors for empirical data and a theoretical model               //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	CDFs (
		const vector <double> &data,		// Empirical data
		const BaseModel &model				// Theoretical model of the CDF
	) : CDFs (data)
	{
		ReferenceModel (model);
	}

	CDFs (
		const list &py_list,				// Empirical data
		const BaseModel &model				// Theoretical model of the CDF
	) : CDFs (to_vector (py_list), model)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors for empirical data only (a sample and a reference)       //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	CDFs (
		const vector <double> &sample,		// Empirical sample data
		const vector <double> &reference	// Empirical reference data
	) : CDFs (sample)
	{
		ReferenceSample (reference);
	}

	CDFs (
		const list &sample,					// Empirical sample data
		const list &reference				// Empirical reference data
	) : CDFs (to_vector (sample), to_vector (reference))
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Load another sample as a reference for the distribution test          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void ReferenceSample (
		const vector <double> &data			// Empirical data
	){
		reference = shared_ptr <const CDFValues> (new CDFValues (data));
	}

	void ReferenceSample (
		const list &py_list					// Empirical data
	){
		ReferenceSample (to_vector (py_list));
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Load a distribution model as a reference for the distribution test    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void ReferenceModel (
		const BaseModel &model				// Theoretical model of the CDF
	){
		const Range& range = sample -> Domain();
		const vector <double> &values = sample -> Values();
		reference = shared_ptr <const CDFValues> (new CDFValues (range, values, model));
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Return sample CDF function                                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const CDFValues& Sample (void) const {
		return *sample;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Return reference CDF function                                         //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const CDFValues& Reference (void) const {
		return *reference;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Do one-sample and two-sample Kolmogorov-Smirnov tests for two CDFs    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	bool KolmogorovSmirnovTest (
		double level						// Confidence level
	) const {
		if (0.0 <= level && level <= 1.0) {
			if (sample -> Size() && reference -> Size()) {
				if (reference -> IsTheoretical())
					return OneSampleTest (1.0 - level);
				else
					return TwoSampleTest (1.0 - level);
			}
			else
				return false;
		}
		else
			throw invalid_argument ("KolmogorovSmirnovTest: The confidence level must be in the range [0..1]");
	}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const CDFs &object)
{
	auto restore = stream.precision();
	stream.precision (PRECISION);
	stream << "\nCDF FUNCTIONS:" << endl;
	stream << "==============" << endl;
	stream << "Sample data points\t\t\t= " << object.Sample().Size() << endl;
	stream << "Reference data points\t\t\t= " << object.Reference().Size() << endl;
	stream.precision (restore);
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
