/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#               THE WRAPPER CODE TO EXPOSE C++ CLASSES TO PYTHON               #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# include	<boost/python.hpp>
# include	<boost/python/suite/indexing/vector_indexing_suite.hpp>
# include	"confidence_interval.hpp"
# include	"discrete/uniform.hpp"
# include	"continuous/kolmogorov.hpp"
# include	"continuous/standard_t.hpp"
# include	"continuous/fisher_snedecor.hpp"
# include	"continuous/beta.hpp"
# include	"continuous/erlang.hpp"
# include	"continuous/chi_squared.hpp"
# include	"continuous/exponential.hpp"
# include	"continuous/normal.hpp"
# include	"continuous/laplace.hpp"
# include	"continuous/asymmetric_laplace.hpp"

//****************************************************************************//
//      Methods are inherited from the base distribution class                //
//****************************************************************************//
# define	BASE_CLASS_METHODS(class) 											\
	.def ("Domain",		&class::Domain,		return_internal_reference <> (),	\
		"Function domain where the distribution exists")						\
	.def ("Generate",	&class::Generate,	args ("count"),						\
		"Generate random values from the distribution")							\
	.def ("Quantile",	&class::Quantile,	args ("level"),						\
		"Quantile value for the target level")									\
	.def ("PDF",		&class::PDF,		args ("x"),							\
		"Probability Density Function (PDF)")									\
	.def ("CDF",		&class::CDF,		args ("x"),							\
		"Cumulative Distribution Function (CDF)")								\
	.def ("data",		&class::data,		return_internal_reference <> (),	\
		"Extract internal data from the distribution")							\
	.def (self_ns::str (self_ns::self))

//****************************************************************************//
//      Properties are inherited from the base distribution class             //
//****************************************************************************//
# define	BASE_CLASS_PROPERTIES(class) 										\
	.add_property ("Mean",					&class::Mean,						\
		"Mean of the distribution")												\
	.add_property ("Variance",				&class::Variance,					\
		"Variance of the distribution")											\
	.add_property ("StdDev",				&class::StdDev,						\
		"Standard deviation of the distribution")								\
	.add_property ("Median",				&class::Median,						\
		"Median of the distribution")											\
	.add_property ("LowerQuartile",			&class::LowerQuartile,				\
		"Lower quartile of the distribution")									\
	.add_property ("UpperQuartile",			&class::UpperQuartile,				\
		"Upper quartile of the distribution")									\
	.add_property ("InterQuartileRange",	&class::InterQuartileRange,			\
		"Inter-quartile range (IQR) of the distribution")						\
	.add_property ("MidHinge",				&class::MidHinge,					\
		"Mid-hinge of the distribution")										\
	.add_property ("TriMean",				&class::TriMean,					\
		"Tukey's tri-mean of the distribution")									\
	.add_property ("QuartileSkewness",		&class::QuartileSkewness,			\
		"Quartile skewness of the distribution")

//****************************************************************************//
//      Properties are inherited from the continuous distribution class       //
//****************************************************************************//
# define	CONTINUOUS_CLASS_PROPERTIES(class) 									\
	BASE_CLASS_PROPERTIES(class)												\
	.add_property ("Location",	&class::Location,								\
		"Location of the distribution")											\
	.add_property ("Scale",		&class::Scale,									\
		"Scale of the distribution")

//****************************************************************************//
//      Python module initialization functions                                //
//****************************************************************************//
BOOST_PYTHON_MODULE (models) {

	// Use shortenings
	using namespace boost::python;

//============================================================================//
//      Expose vector <double> to Python                                      //
//============================================================================//
	class_ <vector <double> > ("DoubleVector")
		.def (vector_indexing_suite <vector <double> > ());

//============================================================================//
//      Expose "Range" class to Python                                        //
//============================================================================//
	class_ <Range> ("Range",
		"Contain a valid range for a numeric value",
		init <double, double> (args ("min", "max"),
			"Define range min and max values"))

		// Methods
		.def ("Clamp",	&Range::Clamp,	args ("value"),
			"Clamp the given value to be within the range [min..max]")
		.def ("Split",	&Range::Split,	args ("bins"),
			"Split the range into subranges (bins)")
		.def (self_ns::str (self_ns::self))

		// Properties
		.add_property ("Min",		&Range::Min,
			"Range min value")
		.add_property ("Max",		&Range::Max,
			"Range max value")
		.add_property ("Length",	&Range::Length,
			"Length of the range");

//============================================================================//
//      Expose "ConfidenceInterval" class to Python                           //
//============================================================================//
	class_ <ConfidenceInterval> ("ConfidenceInterval",
		"A confidence interval for a value",
		init <double, double, Range> (args ("level", "value", "range"),
			"Define a value and its range"))

		// Methods
		.def (self_ns::str (self_ns::self))

		// Properties
		.add_property ("Level",		&ConfidenceInterval::Level,
			"Confidence level of the confidence interval")
		.add_property ("Value",		&ConfidenceInterval::Value,
			"The central value of the confidence interval")
		.add_property ("RelError",	&ConfidenceInterval::RelError,
			"Relative error of the value");

//============================================================================//
//      Discrete distributions                                                //
//============================================================================//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Expose "DiscreteUniform" class to Python                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	class_ <DiscreteUniform> ("DiscreteUniform",
		"Model for a Discrete Uniform distribution",
		init <int64_t, int64_t> (args ("min", "max"),
			"Create a new Discrete Uniform distribution"))
		.def (init <const Observations &> (args ("data"),
			"Create a new Discrete Uniform distribution from empirical data"))

		// Methods
		BASE_CLASS_METHODS(DiscreteUniform)

		// Properties
		BASE_CLASS_PROPERTIES (DiscreteUniform);

//============================================================================//
//      Continuous distributions                                              //
//============================================================================//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Expose "Kolmogorov" class to Python                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	class_ <Kolmogorov> ("Kolmogorov",
		"Model for a Kolmogorov distribution",
		init <> ("Create a new Kolmogorov distribution"))

		// Methods
		BASE_CLASS_METHODS(Kolmogorov)

		// Properties
		BASE_CLASS_PROPERTIES(Kolmogorov);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Expose "StandardT" class to Python                                    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	class_ <StandardT> ("StandardT",
		"Model for a Student’s T-distribution",
		init <size_t> (args ("df"),
			"Create a new Student’s T-distribution"))

		// Methods
		BASE_CLASS_METHODS(StandardT)

		// Properties
		BASE_CLASS_PROPERTIES(StandardT)
		.add_property ("DF",	&StandardT::DF,
			"Degrees of freedom of the distribution");

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Expose "F" class to Python                                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	class_ <F> ("F",
		"Model for a Snedecor's F-distribution",
		init <size_t, size_t> (args ("df1", "df2"),
			"Create a new Snedecor's F-distribution"))

		// Methods
		BASE_CLASS_METHODS(F)

		// Properties
		BASE_CLASS_PROPERTIES(F)
		.add_property ("DF1",	&F::DF1,
			"The first degrees of freedom of the distribution")
		.add_property ("DF2",	&F::DF2,
			"The second degrees of freedom of the distribution");

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Expose "Beta" class to Python                                         //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	class_ <Beta> ("Beta",
		"Model for the Beta distribution",
		init <double, double> (args ("shape1", "shape2"),
			"Create a new Beta distribution"))
		.def (init <const Observations &> (args ("data"),
			"Create a new Beta distribution from empirical data"))

		// Methods
		BASE_CLASS_METHODS(Beta)

		// Properties
		BASE_CLASS_PROPERTIES(Beta)
		.add_property ("Shape1",	&Beta::Shape1,
			"The first shape parameter of the distribution")
		.add_property ("Shape2",	&Beta::Shape2,
			"The second shape parameter of the distribution");

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Expose "Erlang" class to Python                                       //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	class_ <Erlang> ("Erlang",
		"Model for an Erlang distribution",
		init <size_t, double> (args ("shape", "scale"),
			"Create a new Erlang distribution"))
		.def (init <const Observations &> (args ("data"),
			"Create a new Erlang distribution from empirical data"))

		// Methods
		BASE_CLASS_METHODS(Erlang)

		// Properties
		BASE_CLASS_PROPERTIES(Erlang)
		.add_property ("Shape",	&Erlang::Shape,
			"Shape of the distribution")
		.add_property ("Scale",	&Erlang::Scale,
			"Scale of the distribution");

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Expose "Chi-Squared" class to Python                                  //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	class_ <ChiSquared> ("ChiSquared",
		"Model for a Chi-Squared distribution",
		init <size_t> (args ("df"),
			"Create a new Chi-Squared distribution"))
		.def (init <const Observations &> (args ("data"),
			"Create a new Chi-Squared distribution from empirical data"))

		// Methods
		BASE_CLASS_METHODS(ChiSquared)

		// Properties
		BASE_CLASS_PROPERTIES(ChiSquared)
		.add_property ("DF",	&ChiSquared::DF,
			"Degrees of freedom of the distribution");

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Expose "Exponential" class to Python                                  //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	class_ <Exponential> ("Exponential",
		"Model for an Exponential distribution",
		init <double> (args ("scale"),
			"Create a new Exponential distribution"))
		.def (init <const Observations &> (args ("data"),
			"Create a new Exponential distribution from empirical data"))

		// Methods
		BASE_CLASS_METHODS(Exponential)
		.def ("Mean_ConfidenceInterval",	&Exponential::Mean_ConfidenceInterval,
			args ("level", "size"), "Confidence interval of the mean value")

		// Properties
		BASE_CLASS_PROPERTIES(Exponential)
		.add_property ("Scale",				&Exponential::Scale,
			"Scale of the distrsibution");

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Expose "Normal" class to Python                                       //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	class_ <Normal> ("Normal",
		"Model for a Normal (Gaussian) distribution",
		init <double, double> (args ("location", "scale"),
			"Create a new Normal (Gaussian) distribution"))
		.def (init <const Observations &> (args ("data"),
			"Create a new Normal (Gaussian) distribution from empirical data"))

		// Methods
		BASE_CLASS_METHODS(Normal)
		.def ("Mean_ConfidenceInterval",		&Normal::Mean_ConfidenceInterval,
			args ("level", "size"), "Confidence interval of the mean")
		.def ("Variance_ConfidenceInterval",	&Normal::Variance_ConfidenceInterval,
			args ("level", "size"), "Confidence interval of the variance")

		// Properties
		CONTINUOUS_CLASS_PROPERTIES (Normal);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Expose "Laplace" class to Python                                      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	class_ <Laplace> ("Laplace",
		"Model for a Laplace distribution",
		init <double, double> (args ("location", "scale"),
			"Create a new Laplace distribution"))
		.def (init <const Observations &> (args ("data"),
			"Create a new Laplace distribution from empirical data"))

		// Methods
		BASE_CLASS_METHODS(Laplace)
		.def ("MAD_ConfidenceInterval",	&Laplace::MAD_ConfidenceInterval,
			args ("level", "size"), "Confidence interval of the Mean Absolute Deviation (MAD)")

		// Properties
		CONTINUOUS_CLASS_PROPERTIES (Laplace)
		.add_property ("MAD",			&Laplace::MAD,
			"Mean Absolute Deviation (MAD) of the distrsibution");

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Expose "AsymmetricLaplace" class to Python                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	class_ <AsymmetricLaplace> ("AsymmetricLaplace",
		"Model for an asymmetric Laplace distribution",
		init <double, double, double> (args ("location", "scale", "asymmetry"),
			"Create a new asymmetric Laplace distribution"))
		.def (init <const Observations &> (args ("data"),
			"Create a new asymmetric Laplace distribution from empirical data"))

		// Methods
		BASE_CLASS_METHODS(AsymmetricLaplace)

		// Properties
		CONTINUOUS_CLASS_PROPERTIES (AsymmetricLaplace)
		.add_property ("Asymmetry",	&AsymmetricLaplace::Asymmetry,
			"Asymmetry of the distrsibution");
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
