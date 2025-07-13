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
//      Python module initialization functions                                //
//****************************************************************************//
BOOST_PYTHON_MODULE (distribution) {

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
		.def ("Apply",	Apply1,	args ("data"),
			"Apply the filter to the target python list")
		.def ("Apply",	Apply2,	args ("data"),
			"Apply the filter to the target time series");

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
			"Apply the filter to the target time series");

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
			"Apply the filter to the target time series");
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
