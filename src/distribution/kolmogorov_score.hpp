/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#             RANK RESULTS OF ONE-SAMPLE KOLMOGOROV-SMIRNOV TESTS              #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	<string>
# include	"cdfs.hpp"
# include	"../models/beta.hpp"
# include	"../models/exponential.hpp"
# include	"../models/erlang.hpp"
# include	"../models/chi_squared.hpp"
# include	"../models/normal.hpp"
# include	"../models/laplace.hpp"
# include	"../models/asymmetric_laplace.hpp"

//****************************************************************************//
//      Run a test for a distribution model                                   //
//****************************************************************************//
# define	TEST_MODEL(func, model, name)										\
	void func (																	\
		const Observations &data												\
	){																			\
		if (model::InDomain (data.Domain())) {									\
			model model (data);													\
			cdf.ReferenceModel (model);											\
			table.push_back (ModelTest (name, cdf.ConfidenceLevel()));			\
		}																		\
	}

// Use shortenings
using namespace std;
using ModelTest = pair <string, double>;

//****************************************************************************//
//      Class "KolmogorovScore"                                               //
//****************************************************************************//
class KolmogorovScore
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	CDFs cdf;						// Comparator for CDF functions
	vector <ModelTest> table;		// Score table

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Tests of different distributions                                      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	TEST_MODEL(TestBeta,				Beta,				"Beta\t\t\t= ")
	TEST_MODEL(TestExponential,			Exponential,		"Exponential\t\t= ")
	TEST_MODEL(TestErlang,				Erlang,				"Erlang\t\t\t= ")
	TEST_MODEL(TestChiSquared,			ChiSquared,			"Chi-squared\t\t= ")
	TEST_MODEL(TestNormal,				Normal,				"Normal\t\t\t= ")
	TEST_MODEL(TestLaplace,				Laplace,			"Laplace\t\t\t= ")
	TEST_MODEL(TestAsymmetricLaplace,	AsymmetricLaplace,	"Asymmetric Laplace\t= ")

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	KolmogorovScore (
		const Observations &data	// Empirical observations
	) :	cdf (data)
	{
		// Test available distribution models
		TestBeta (data);
		TestExponential (data);
		TestErlang (data);
		TestChiSquared (data);
		TestNormal (data);
		TestLaplace (data);
		TestAsymmetricLaplace (data);

		// Compare function to sort the scores in descending order
		auto comp = [] (ModelTest a, ModelTest b) {
			return a.second > b.second;
		};

		// Rank the scores
		sort (table.begin(), table.end(), comp);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Score table (confidence level) for different models                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const vector <ModelTest>& ScoreTable (void) const {
		return table;
	}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const KolmogorovScore &object)
{
	auto restore = stream.precision();
	stream.precision (PRECISION);
	stream << "\nKOLMOGOROV SCORE TABLE:" << std::endl;
	stream << "===============================================" << std::endl;
	stream << "Distribution name\tScore value (%)" << std::endl;
	stream << "~~~~~~~~~~~~~~~~~~~~~~~\t~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
	for (const auto &item : object.ScoreTable())
		stream << item.first << fixed << setprecision (3) << item.second * 100 <<endl;
	stream.precision (restore);
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
