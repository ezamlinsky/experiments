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
# include	"base.hpp"
# include	"../models/discrete/uniform.hpp"
# include	"../models/continuous/beta.hpp"
# include	"../models/continuous/erlang.hpp"
# include	"../models/continuous/chi_squared.hpp"
# include	"../models/continuous/exponential.hpp"
# include	"../models/continuous/normal.hpp"
# include	"../models/continuous/laplace.hpp"
# include	"../models/continuous/asymmetric_laplace.hpp"

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
class DistComparator : public BaseComparator
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	size_t observations;					// Number of collected observations
	size_t params;							// Number of model parameters

//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Perform a test of a continuous distribution model                     //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	template <typename T>
	void TestModel (
		vector <PearsonScore> &table,		// Score table
		const Observations &data,			// Observations of a random value
		const string name					// Distribution model name
	)
	try {
		if (T::InDomain (data.Domain())) {
			T model (data);
			ReferenceModel (model);
			table.push_back (PearsonScore {name, PearsonConfidenceLevel()});
		}
	} catch (const invalid_argument &exception) {}

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
	) : BaseComparator (data),
		observations (data.Size())
	{}

	// Discrete distribution
	DistComparator (
		const vector <double> &data			// Empirical data
	) : BaseComparator (data),
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
	) : BaseComparator (data, bins),
		observations (data.Size())
	{}

	// Continuous distribution
	DistComparator (
		const vector <double> &data,		// Empirical data
		size_t bins							// Bins count for a histogram
	) : BaseComparator (data, bins),
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
	) : DistComparator (data)
	{
		ReferenceModel (model);
	}

	// Discrete distribution
	DistComparator (
		const vector <double> &data,		// Empirical data
		const BaseModel &model				// Theoretical model
	) : DistComparator (data)
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
	) : DistComparator (data, bins)
	{
		ReferenceModel (model);
	}

	// Continuous distribution
	DistComparator (
		const vector <double> &data,		// Empirical data
		const BaseModel &model,				// Theoretical model
		size_t bins							// Bins count for a histogram
	) : DistComparator (data, bins)
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
//      Load a distribution model as a reference for the distribution test    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void ReferenceModel (
		const BaseModel &model				// Theoretical model
	){
		BaseComparator::ReferenceModel (model);
		params = model.Parameters();
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
//      Score table (confidence level) for different distribution models      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	static const vector <PearsonScore> ScoreTable (
		const Observations &data,			// Observations of a random value
		size_t bins							// Bins count for a histogram
	){
		// Distribution comparator
		DistComparator temp (data, bins);

		// Score table
		vector <PearsonScore> table;

		// Test available distribution models
		temp.TestModel <Beta> (table, data, "Beta\t\t\t= ");
		temp.TestModel <Erlang> (table, data, "Erlang\t\t\t= ");
		temp.TestModel <ChiSquared> (table, data, "Chi-squared\t\t= ");
		temp.TestModel <Exponential> (table, data, "Exponential\t\t= ");
		temp.TestModel <Normal> (table, data, "Normal\t\t\t= ");
		temp.TestModel <Laplace> (table, data, "Laplace\t\t\t= ");
		temp.TestModel <AsymmetricLaplace> (table, data, "Asymmetric Laplace\t= ");

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
//      Translate the object to a string                                      //
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
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
