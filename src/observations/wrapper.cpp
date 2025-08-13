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
# include	"observations.hpp"
# include	"population.hpp"
# include	"sample.hpp"

//****************************************************************************//
//      Methods are inherited from the base observations class                //
//****************************************************************************//
# define	BASE_CLASS_METHODS(class) 											\
	.def ("Domain",				&class::Domain,									\
		return_internal_reference <> (),										\
		"Values range")															\
	.def ("Data",				&class::Data,									\
		"Ranked dataset")														\
	.def ("TrimMin",			&class::TrimMin,			args ("percent"),	\
		"Trim the target percent of observed min values")						\
	.def ("TrimMax",			&class::TrimMax,			args ("percent"),	\
		"Trim the target percent of observed max values")						\
	.def ("TrimBoth",			&class::TrimBoth,			args ("percent"),	\
		"Trim the target percent of observed min and max values")				\
	.def ("TrimLess",			&class::TrimLess,			args ("value"),		\
		"Trim all the values less than the target value")						\
	.def ("TrimLessOrEqual",	&class::TrimLessOrEqual,	args ("value"),		\
		"Trim all the values less than or equal to the target value")			\
	.def ("TrimGreater",		&class::TrimGreater,		args ("value"),		\
		"Trim all the values greater than the target value")					\
	.def ("TrimGreaterOrEqual",	&class::TrimGreaterOrEqual,	args ("value"),		\
		"Trim all the values greater than or equal to the target value")		\
	.def ("Add",				&class::Add,	args ("value"),					\
		"Add a value to all the observations")									\
	.def ("Sub",				&class::Sub,	args ("value"),					\
		"Subtract a value from all the observations")							\
	.def ("Mul",				&class::Mul,	args ("value"),					\
		"Multiply all the observations by a value")								\
	.def ("Div",				&class::Div,	args ("value"),					\
		"Divide all the observations by a value")								\
	.def ("Quantile",			&class::Quantile,			args ("level"),		\
		"Quantile value for the target level")									\
	.def ("data",				&class::data,	return_internal_reference <> (),\
		"Extract internal data about observed values")							\
	.def (self_ns::str (self_ns::self))

//****************************************************************************//
//      Properties are inherited from the base observations class             //
//****************************************************************************//
# define	BASE_CLASS_PROPERTIES(class) 										\
	.add_property ("Size",						&class::Size,					\
		"Count of observed values in the dataset")								\
	.add_property ("LowerQuartile",				&class::LowerQuartile,			\
		"Lower quartile of the dataset")										\
	.add_property ("UpperQuartile",				&class::UpperQuartile,			\
		"Upper quartile of the dataset")										\
	.add_property ("InterQuartileRange",		&class::InterQuartileRange,		\
		"Inter-quartile range (IQR) of the dataset")							\
	.add_property ("MidHinge",					&class::MidHinge,				\
		"Mid-hinge of the dataset")												\
	.add_property ("TriMean",					&class::TriMean,				\
		"Tukey's tri-mean of the dataset")										\
	.add_property ("QuartileSkewness",			&class::QuartileSkewness,		\
		"Quartile skewness of the dataset")										\
	.add_property ("Median",					&class::Median,					\
		"Median of the dataset")												\
	.add_property ("Mean",						&class::Mean,					\
		"Mean of the dataset")													\
	.add_property ("Variance",					&class::Variance,				\
		"Variance of the dataset")												\
	.add_property ("StdDev",					&class::StdDev,					\
		"Standard deviation of the dataset")									\
	.add_property ("StdErr",					&class::StdErr,					\
		"Standard error of the dataset")										\
	.add_property ("MeanSqrDevFromMean",		&class::MeanSqrDevFromMean,		\
		"Mean of squared deviations from the mean value")						\
	.add_property ("MeanAbsDevFromMean",		&class::MeanAbsDevFromMean,		\
		"Mean of absolute deviations from the mean value")						\
	.add_property ("MeanSqrDevFromMedian",		&class::MeanSqrDevFromMedian,	\
		"Mean of squared deviations from the median value")						\
	.add_property ("MeanAbsDevFromMedian",		&class::MeanAbsDevFromMedian,	\
		"Mean of absolute deviations from the median value")					\
	.add_property ("MeanSignDevFromMedian",		&class::MeanSignDevFromMedian,	\
		"Mean of signed deviations from the median value")						\
	.add_property ("MedianSqrDevFromMean",		&class::MedianSqrDevFromMean,	\
		"Median of squared deviations from the mean value")						\
	.add_property ("MedianAbsDevFromMean",		&class::MedianAbsDevFromMean,	\
		"Median of absolute deviations from the mean value")					\
	.add_property ("MedianSignDevFromMean",		&class::MedianSignDevFromMean,	\
		"Median of signed deviations from the mean value")						\
	.add_property ("MedianSqrDevFromMedian",	&class::MedianSqrDevFromMedian,	\
		"Median of squared deviations from the median value")					\
	.add_property ("MedianAbsDevFromMedian",	&class::MedianSqrDevFromMedian,	\
		"Median of absolute deviations from the median value")					\
	.add_property ("MedianSignDevFromMedian",	&class::MedianSignDevFromMedian,\
		"Median of signed deviations from the median value")					\
	.add_property ("VariationAroundMean",		&class::VariationAroundMean,	\
		"Variation around the mean value")										\
	.add_property ("SkewnessAroundMean",		&class::SkewnessAroundMean,		\
		"Skewness around the mean value")										\
	.add_property ("KurtosisAroundMean",		&class::KurtosisAroundMean,		\
		"Kurtosis around the mean value")										\
	.add_property ("KurtosisExcessAroundMean",	&class::KurtosisExcessAroundMean,\
		"Excess kurtosis around the mean value")								\
	.add_property ("VariationAroundMedian",		&class::VariationAroundMedian,	\
		"Variation around the median value")									\
	.add_property ("SkewnessAroundMedian",		&class::SkewnessAroundMedian,	\
		"Skewness around the median value")										\
	.add_property ("KurtosisAroundMedian",		&class::KurtosisAroundMedian,	\
		"Kurtosis around the median value")										\
	.add_property ("KurtosisExcessAroundMedian",&class::KurtosisExcessAroundMedian,\
		"Excess kurtosis around the median value")								\
	.add_property ("PearsonSkewness",			&class::PearsonSkewness,		\
		"Pearson's skewness estimator of the dataset")							\
	.add_property ("MeedenSkewness",			&class::MeedenSkewness,			\
		"Meeden's skewness estimator of the dataset")

//****************************************************************************//
//      Python module initialization functions                                //
//****************************************************************************//
BOOST_PYTHON_MODULE (observations) {

	// Use shortenings
	using namespace boost::python;

//============================================================================//
//      Expose "Population" class to Python                                   //
//============================================================================//
	class_ <Population> ("Population",
		"Statistical population",
		init <const list&, double, double> (args ("data", "mean", "median"),
			"Initialize a statistical population from data"))
		.def (init <const vector <double>&, double, double>
			(args ("data", "mean", "median"),
			"Initialize a statistical population from data"))

		// Methods
		BASE_CLASS_METHODS(Population)

		// Properties
		BASE_CLASS_PROPERTIES(Population);

//============================================================================//
//      Expose "Sample" class to Python                                       //
//============================================================================//
	class_ <Sample> ("Sample",
		"Statistical sample",
		init <const list&> (args ("data"),
			"Initialize a statistical sample from data"))
		.def (init <const vector <double>&> (args ("data"),
			"Initialize a statistical sample from data"))

		// Methods
		BASE_CLASS_METHODS(Sample)

		// Properties
		BASE_CLASS_PROPERTIES(Sample);
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
