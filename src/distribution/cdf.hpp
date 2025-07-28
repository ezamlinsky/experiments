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
# include	"base.hpp"
# include	"../models/continuous/kolmogorov.hpp"
# include	"../models/discrete/uniform.hpp"
# include	"../models/continuous/kolmogorov.hpp"
# include	"../models/continuous/beta.hpp"
# include	"../models/continuous/erlang.hpp"
# include	"../models/continuous/chi_squared.hpp"
# include	"../models/continuous/exponential.hpp"
# include	"../models/continuous/normal.hpp"
# include	"../models/continuous/laplace.hpp"
# include	"../models/continuous/asymmetric_laplace.hpp"

// Use shortenings
using namespace std;
using ModelTest = pair <string, double>;

//****************************************************************************//
//      Class "CDF"                                                           //
//****************************************************************************//
class CDF : public BaseComparator
{
//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Perform a test of a continuous distribution model                     //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	template <typename T>
	void TestModel (
		vector <ModelTest> &table,			// Score table
		const Observations &data,			// Observations of a random value
		const string name					// Distribution model name
	)
	try {
		if (T::InDomain (data.Domain())) {
			T model (data);
			ReferenceModel (model);
			table.push_back (ModelTest (name, KolmogorovConfidenceLevel()));
		}
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
		const size_t size1 = sample.Bins();
		const size_t size2 = reference.Bins();
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
	) : BaseComparator (data)
	{}

	CDF (
		const vector <double> &data			// Empirical data
	) : BaseComparator (data)
	{}

	CDF (
		const list &py_list					// Empirical data
	) : CDF (to_vector (py_list))
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors from empirical data and a theoretical model              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	CDF (
		const Observations &data,			// Observations of a random value
		const BaseModel &model				// Theoretical model
	) : CDF (data)
	{
		ReferenceModel (model);
	}

	CDF (
		const vector <double> &data,		// Empirical data
		const BaseModel &model				// Theoretical model
	) : CDF (data)
	{
		ReferenceModel (model);
	}

	CDF (
		const list &py_list,				// Empirical data
		const BaseModel &model				// Theoretical model
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
		const list &sample,					// Empirical sample data
		const list &reference				// Empirical reference data
	) : CDF (to_vector (sample), to_vector (reference))
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Load another sample as a reference for the distribution test          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void ReferenceSample (
		const Observations &data			// Observations of a random value
	){
		reference = Distribution (data);
	}

	void ReferenceSample (
		const vector <double> &data			// Empirical data
	){
		// Check type of the sample distribution
		reference = Distribution (data);
	}

	void ReferenceSample (
		const list &py_list					// Empirical data
	){
		ReferenceSample (to_vector (py_list));
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Confidence level of the one-sample Kolmogorov-Smirnov test            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// NOTE: Bigger confidence levels indicate we should accept the null hypothesis
// about the distribution type
	double KolmogorovConfidenceLevel (void) const {

		// Check if the sample and the reference distributions are set
		if (sample.Bins() && reference.Bins()) {

			// Check type of the reference distribution
			Distribution::DistType type = reference.Type();
			if (type == Distribution::THEORETICAL_DISCRETE || type == Distribution::THEORETICAL_CONTINUOUS)
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
			if (sample.Bins() && reference.Bins()) {

				// Check type of the reference distribution
				Distribution::DistType type = reference.Type();
				if (type == Distribution::THEORETICAL_DISCRETE || type == Distribution::THEORETICAL_CONTINUOUS)
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
	static const vector <ModelTest> ScoreTable (
		const Observations &data			// Observations of a random value
	){
		CDF temp (data);

		// Score table
		vector <ModelTest> table;

		// Test available distribution models
		temp.TestModel <Beta> (table, data, "Beta\t\t\t= ");
		temp.TestModel <Erlang> (table, data, "Erlang\t\t\t= ");
		temp.TestModel <ChiSquared> (table, data, "Chi-squared\t\t= ");
		temp.TestModel <Exponential> (table, data, "Exponential\t\t= ");
		temp.TestModel <Normal> (table, data, "Normal\t\t\t= ");
		temp.TestModel <Laplace> (table, data, "Laplace\t\t\t= ");
		temp.TestModel <AsymmetricLaplace> (table, data, "Asymmetric Laplace\t= ");

		// Compare function to sort the scores in descending order
		auto comp = [] (ModelTest a, ModelTest b) {
			return a.second > b.second;
		};

		// Rank the scores
		sort (table.begin(), table.end(), comp);
		return table;
	}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
string vector_to_string (const vector <ModelTest> &table)
{
	stringstream stream;
	stream.precision (PRECISION);
	stream << "\nKOLMOGOROV SCORE TABLE:" << std::endl;
	stream << "===============================================" << std::endl;
	stream << "Distribution name\tScore value (%)" << std::endl;
	stream << "~~~~~~~~~~~~~~~~~~~~~~~\t~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
	for (const auto &item : table)
		stream << item.first << fixed << setprecision (3) << item.second * 100 <<endl;
	return stream.str();
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
