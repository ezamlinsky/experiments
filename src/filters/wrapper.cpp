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
# include	"median.hpp"
# include	"smooth.hpp"
# include	"min_delay.hpp"

//****************************************************************************//
//      Common methods for time series filters                                //
//****************************************************************************//
# define	FILTERS_COMMON(class) 												\
	.add_property ("Points",	&class::Points, 								\
		"Count of neighbor points to filter by") 								\
	.add_property ("Size",		&class::Size, 									\
		"Size of the impulse response function") 								\
	.def ("Impulse",			&class::Impulse, 								\
		return_internal_reference <> (), 										\
		"Values of the impulse response function")								\
	.def (self_ns::str (self_ns::self));

//****************************************************************************//
//      Python module initialization functions                                //
//****************************************************************************//
BOOST_PYTHON_MODULE (filters) {

	// Use shortenings
	using namespace boost::python;

//============================================================================//
//      Expose "MedianFilter" class to Python                                 //
//============================================================================//
vector <double> (MedianFilter::*Apply1)(const list &py_list) const			= &MedianFilter::Apply;
vector <double> (MedianFilter::*Apply2)(const vector <double> &data) const	= &MedianFilter::Apply;
	class_ <MedianFilter> ("MedianFilter",
		"Median filter for a time series",
		init <size_t> (args ("points"),
			"Set count of neighbor points to filter by"))
		.add_property ("Points",	&MedianFilter::Points,
			"Count of neighbor points to filter by")
		.def ("Apply",	Apply1,	args ("data"),
			"Apply the filter to the target python list")
		.def ("Apply",	Apply2,	args ("data"),
			"Apply the filter to the target time series")
		.def (self_ns::str (self_ns::self));

//============================================================================//
//      Expose "SmoothFilter" class to Python                                 //
//============================================================================//
vector <double> (SmoothFilter::*Apply3)(const list &py_list) const			= &SmoothFilter::Apply;
vector <double> (SmoothFilter::*Apply4)(const vector <double> &data) const	= &SmoothFilter::Apply;
	class_ <SmoothFilter> ("SmoothFilter",
		"Smooth filter for a time series",
		init <size_t> (args ("points"),
			"Set count of neighbor points to smooth by"))
		.def ("Apply",	Apply3,	args ("data"),
			"Apply the filter to the target python list")
		.def ("Apply",	Apply4,	args ("data"),
			"Apply the filter to the target time series")
		FILTERS_COMMON(SmoothFilter);

//============================================================================//
//      Expose "MinDelayFilter" class to Python                               //
//============================================================================//
vector <double> (MinDelayFilter::*Apply5)(const list &py_list) const			= &MinDelayFilter::Apply;
vector <double> (MinDelayFilter::*Apply6)(const vector <double> &data) const	= &MinDelayFilter::Apply;
	class_ <MinDelayFilter> ("MinDelayFilter",
		"Minimum group delay filter for a time series",
		init <size_t> (args ("points"),
			"Set count of neighbor points to filter by"))
		.def ("Apply",	Apply5,	args ("data"),
			"Apply the filter to the target python list")
		.def ("Apply",	Apply6,	args ("data"),
			"Apply the filter to the target time series")
		FILTERS_COMMON(MinDelayFilter);
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
