/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                  COMPARE TWO CDF FUNCTIONS WITH ONE ANOTHER                  #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	<cmath>
# include	"raw.hpp"
# include	"../models/discrete/uniform.hpp"
# include	"../models/discrete/binomial.hpp"
# include	"../models/discrete/negative_binomial.hpp"
# include	"../models/discrete/bernoulli.hpp"
# include	"../models/discrete/geometric.hpp"
# include	"../models/discrete/poisson.hpp"
# include	"../models/continuous/chi_squared.hpp"
# include	"../models/continuous/kolmogorov.hpp"
# include	"../models/continuous/uniform.hpp"
# include	"../models/continuous/rayleigh.hpp"
# include	"../models/continuous/exponential.hpp"
# include	"../models/continuous/erlang.hpp"
# include	"../models/continuous/chi_squared.hpp"
# include	"../models/continuous/gamma.hpp"
# include	"../models/continuous/pareto.hpp"
# include	"../models/continuous/beta.hpp"
# include	"../models/continuous/logistic.hpp"
# include	"../models/continuous/normal.hpp"
# include	"../models/continuous/laplace.hpp"
# include	"../models/continuous/asymmetric_laplace.hpp"

//****************************************************************************//
//      Class "KolmogorovScore"                                               //
//****************************************************************************//
struct KolmogorovScore
{
	string name;							// Distribution model name
	double score;							// Score points

	// Check two instances for equality (required by the vector template)
	bool operator== (const KolmogorovScore &obj) const {
		return (name == obj.name && score == obj.score);
	}
};

//****************************************************************************//
//      Class "CDF"                                                           //
//****************************************************************************//
class CDF
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	RawCDF sample;							// Sample CDF to compare
	RawCDF reference;						// Reference CDF

//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Perform a test of a distribution model                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	template <typename T>
	void TestModel (
		vector <KolmogorovScore> &table,	// Score table
		const Observations &data,			// Observations of a random value
		const string name					// Distribution model name
	)
	try {
		// Set the distribution model
		ReferenceModel (T (data));

		// Try to estimate the confidence level of the one-sample Kolmogorov-Smirnov test
		const double level = KolmogorovConfidenceLevel();
		if (!isnan (level))
			table.push_back (KolmogorovScore {name, level});

	} catch (const invalid_argument &exception) {}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Perform a test of a distribution model with the range validation      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	template <typename T>
	void TestModelWithRange (
		vector <KolmogorovScore> &table,	// Score table
		const Observations &data,			// Observations of a random value
		const string name					// Distribution model name
	)
	try {
		if (T::InDomain (data.Domain()))
			TestModel <T> (table, data, name);
	} catch (const invalid_argument &exception) {}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Compute the value of the one-sample Kolmogorov-Smirnov test           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double KolmogorovCriteria1 (void) const {

		// Get both CDF functions
		const vector <double> &src = sample.CDF();
		const vector <double> &ref = reference.CDF();

		// Find the supremum between two CDFs
		double max_diff = 0.0;
		const size_t size = src.size();
		for (size_t i = 0; i < size; i++) {
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
		const size_t size1 = sample.Size();
		const size_t size2 = reference.Size();
		return sqrt (size1 * size2 / double (size1 + size2)) * max_diff;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Compute the confidence level of the one-sample Kolmogorov-Smirnov test//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double KolmogorovLevel (void) const {

		// Compute the value of the one-sample Kolmogorov-Smirnov test
		const double criteria = KolmogorovCriteria1();

		// Return the confidence level
		const Kolmogorov &dist = Kolmogorov();
		return 1.0 - dist.CDF (criteria);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      One-sample Kolmogorov-Smirnov test                                    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	bool OneSampleTest (
		double alpha				// Rejection level of the null hypothesis
	) const {

		// Compute the value of the one-sample Kolmogorov-Smirnov test
		const double criteria = KolmogorovCriteria1 ();

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
	CDF (
		const Observations &data			// Observations of a random value
	) : sample (data)
	{}

	CDF (
		const vector <double> &data			// Empirical data
	) : sample (move (vector <double> (data)))
	{}

	CDF (
		const pylist &py_list				// Empirical data
	) : CDF (to_vector (py_list))
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors from empirical data and a theoretical model              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

	// Discrete distribution
	CDF (
		const Observations &data,			// Observations of a random value
		const BaseDiscrete &model			// Theoretical model
	) : CDF (data)
	{
		ReferenceModel (model);
	}

	// Discrete distribution
	CDF (
		const vector <double> &data,		// Empirical data
		const BaseDiscrete &model			// Theoretical model
	) : CDF (data)
	{
		ReferenceModel (model);
	}

	// Discrete distribution
	CDF (
		const pylist &py_list,				// Empirical data
		const BaseDiscrete &model			// Theoretical model
	) : CDF (to_vector (py_list), model)
	{}

	// Continuous distribution
	CDF (
		const Observations &data,			// Observations of a random value
		const BaseContinuous &model			// Theoretical model
	) : CDF (data)
	{
		ReferenceModel (model);
	}

	// Continuous distribution
	CDF (
		const vector <double> &data,		// Empirical data
		const BaseContinuous &model			// Theoretical model
	) : CDF (data)
	{
		ReferenceModel (model);
	}

	// Continuous distribution
	CDF (
		const pylist &py_list,				// Empirical data
		const BaseContinuous &model			// Theoretical model
	) : CDF (to_vector (py_list), model)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors from empirical data only (a sample and a reference)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	CDF (
		const Observations &sample,			// Sample observations
		const Observations &reference		// Reference observations
	) : CDF (sample)
	{
		ReferenceSample (reference);
	}

	CDF (
		const vector <double> &sample,		// Empirical sample data
		const vector <double> &reference	// Empirical reference data
	) : CDF (sample)
	{
		ReferenceSample (reference);
	}

	CDF (
		const pylist &sample,				// Empirical sample data
		const pylist &reference				// Empirical reference data
	) : CDF (to_vector (sample), to_vector (reference))
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Load a CDF model as a reference for the distribution test             //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void ReferenceModel (
		const BaseModel &model				// Theoretical model
	){
		// Check if empirical data range is inside the model domain
		if (model.Domain() >= sample.Domain()) {

			// Set the CDF model
			reference = RawCDF (model, sample.Values());
		}
		else
			throw invalid_argument ("ReferenceModel: The sample data range is outside the distribution model domain");
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Load another sample as a reference for the distribution test          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void ReferenceSample (
		const Observations &data			// Observations of a random value
	){
		reference = RawCDF (data);
	}

	void ReferenceSample (
		const vector <double> &data			// Empirical data
	){
		reference = RawCDF (move (vector <double> (data)));
	}

	void ReferenceSample (
		const pylist &py_list				// Empirical data
	){
		ReferenceSample (to_vector (py_list));
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Return sample CDF function                                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const RawCDF& Sample (void) const {
		return sample;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Return reference CDF function                                         //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const RawCDF& Reference (void) const {
		return reference;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Confidence level of the one-sample Kolmogorov-Smirnov test            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// NOTE:	Bigger confidence levels indicate we should accept
//			the null hypothesis about the distribution type
	double KolmogorovConfidenceLevel (void) const {

		// Check if the sample and the reference distributions are set
		if (sample.Size() && reference.Size()) {

			// Check type of the reference distribution
			if (reference.Type() == RawCDF::THEORETICAL)
				return KolmogorovLevel();
			else
				throw invalid_argument ("KolmogorovConfidenceLevel: Can calculate the critical confidence level for a theoretical model only");
		}
		else
			throw invalid_argument ("KolmogorovConfidenceLevel: Set a sample and a reference for the confidence level");
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
			if (sample.Size() && reference.Size()) {

				// Check type of the reference distribution
				if (reference.Type() == RawCDF::THEORETICAL)
					return OneSampleTest (1.0 - level);
				else
					return TwoSampleTest (1.0 - level);
			}
			else
				throw invalid_argument ("KolmogorovSmirnovTest: Set a sample and a reference for the test");
		}
		else
			throw invalid_argument ("KolmogorovSmirnovTest: The confidence level must be in the range [0..1]");
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Score table (confidence level) for different distribution models      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	static const vector <KolmogorovScore> ScoreTable (
		const Observations &data			// Observations of a random value
	){
		// Distribution comparator
		CDF temp (data);

		// Score table
		vector <KolmogorovScore> table;

		// Test available discrete distribution models
		temp.TestModel <DiscreteUniform> (table, data, "Discrete Uniform\t= ");
		temp.TestModel <Binomial> (table, data, "Binomial\t\t= ");
		temp.TestModel <NegativeBinomial> (table, data, "NegativeBinomial\t= ");
		temp.TestModelWithRange <Bernoulli> (table, data, "Bernoulli\t\t= ");
		temp.TestModelWithRange <Geometric> (table, data, "Geometric\t\t= ");
		temp.TestModelWithRange <Poisson> (table, data, "Poisson\t\t\t= ");

		// Test available continuous distribution models
		temp.TestModel <ContinuousUniform> (table, data, "Continuous Uniform\t= ");
		temp.TestModel <Pareto> (table, data, "Pareto\t\t\t= ");
		temp.TestModelWithRange <Rayleigh> (table, data, "Rayleigh\t\t= ");
		temp.TestModelWithRange <Exponential> (table, data, "Exponential\t\t= ");
		temp.TestModelWithRange <Erlang> (table, data, "Erlang\t\t\t= ");
		temp.TestModelWithRange <ChiSquared> (table, data, "Chi-squared\t\t= ");
		temp.TestModelWithRange <Gamma> (table, data, "Gamma\t\t\t= ");
		temp.TestModelWithRange <Beta> (table, data, "Beta\t\t\t= ");
		temp.TestModelWithRange <Logistic> (table, data, "Logistic\t\t= ");
		temp.TestModelWithRange <Normal> (table, data, "Normal\t\t\t= ");
		temp.TestModelWithRange <Laplace> (table, data, "Laplace\t\t\t= ");
		temp.TestModelWithRange <AsymmetricLaplace> (table, data, "Asymmetric Laplace\t= ");

		// Compare function to sort the scores in descending order
		auto comp = [] (KolmogorovScore a, KolmogorovScore b) {
			return a.score > b.score;
		};

		// Rank the scores
		sort (table.begin(), table.end(), comp);
		return table;
	}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
const string kolmogorov_score_to_string (const vector <KolmogorovScore> &table)
{
	stringstream stream;
	stream.precision (PRECISION);
	stream << "\nKOLMOGOROV SCORE TABLE:" << std::endl;
	stream << "===============================================" << std::endl;
	stream << "Distribution name\tScore value (%)" << std::endl;
	stream << "~~~~~~~~~~~~~~~~~~~~~~~\t~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
	for (const auto &item : table)
		stream << item.name << fixed << setprecision (3) << item.score * 100 <<endl;
	return stream.str();
}
ostream& operator << (ostream &stream, const CDF &object)
{
	auto restore = stream.precision();
	stream.precision (PRECISION);
	stream << "\nCDF COMPARATOR:" << endl;
	stream << "===============" << endl;
	stream << "Sample CDF values\t\t\t= " << object.Sample().Size() << endl;
	stream << "Reference CDF values\t\t\t= " << object.Reference().Size() << endl;
	stream.precision (restore);
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
