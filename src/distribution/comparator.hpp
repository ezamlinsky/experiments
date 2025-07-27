/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                  COMPARE TWO DISTRIBUTIONS WITH ONE ANOTHER                  #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	<cmath>
# include	"distribution.hpp"
# include	"../models/continuous/kolmogorov.hpp"
# include	"../models/continuous/chi_squared.hpp"

//****************************************************************************//
//      Class "DistComparator"                                                //
//****************************************************************************//
class DistComparator
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	Distribution sample;					// Sample distribution to compare
	Distribution reference;					// Reference distribution
	size_t observations;					// Number of collected observations
	size_t params;							// Number of model parameters

//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Compute the value of the Pearson's chi-squared test                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double PearsonCriteria (
		size_t start_index					// The first index to start from
	) const {

		// Get both PDF functions
		const vector <double> &src = sample.PDF();
		const vector <double> &ref = reference.PDF();

		// Find the relative error between them
		double sum = 0.0;
		const size_t size = src.size();
		for (size_t i = start_index; i < size; i++) {
			const double diff = abs (src[i] - ref[i]);
			sum += diff * diff / ref[i];
		}

		// Return the difference between the empirical dataset and the model
		return sum * observations;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Compute the value of the one-sample Kolmogorov-Smirnov test           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double KolmogorovCriteria1 (
		size_t start_index					// The first index to start from
	) const {

		// Get both CDF functions
		const vector <double> &src = sample.CDF();
		const vector <double> &ref = reference.CDF();

		// Find the supremum between two CDFs
		double max_diff = 0.0;
		const size_t size = src.size();
		for (size_t i = start_index; i < size; i++) {
			const double diff = abs (src[i] - ref[i]);
			if (max_diff < diff) max_diff = diff;
		}

		// Calculate the criteria function with the correction value
		const double criteria = sqrt (size) * max_diff;
		return criteria + 1.0 / (6.0 * sqrt (size)) + 0.25 * (criteria - 1.0) / size;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Compute the value of the two-sample Kolmogorov-Smirnov test           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double KolmogorovCriteria2 (void) const {

		// Merge all the values from both samples
		vector <double> values;
		const vector <double> &v1 = sample.Values();
		const vector <double> &v2 = reference.Values();
		merge (v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter (values));

		// Remove consecutive (adjacent) duplicates
		auto last = unique (values.begin(), values.end());
		values.erase (last, values.end());

		// Find the supremum between two CDFs
		double max_diff = 0.0;
		const size_t size = values.size();
		for (size_t i = 0; i < size; i++) {
			const double x = values[i];
			const double diff = abs (sample.GetCDF (x) - reference.GetCDF (x));
			if (max_diff < diff) max_diff = diff;
		}

		// Calculate the criteria function
		const size_t size1 = sample.Bins();
		const size_t size2 = reference.Bins();
		return sqrt (size1 * size2 / double (size1 + size2)) * max_diff;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Compute the confidence level of Pearson's chi-squared test            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double PearsonLevel (
		size_t start_index					// The first index to start from
	) const {

		// Compute the value of the Pearson's chi-squared test
		const double criteria = PearsonCriteria (start_index);

		// Return the confidence level
		size_t correction = params + start_index + 1;
		const ChiSquared &dist = ChiSquared (sample.Bins() - correction);
		return 1.0 - dist.CDF (criteria);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Compute the confidence level of the one-sample Kolmogorov-Smirnov test//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double KolmogorovLevel (
		size_t start_index					// The first index to start from
	) const {

		// Compute the value of the one-sample Kolmogorov-Smirnov test
		const double criteria = KolmogorovCriteria1 (start_index);

		// Return the confidence level
		const Kolmogorov &dist = Kolmogorov();
		return 1.0 - dist.CDF (criteria);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Pearson's chi-squared test                                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	bool PearsonTest (
		double alpha,				// Rejection level of the null hypothesis
		size_t start_index			// The first index to start from
	) const {

		// Compute the value of the Pearson's chi-squared test
		const double criteria = PearsonCriteria (start_index);

		// Find the quantile of the Chi-squared distribution
		size_t correction = params + start_index + 1;
		const ChiSquared &dist = ChiSquared (sample.Bins() - correction);
		const double quantile = dist.Quantile (alpha);

		// Check if we can accept the null hypothesis about the distribution type
		return criteria <= quantile;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      One-sample Kolmogorov-Smirnov test                                    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	bool OneSampleTest (
		double alpha,				// Rejection level of the null hypothesis
		size_t start_index			// The first index to start from
	) const {

		// Compute the value of the one-sample Kolmogorov-Smirnov test
		const double criteria = KolmogorovCriteria1 (start_index);

		// Find the quantile of the Kolmogorov distribution
		const Kolmogorov &dist = Kolmogorov();
		const double quantile = dist.Quantile (alpha);

		// Check if we can accept the null hypothesis about the distribution type
		return criteria <= quantile;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Two-sample Kolmogorov-Smirnov test                                    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	bool TwoSampleTest (
		double alpha				// Rejection level of the null hypothesis
	) const {

		// Compute the value of the two-sample Kolmogorov-Smirnov test
		const double criteria = KolmogorovCriteria2();

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
//      Constructors from empirical data                                      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

	// Discrete distribution
	DistComparator (
		const Observations &data			// Observations of a random value
	) : sample (data),
		observations (data.Size())
	{}

	// Discrete distribution
	DistComparator (
		const vector <double> &data			// Empirical data
	) : sample (data),
		observations (data.size())
	{}

	// Discrete distribution
	DistComparator (
		const list &py_list					// Empirical data
	) : DistComparator (to_vector (py_list))
	{}

	// Continuous distribution
	DistComparator (
		const Observations &data,			// Observations of a random value
		size_t bins							// Bins count for a histogram
	) : sample (data, bins),
		observations (data.Size())
	{}

	// Continuous distribution
	DistComparator (
		const vector <double> &data,		// Empirical data
		size_t bins							// Bins count for a histogram
	) : sample (data, bins),
		observations (data.size())
	{}

	// Continuous distribution
	DistComparator (
		const list &py_list,				// Empirical data
		size_t bins							// Bins count for a histogram
	) : DistComparator (to_vector (py_list), bins)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors from empirical data and a theoretical model              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

	// Discrete distribution
	DistComparator (
		const Observations &data,			// Observations of a random value
		const BaseModel &model				// Theoretical model
	) : sample (data),
		observations (data.Size())
	{
		ReferenceModel (model);
	}

	// Discrete distribution
	DistComparator (
		const vector <double> &data,		// Empirical data
		const BaseModel &model				// Theoretical model
	) : sample (data),
		observations (data.size())
	{
		ReferenceModel (model);
	}

	// Discrete distribution
	DistComparator (
		const list &py_list,				// Empirical data
		const BaseModel &model				// Theoretical model
	) : DistComparator (to_vector (py_list), model)
	{}

	// Continuous distribution
	DistComparator (
		const Observations &data,			// Observations of a random value
		const BaseModel &model,				// Theoretical model
		size_t bins							// Bins count for a histogram
	) : sample (data, bins),
		observations (data.Size())
	{
		ReferenceModel (model);
	}

	// Continuous distribution
	DistComparator (
		const vector <double> &data,		// Empirical data
		const BaseModel &model,				// Theoretical model
		size_t bins							// Bins count for a histogram
	) : sample (data, bins),
		observations (data.size())
	{
		ReferenceModel (model);
	}

	// Continuous distribution
	DistComparator (
		const list &py_list,				// Empirical data
		const BaseModel &model,				// Theoretical model
		size_t bins							// Bins count for a histogram
	) : DistComparator (to_vector (py_list), model, bins)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors from empirical data only (a sample and a reference)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

	// Discrete distribution
	DistComparator (
		const Observations &sample,			// Sample observations
		const Observations &reference		// Reference observations
	) : sample (sample),
		observations (sample.Size())
	{
		ReferenceSample (reference);
	}

	// Discrete distribution
	DistComparator (
		const vector <double> &sample,		// Empirical sample data
		const vector <double> &reference	// Empirical reference data
	) : sample (sample),
		observations (sample.size())
	{
		ReferenceSample (reference);
	}

	// Discrete distribution
	DistComparator (
		const list &sample,					// Empirical sample data
		const list &reference				// Empirical reference data
	) : DistComparator (to_vector (sample), to_vector (reference))
	{}

	// Continuous distribution
	DistComparator (
		const Observations &sample,			// Sample observations
		const Observations &reference,		// Reference observations
		size_t bins							// Bins count for a histogram
	) : sample (sample, bins),
		observations (sample.Size())
	{
		ReferenceSample (reference);
	}

	// Continuous distribution
	DistComparator (
		const vector <double> &sample,		// Empirical sample data
		const vector <double> &reference,	// Empirical reference data
		size_t bins							// Bins count for a histogram
	) : sample (sample, bins),
		observations (sample.size())
	{
		ReferenceSample (reference);
	}

	// Continuous distribution
	DistComparator (
		const list &sample,					// Empirical sample data
		const list &reference,				// Empirical reference data
		size_t bins							// Bins count for a histogram
	) : DistComparator (to_vector (sample), to_vector (reference), bins)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Load another sample as a reference for the distribution test          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void ReferenceSample (
		const Observations &data			// Observations of a random value
	){
		// Check type of the sample distribution
		if (sample.Type() == Distribution::EMPIRICAL_DISCRETE)
			reference = Distribution (data);
		else
			reference = Distribution (data, sample.Bins());
		params = 0;
	}

	void ReferenceSample (
		const vector <double> &data			// Empirical data
	){
		// Check type of the sample distribution
		if (sample.Type() == Distribution::EMPIRICAL_DISCRETE)
			reference = Distribution (data);
		else
			reference = Distribution (data, sample.Bins());
		params = 0;
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
		const BaseModel &model				// Theoretical model
	){
		// Check if empirical data range is inside the model domain
		const Range &range =  model.Domain();
		if (range.IsInside (sample.Domain())) {

			// Set the distribution model
			reference = Distribution (model, sample.Values());
			params = model.Parameters();
		}
		else
			throw invalid_argument ("ReferenceModel: The sample data range is outside the distribution model domain");
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Return sample distribution                                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const Distribution& Sample (void) const {
		return sample;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Return reference distribution                                         //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const Distribution& Reference (void) const {
		return reference;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Confidence level of Pearson's chi-squared test                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// NOTE: Bigger confidence levels indicate we should accept the null hypothesis
// about the distribution type
	double PearsonConfidenceLevel (void) const {

		// Check if the sample and the reference distributions are set
		if (sample.Bins() && reference.Bins()) {

			// Check type of the reference distribution
			Distribution::DistType type = reference.Type();
			if (type == Distribution::THEORETICAL_DISCRETE)
				return PearsonLevel (0);
			else if (type == Distribution::THEORETICAL_CONTINUOUS)
				return PearsonLevel (1);
			else
				throw invalid_argument ("PearsonConfidenceLevel: Can calculate the critical confidence level for a theoretical model only");
		}
		else
			throw invalid_argument ("PearsonConfidenceLevel: Set a sample and a reference for the confidence level");
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Confidence level of the one-sample Kolmogorov-Smirnov test            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// NOTE: Bigger confidence levels indicate we should accept the null hypothesis
// about the distribution type
	double KolmogorovConfidenceLevel (void) const {

		// Check if the sample and the reference distributions are set
		if (sample.Bins() && reference.Bins()) {

			// Check type of the sample distribution
			if (sample.Type() == Distribution::EMPIRICAL_DISCRETE) {

				// Check type of the reference distribution
				Distribution::DistType type = reference.Type();
				if (type == Distribution::THEORETICAL_DISCRETE)
					return KolmogorovLevel (0);
				else if (type == Distribution::THEORETICAL_CONTINUOUS)
					return KolmogorovLevel (1);
				else
					throw invalid_argument ("KolmogorovConfidenceLevel: Can calculate the critical confidence level for a theoretical model only");
			}
			else
				throw invalid_argument ("KolmogorovConfidenceLevel: Do not group observations for this test");
		}
		else
			throw invalid_argument ("KolmogorovConfidenceLevel: Set a sample and a reference for the confidence level");
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Perform the Pearson's chi-squared test                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	bool PearsonChiSquaredTest (
		double level						// Confidence level
	) const {

		// Check if the level is correct
		if (0.0 <= level && level <= 1.0) {

			// Check if the sample and the reference distributions are set
			if (sample.Bins() && reference.Bins()) {

				// Check type of the reference distribution
				Distribution::DistType type = reference.Type();
				if (type == Distribution::THEORETICAL_DISCRETE)
					return PearsonTest (1.0 - level, 0);
				else if (type == Distribution::THEORETICAL_CONTINUOUS)
					return PearsonTest (1.0 - level, 1);
				else
					throw invalid_argument ("PearsonTest: Pearson's chi-squared test is available for a theoretical model only");
			}
			else
				throw invalid_argument ("PearsonTest: Set a sample and a reference for the test");
		}
		else
			throw invalid_argument ("PearsonTest: The confidence level must be in the range [0..1]");
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Perform the one-sample or two-sample Kolmogorov-Smirnov test          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	bool KolmogorovSmirnovTest (
		double level						// Confidence level
	) const {

		// Check if the level is correct
		if (0.0 <= level && level <= 1.0) {

			// Check if the sample and the reference distributions are set
			if (sample.Bins() && reference.Bins()) {

				// Check type of the sample distribution
				if (sample.Type() == Distribution::EMPIRICAL_DISCRETE) {

					// Check type of the reference distribution
					Distribution::DistType type = reference.Type();
					if (type == Distribution::THEORETICAL_DISCRETE)
						return OneSampleTest (1.0 - level, 0);
					else if (type == Distribution::THEORETICAL_CONTINUOUS)
						return OneSampleTest (1.0 - level, 1);
					else
						return TwoSampleTest (1.0 - level);
				}
				else
					throw invalid_argument ("KolmogorovSmirnovTest: Do not group observations for this test");
			}
			else
				throw invalid_argument ("KolmogorovSmirnovTest: Set a sample and a reference for the test");
		}
		else
			throw invalid_argument ("KolmogorovSmirnovTest: The confidence level must be in the range [0..1]");
	}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const DistComparator &object)
{
	auto restore = stream.precision();
	stream.precision (PRECISION);
	stream << "\nCOMPARATOR FOR DISTRIBUTIONS:" << endl;
	stream << "=============================" << endl;
	stream << "Sample data points\t\t\t= " << object.Sample().Bins() << endl;
	stream << "Reference data points\t\t\t= " << object.Reference().Bins() << endl;
	stream.precision (restore);
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
