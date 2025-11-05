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
# include	"linear_regression.hpp"
# include	"trigonometric_regression.hpp"

//****************************************************************************//
//      Methods are inherited from the base class                             //
//****************************************************************************//
# define	BASE_CLASS_METHODS(class) 											\
	.def ("Domain",			&class::Domain,										\
		return_internal_reference <> (),										\
		"Range of predictors (independent variables)")							\
	.def ("X",				&class::X,											\
		"Predictors (independent variables)")									\
	.def ("Y",				&class::Y,											\
		"Response (dependent variables)")										\
	.def ("Approx",			&class::Approx,										\
		"Regression approximation")												\
	.def ("Residuals",		&class::Residuals,									\
		"Regression residuals")													\
	.def ("ApproxMin",		&class::ApproxMin,	args ("stdev"),					\
		"Deviation from the regression approximation line (min values)")		\
	.def ("ApproxMax",		&class::ApproxMax,	args ("stdev"),					\
		"Deviation from the regression approximation line (max values)")		\
	.def ("Coeffs",			&class::Coeffs,										\
		"Regression coefficients")												\
	.def ("StdErrCoeffs",	&class::StdErrCoeffs,								\
		"Standard errors of the regression coefficients")						\
	.def ("Regression",		&class::Regression,	args ("x"),						\
		"Regression value for the target argument")								\
	.def (self_ns::str (self_ns::self))

//****************************************************************************//
//      Properties are inherited from the base class                          //
//****************************************************************************//
# define	BASE_CLASS_PROPERTIES(class) 										\
	.add_property ("TSS",					&class::TSS,						\
		"Total sum of squares (TSS)")											\
	.add_property ("RSS",					&class::RSS,						\
		"Residual sum of squares (RSS)")										\
	.add_property ("ESS",					&class::ESS,						\
		"Explained sum of squares (ESS)")										\
	.add_property ("MSE_TSS",				&class::MSE_TSS,					\
		"Mean square error (MSE) of total sum of squares (TSS)")				\
	.add_property ("MSE_RSS",				&class::MSE_RSS,					\
		"Mean square error (MSE) of residual sum of squares (RSS)")				\
	.add_property ("MSE_ESS",				&class::MSE_ESS,					\
		"Mean square error (MSE) of explained sum of squares (ESS)")			\
	.add_property ("ExplainedVarFrac",		&class::ExplainedVarFrac,			\
		"Fraction of variance explained")										\
	.add_property ("UnexplainedVarFrac",	&class::UnexplainedVarFrac,			\
		"Fraction of variance unexplained")										\
	.add_property ("Determination",			&class::Determination,				\
		"Coefficient of determination")											\
	.add_property ("Correlation",			&class::Correlation,				\
		"Coefficient of correlation")											\
	.add_property ("StdErrCorrelation",		&class::StdErrCorrelation,			\
		"Standard error of the coefficient of correlation")						\
	.add_property ("Size",					&class::Size,						\
		"Data size")															\
	.add_property ("Degree",				&class::Degree,						\
		"Regression degree")													\
	.add_property ("Functions",				&class::Functions,					\
		"Count of the orthogonal functions used")

//****************************************************************************//
//      Python module initialization functions                                //
//****************************************************************************//
BOOST_PYTHON_MODULE (regression) {

	// Use shortenings
	using namespace boost::python;

//============================================================================//
//      Expose "LegendreRegression" class to Python                           //
//============================================================================//
	class_ <LegendreRegression> ("LegendreRegression",
		"Polynomial regression by orthogonal legendre polynomials",
		init <const pylist&, const pylist&, size_t> (args ("x", "y", "degree"),
			"Calculate the Legendre regression over the dataset"))
		.def (init <vector <double>, vector <double>, size_t>
			(args ("x", "y", "degree"),
			"Calculate the Legendre regression over the dataset"))

		// Methods
		BASE_CLASS_METHODS(LegendreRegression)

		// Properties
		BASE_CLASS_PROPERTIES(LegendreRegression);

//============================================================================//
//      Expose "LinearRegression" class to Python                             //
//============================================================================//
	class_ <LinearRegression> ("LinearRegression",
		"Classical linear regression over the dataset",
		init <const pylist&, const pylist&> (args ("x", "y"),
			"Calculate the linear regression over the dataset"))
		.def (init <vector <double>, vector <double>>
			(args ("x", "y"),
			"Calculate the linear regression over the dataset"))

		// Methods
		BASE_CLASS_METHODS(LinearRegression)

		// Properties
		BASE_CLASS_PROPERTIES(LinearRegression);

//============================================================================//
//      Expose "TrigonometricRegression" class to Python                      //
//============================================================================//
	class_ <TrigonometricRegression> ("TrigonometricRegression",
		"Trigonometric regression by orthogonal trigonometric functions",
		init <const pylist&, const pylist&, size_t> (args ("x", "y", "degree"),
			"Calculate the trigonometric regression over the dataset"))
		.def (init <vector <double>, vector <double>, size_t>
			(args ("x", "y", "degree"),
			"Calculate the trigonometric regression over the dataset"))

		// Methods
		BASE_CLASS_METHODS(TrigonometricRegression)

		// Properties
		BASE_CLASS_PROPERTIES(TrigonometricRegression);
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
