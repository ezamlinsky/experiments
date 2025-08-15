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
# include	"../models/discrete/uniform.hpp"
# include	"../models/discrete/binomial.hpp"
# include	"../models/discrete/negative_binomial.hpp"
# include	"../models/discrete/bernoulli.hpp"
# include	"../models/discrete/geometric.hpp"
# include	"../models/discrete/poisson.hpp"
# include	"../models/continuous/chi_squared.hpp"

//****************************************************************************//
//      Class "PearsonScore"                                                  //
//****************************************************************************//
struct PearsonScore
{
	string name;							// Distribution model name
	double score;							// Score points

	// Check two instances for equality (required by the vector template)
	bool operator== (const PearsonScore &obj) const {
		return (name == obj.name && score == obj.score);
	}
};

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
//      Perform a test of a distribution model                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	template <typename T>
	void TestModel (
		vector <PearsonScore> &table,	// Score table
		const Observations &data,			// Observations of a random value
		const string name					// Distribution model name
	)
	try {
		// Set the distribution model
		ReferenceModel (T (data));

		// Try to estimate the confidence level of Pearson's chi-squared test
		const double level = PearsonConfidenceLevel();
		if (!isnan (level))
			table.push_back (PearsonScore {name, level});

	} catch (const invalid_argument &exception) {}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Perform a test of a distribution model with the range validation      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	template <typename T>
	void TestModelWithRange (
		vector <PearsonScore> &table,	// Score table
		const Observations &data,			// Observations of a random value
		const string name					// Distribution model name
	)
	try {
		if (T::InDomain (data.Domain())) {
			TestModel <T> (table, data, name);
		}
	} catch (const invalid_argument &exception) {}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Compute the value of the Pearson's chi-squared test                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double PearsonCriteria (void) const {

		// Get both PDF functions
		const vector <double> &src = sample.PDF();
		const vector <double> &ref = reference.PDF();

		// Find the relative error between them
		double sum = 0.0;
		const size_t size = src.size();
		for (size_t i = 0; i < size; i++) {
			const double diff = abs (src[i] - ref[i]);
			sum += diff * diff / ref[i];
		}

		// Return the difference between the empirical dataset and the model
		return sum * observations;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Compute the confidence level of Pearson's chi-squared test            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double PearsonLevel (void) const {

		// Compute the value of the Pearson's chi-squared test
		const double criteria = PearsonCriteria();

		// Calculate the degrees of freedom of the Chi-squared distribution
		size_t bins = sample.Bins();
		size_t correction = params + 1;
		if (bins > correction) {

			// Return the confidence level
			const ChiSquared &dist = ChiSquared (bins - correction);
			return 1.0 - dist.CDF (criteria);
		}

		// The test criteria are malformed
		throw invalid_argument ("PearsonLevel: Degrees of freedom of the Chi-squared distribution are malformed");
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Pearson's chi-squared test                                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	bool PearsonTest (
		double alpha				// Rejection level of the null hypothesis
	) const {

		// Compute the value of the Pearson's chi-squared test
		const double criteria = PearsonCriteria();

		// Calculate the degrees of freedom of the Chi-squared distribution
		size_t bins = sample.Bins();
		size_t correction = params + 1;
		if (bins > correction) {

			// Find the quantile of the Chi-squared distribution
			const ChiSquared &dist = ChiSquared (bins - correction);
			const double quantile = dist.Quantile (alpha);

			// Check if we can accept the null hypothesis about the distribution type
			return criteria <= quantile;
		}

		// The test criteria are malformed
		throw invalid_argument ("PearsonTest: Degrees of freedom of the Chi-squared distribution are malformed");
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
		observations (data.Size()),
		params (0)
	{}

	// Discrete distribution
	DistComparator (
		const vector <double> &data			// Empirical data
	) : sample (data),
		observations (data.size()),
		params (0)
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
		observations (data.Size()),
		params (0)
	{}

	// Continuous distribution
	DistComparator (
		const vector <double> &data,		// Empirical data
		size_t bins							// Bins count for a histogram
	) : sample (data, bins),
		observations (data.size()),
		params (0)
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
		const BaseDiscrete &model			// Theoretical model
	) : DistComparator (data)
	{
		ReferenceModel (model);
	}

	// Discrete distribution
	DistComparator (
		const vector <double> &data,		// Empirical data
		const BaseDiscrete &model			// Theoretical model
	) : DistComparator (data)
	{
		ReferenceModel (model);
	}

	// Discrete distribution
	DistComparator (
		const list &py_list,				// Empirical data
		const BaseDiscrete &model			// Theoretical model
	) : DistComparator (to_vector (py_list), model)
	{}

	// Continuous distribution
	DistComparator (
		const Observations &data,			// Observations of a random value
		const BaseContinuous &model,		// Theoretical model
		size_t bins							// Bins count for a histogram
	) : DistComparator (data, bins)
	{
		ReferenceModel (model);
	}

	// Continuous distribution
	DistComparator (
		const vector <double> &data,		// Empirical data
		const BaseContinuous &model,		// Theoretical model
		size_t bins							// Bins count for a histogram
	) : DistComparator (data, bins)
	{
		ReferenceModel (model);
	}

	// Continuous distribution
	DistComparator (
		const list &py_list,				// Empirical data
		const BaseContinuous &model,		// Theoretical model
		size_t bins							// Bins count for a histogram
	) : DistComparator (to_vector (py_list), model, bins)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Load a distribution model as a reference for the distribution test    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	template <typename T>
	void ReferenceModel (
		const T &model						// Theoretical model
	){
		// Check if empirical data range is inside the model domain
		if (model.Domain() >= sample.Domain()) {

			// Set the distribution model
			reference = Distribution (model, sample.Values());
			params = model.Parameters();
		}
		else
			throw invalid_argument ("ReferenceModel: The sample data range is outside the distribution model domain");
	}

	// Discrete distribution
	void ReferenceModel (
		const BaseDiscrete &model			// Theoretical model
	){
		ReferenceModel <BaseDiscrete> (model);
	}

	// Continuous distribution
	void ReferenceModel (
		const BaseContinuous &model			// Theoretical model
	){
		ReferenceModel <BaseContinuous> (model);
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
// NOTE:	Bigger confidence levels indicate we should accept
//			the null hypothesis about the distribution type
	double PearsonConfidenceLevel (void) const {

		// Check if the sample and the reference distributions are set
		if (sample.Bins() && reference.Bins()) {

			// Check type of the reference distribution
			Distribution::DistType type = reference.Type();
			if (type == Distribution::THEORETICAL_DISCRETE)
				return PearsonLevel();
			else if (type == Distribution::THEORETICAL_CONTINUOUS)
				throw invalid_argument ("PearsonConfidenceLevel: Only discrete distribution models can be tested");
			else
				throw invalid_argument ("PearsonConfidenceLevel: Can calculate the critical confidence level for a theoretical model only");
		}
		else
			throw invalid_argument ("PearsonConfidenceLevel: Set a sample and a reference for the confidence level");
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
					return PearsonTest (1.0 - level);
				else if (type == Distribution::THEORETICAL_CONTINUOUS)
					throw invalid_argument ("PearsonTest: Only discrete distribution models can be tested");
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
//      Score table (confidence level) for different distribution models      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	static const vector <PearsonScore> ScoreTable (
		const Observations &data			// Observations of a random value
	){
		// Distribution comparator
		DistComparator temp (data);

		// Score table
		vector <PearsonScore> table;

		// Test available distribution models
		temp.TestModel <DiscreteUniform> (table, data, "Discrete Uniform\t= ");
		temp.TestModel <Binomial> (table, data, "Binomial\t\t= ");
		temp.TestModel <NegativeBinomial> (table, data, "NegativeBinomial\t= ");
		temp.TestModelWithRange <Bernoulli> (table, data, "Bernoulli\t\t= ");
		temp.TestModelWithRange <Geometric> (table, data, "Geometric\t\t= ");
		temp.TestModelWithRange <Poisson> (table, data, "Poisson\t\t\t= ");

		// Compare function to sort the scores in descending order
		auto comp = [] (PearsonScore a, PearsonScore b) {
			return a.score > b.score;
		};

		// Rank the scores
		sort (table.begin(), table.end(), comp);
		return table;
	}
};

//****************************************************************************//
//      Translate the objects to a string                                     //
//****************************************************************************//
const string pearson_score_to_string (const vector <PearsonScore> &table)
{
	stringstream stream;
	stream.precision (PRECISION);
	stream << "PEARSON CHI-SQUARED SCORE TABLE:" << std::endl;
	stream << "===============================================" << std::endl;
	stream << "Distribution name\tScore value (%)" << std::endl;
	stream << "~~~~~~~~~~~~~~~~~~~~~~~\t~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
	for (const auto &item : table)
		stream << item.name << fixed << setprecision (3) << item.score * 100 <<endl;
	return stream.str();
}
ostream& operator << (ostream &stream, const DistComparator &object)
{
	auto restore = stream.precision();
	stream.precision (PRECISION);
	stream << "\nDISTRIBUTION COMPARATOR:" << endl;
	stream << "========================" << endl;
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
