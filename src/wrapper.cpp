/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#              THE WRAPPER CODE TO EXPOSE C++ FUNCTIONS TO PYTHON              #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# include	"gui.hpp"

//****************************************************************************//
//      Python module initialization functions                                //
//****************************************************************************//
BOOST_PYTHON_MODULE (gui) {

	// Use shortenings
	using namespace boost::python;

//============================================================================//
//      Expose "Show" functions for the "Range" object                        //
//============================================================================//
	void (*RangeShow1)(const Model::Range &range)						= &Show;
	void (*RangeShow2)(const string &name, const Model::Range &range)	= &Show;
	def ("show", RangeShow1, "Show a GUI window with a range summary");
	def ("show", RangeShow2, "Show a GUI window with a range summary and a custom title");

//============================================================================//
//      Expose "Show" functions for the "ConfidenceInterval" object           //
//============================================================================//
	void (*CIShow1)(const Model::ConfidenceInterval &range)						= &Show;
	void (*CIShow2)(const string &name, const Model::ConfidenceInterval &range)	= &Show;
	def ("show", CIShow1, "Show a GUI window with a confidence interval summary");
	def ("show", CIShow2, "Show a GUI window with a confidence interval summary and a custom title");

//============================================================================//
//      Expose "Show" functions for the "DiscreteUniform" object              //
//============================================================================//
	void (*DiscreteUniformShow1)(const Model::DiscreteUniform &model)						= &Show;
	void (*DiscreteUniformShow2)(const string &name, const Model::DiscreteUniform &model)	= &Show;
	def ("show", DiscreteUniformShow1, "Show a GUI window with a summary for the Discrete Uniform distribution");
	def ("show", DiscreteUniformShow2, "Show a GUI window with a summary for the Discrete Uniform distribution and a custom title");

//============================================================================//
//      Expose "Show" functions for the "Bernoulli" object                    //
//============================================================================//
	void (*BernoulliShow1)(const Model::Bernoulli &model)						= &Show;
	void (*BernoulliShow2)(const string &name, const Model::Bernoulli &model)	= &Show;
	def ("show", BernoulliShow1, "Show a GUI window with a summary for the Bernoulli distribution");
	def ("show", BernoulliShow2, "Show a GUI window with a summary for the Bernoulli distribution and a custom title");

//============================================================================//
//      Expose "Show" functions for the "Geometric" object                    //
//============================================================================//
	void (*GeometricShow1)(const Model::Geometric &model)						= &Show;
	void (*GeometricShow2)(const string &name, const Model::Geometric &model)	= &Show;
	def ("show", GeometricShow1, "Show a GUI window with a summary for the Geometric distribution");
	def ("show", GeometricShow2, "Show a GUI window with a summary for the Geometric distribution and a custom title");

//============================================================================//
//      Expose "Show" functions for the "Binomial" object                     //
//============================================================================//
	void (*BinomialShow1)(const Model::Binomial &model)						= &Show;
	void (*BinomialShow2)(const string &name, const Model::Binomial &model)	= &Show;
	def ("show", BinomialShow1, "Show a GUI window with a summary for the Binomial distribution");
	def ("show", BinomialShow2, "Show a GUI window with a summary for the Binomial distribution and a custom title");

//============================================================================//
//      Expose "Show" functions for the "NegativeBinomial" object             //
//============================================================================//
	void (*NegativeBinomialShow1)(const Model::NegativeBinomial &model)						= &Show;
	void (*NegativeBinomialShow2)(const string &name, const Model::NegativeBinomial &model)	= &Show;
	def ("show", NegativeBinomialShow1, "Show a GUI window with a summary for the Negative Binomial distribution");
	def ("show", NegativeBinomialShow2, "Show a GUI window with a summary for the Negative Binomial distribution and a custom title");

//============================================================================//
//      Expose "Show" functions for the "Poisson" object                      //
//============================================================================//
	void (*PoissonShow1)(const Model::Poisson &model)						= &Show;
	void (*PoissonShow2)(const string &name, const Model::Poisson &model)	= &Show;
	def ("show", PoissonShow1, "Show a GUI window with a summary for the Poisson distribution");
	def ("show", PoissonShow2, "Show a GUI window with a summary for the Poisson distribution and a custom title");

//============================================================================//
//      Expose "Show" functions for the "Kolmogorov" object                   //
//============================================================================//
	void (*KolmogorovShow1)(const Model::Kolmogorov &model)						= &Show;
	void (*KolmogorovShow2)(const string &name, const Model::Kolmogorov &model)	= &Show;
	def ("show", KolmogorovShow1, "Show a GUI window with a summary for the Kolmogorov distribution");
	def ("show", KolmogorovShow2, "Show a GUI window with a summary for the Kolmogorov distribution and a custom title");

//============================================================================//
//      Expose "Show" functions for the "ContinuousUniform" object            //
//============================================================================//
	void (*ContinuousUniformShow1)(const Model::ContinuousUniform &model)						= &Show;
	void (*ContinuousUniformShow2)(const string &name, const Model::ContinuousUniform &model)	= &Show;
	def ("show", ContinuousUniformShow1, "Show a GUI window with a summary for the Continuous Uniform distribution");
	def ("show", ContinuousUniformShow2, "Show a GUI window with a summary for the Continuous Uniform distribution and a custom title");

//============================================================================//
//      Expose "Show" functions for the "Rayleigh" object                     //
//============================================================================//
	void (*RayleighShow1)(const Model::Rayleigh &model)						= &Show;
	void (*RayleighShow2)(const string &name, const Model::Rayleigh &model)	= &Show;
	def ("show", RayleighShow1, "Show a GUI window with a summary for the Rayleigh distribution");
	def ("show", RayleighShow2, "Show a GUI window with a summary for the Rayleigh distribution and a custom title");

//============================================================================//
//      Expose "Show" functions for the "Exponential" object                  //
//============================================================================//
	void (*ExponentialShow1)(const Model::Exponential &model)						= &Show;
	void (*ExponentialShow2)(const string &name, const Model::Exponential &model)	= &Show;
	def ("show", ExponentialShow1, "Show a GUI window with a summary for the Exponential distribution");
	def ("show", ExponentialShow2, "Show a GUI window with a summary for the Exponential distribution and a custom title");

//============================================================================//
//      Expose "Show" functions for the "Erlang" object                       //
//============================================================================//
	void (*ErlangShow1)(const Model::Erlang &model)						= &Show;
	void (*ErlangShow2)(const string &name, const Model::Erlang &model)	= &Show;
	def ("show", ErlangShow1, "Show a GUI window with a summary for the Erlang distribution");
	def ("show", ErlangShow2, "Show a GUI window with a summary for the Erlang distribution and a custom title");

//============================================================================//
//      Expose "Show" functions for the "ChiSquared" object                   //
//============================================================================//
	void (*ChiSquaredShow1)(const Model::ChiSquared &model)						= &Show;
	void (*ChiSquaredShow2)(const string &name, const Model::ChiSquared &model)	= &Show;
	def ("show", ChiSquaredShow1, "Show a GUI window with a summary for the Chi-squared distribution");
	def ("show", ChiSquaredShow2, "Show a GUI window with a summary for the Chi-squared distribution and a custom title");

//============================================================================//
//      Expose "Show" functions for the "Gamma" object                        //
//============================================================================//
	void (*GammaShow1)(const Model::Gamma &model)						= &Show;
	void (*GammaShow2)(const string &name, const Model::Gamma &model)	= &Show;
	def ("show", GammaShow1, "Show a GUI window with a summary for the Gamma distribution");
	def ("show", GammaShow2, "Show a GUI window with a summary for the Gamma distribution and a custom title");

//============================================================================//
//      Expose "Show" functions for the "Pareto" object                       //
//============================================================================//
	void (*ParetoShow1)(const Model::Pareto &model)						= &Show;
	void (*ParetoShow2)(const string &name, const Model::Pareto &model)	= &Show;
	def ("show", ParetoShow1, "Show a GUI window with a summary for the Pareto distribution");
	def ("show", ParetoShow2, "Show a GUI window with a summary for the Pareto distribution and a custom title");

//============================================================================//
//      Expose "Show" functions for the "Beta" object                         //
//============================================================================//
	void (*BetaShow1)(const Model::Beta &model)						= &Show;
	void (*BetaShow2)(const string &name, const Model::Beta &model)	= &Show;
	def ("show", BetaShow1, "Show a GUI window with a summary for the Beta distribution");
	def ("show", BetaShow2, "Show a GUI window with a summary for the Beta distribution and a custom title");

//============================================================================//
//      Expose "Show" functions for the "F" object                            //
//============================================================================//
	void (*FShow1)(const Model::F &model)						= &Show;
	void (*FShow2)(const string &name, const Model::F &model)	= &Show;
	def ("show", FShow1, "Show a GUI window with a summary for the Snedecor's f-distribution");
	def ("show", FShow2, "Show a GUI window with a summary for the Snedecor's f-distribution and a custom title");

//============================================================================//
//      Expose "Show" functions for the "StandardT" object                    //
//============================================================================//
	void (*StandardTShow1)(const Model::StandardT &model)						= &Show;
	void (*StandardTShow2)(const string &name, const Model::StandardT &model)	= &Show;
	def ("show", StandardTShow1, "Show a GUI window with a summary for the Student’s t-distribution");
	def ("show", StandardTShow2, "Show a GUI window with a summary for the Student’s t-distribution and a custom title");

//============================================================================//
//      Expose "Show" functions for the "Logistic" object                     //
//============================================================================//
	void (*LogisticShow1)(const Model::Logistic &model)						= &Show;
	void (*LogisticShow2)(const string &name, const Model::Logistic &model)	= &Show;
	def ("show", LogisticShow1, "Show a GUI window with a summary for the Logistic distribution");
	def ("show", LogisticShow2, "Show a GUI window with a summary for the Logistic distribution and a custom title");

//============================================================================//
//      Expose "Show" functions for the "Normal" object                       //
//============================================================================//
	void (*NormalShow1)(const Model::Normal &model)						= &Show;
	void (*NormalShow2)(const string &name, const Model::Normal &model)	= &Show;
	def ("show", NormalShow1, "Show a GUI window with a summary for the Normal distribution");
	def ("show", NormalShow2, "Show a GUI window with a summary for the Normal distribution and a custom title");

//============================================================================//
//      Expose "Show" functions for the "Laplace" object                      //
//============================================================================//
	void (*LaplaceShow1)(const Model::Laplace &model)						= &Show;
	void (*LaplaceShow2)(const string &name, const Model::Laplace &model)	= &Show;
	def ("show", LaplaceShow1, "Show a GUI window with a summary for the Laplace distribution");
	def ("show", LaplaceShow2, "Show a GUI window with a summary for the Laplace distribution and a custom title");

//============================================================================//
//      Expose "Show" functions for the "AsymmetricLaplace" object            //
//============================================================================//
	void (*AsymmetricLaplaceShow1)(const Model::AsymmetricLaplace &model)						= &Show;
	void (*AsymmetricLaplaceShow2)(const string &name, const Model::AsymmetricLaplace &model)	= &Show;
	def ("show", AsymmetricLaplaceShow1, "Show a GUI window with a summary for the Asymmetric Laplace distribution");
	def ("show", AsymmetricLaplaceShow2, "Show a GUI window with a summary for the Asymmetric Laplace distribution and a custom title");

//============================================================================//
//      Expose "Show" functions for the "Population" object                   //
//============================================================================//
	void (*PopulationShow1)(const Population &population)						= &Show;
	void (*PopulationShow2)(const string &name, const Population &population)	= &Show;
	def ("show", PopulationShow1, "Show a GUI window with a population summary");
	def ("show", PopulationShow2, "Show a GUI window with a population summary and a custom title");

//============================================================================//
//      Expose "Show" functions for the "Sample" object                       //
//============================================================================//
	void (*SampleShow1)(const Sample &sample)						= &Show;
	void (*SampleShow2)(const string &name, const Sample &sample)	= &Show;
	def ("show", SampleShow1, "Show a GUI window with a sample summary");
	def ("show", SampleShow2, "Show a GUI window with a sample summary and a custom title");
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
