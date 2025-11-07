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
# include	"smooth_filter.hpp"
# include	"diff_filter.hpp"
# include	"swing_filter.hpp"
# include	"hilbert_filter.hpp"
# include	"min_delay.hpp"

//****************************************************************************//
//      Pointers to the filter overloaded methods                             //
//****************************************************************************//
# define	POINTERS_COMMON(class) 												\
vector <double> (class::*class##Apply)(const vector <double> &data) const	= &class::Apply;
vector <double> (BaseFilter::*ListApply)(const pylist &py_list) const 		= &BaseFilter::Apply;

//****************************************************************************//
//      Base methods of the time series filters                               //
//****************************************************************************//
# define	FILTERS_BASE(class) 												\
	.add_property ("Points",	&class::Points, 								\
		"Count of neighbor points to filter by") 								\
	.def ("Apply",				ListApply,		args ("data"),					\
		"Apply the filter to the target python list")							\
	.def ("Apply",				class##Apply,	args ("data"),					\
		"Apply the filter to the target time series")							\
	.def (self_ns::str (self_ns::self));

//****************************************************************************//
//      Common methods of the time series filters                             //
//****************************************************************************//
# define	FILTERS_COMMON(class) 												\
	.add_property ("Size",		&class::Size, 									\
		"Size of the impulse response function") 								\
	.def ("Impulse",			&class::Impulse, 								\
		return_internal_reference <> (), 										\
		"Values of the impulse response function")								\
	FILTERS_BASE(class)

//****************************************************************************//
//      Python module initialization functions                                //
//****************************************************************************//
BOOST_PYTHON_MODULE (filters) {

	// Use shortenings
	using namespace boost::python;

//============================================================================//
//      Expose "MedianFilter" class to Python                                 //
//============================================================================//
	POINTERS_COMMON(MedianFilter)
	class_ <MedianFilter> ("MedianFilter",
		"Median filter for a time series",
		init <size_t> (args ("points"),
			"Set count of neighbor points to filter by"))
		FILTERS_BASE(MedianFilter);

//============================================================================//
//      Expose "SmoothFilter" class to Python                                 //
//============================================================================//
	POINTERS_COMMON(SmoothFilter)
	class_ <SmoothFilter> ("SmoothFilter",
		"Smooth filter for a time series",
		init <size_t> (args ("points"),
			"Set count of neighbor points to smooth by"))
		FILTERS_COMMON(SmoothFilter);

//============================================================================//
//      Expose "DiffFilter" class to Python                                   //
//============================================================================//
	POINTERS_COMMON(DiffFilter)
	class_ <DiffFilter> ("DiffFilter",
		"Differentiator filter for a time series",
		init <size_t, double> (args ("points", "step"),
			"Set count of neighbor points to smooth by and the discretization step"))
		FILTERS_COMMON(DiffFilter);

//============================================================================//
//      Expose "SwingFilter" class to Python                                  //
//============================================================================//
	POINTERS_COMMON(SwingFilter)
	class_ <SwingFilter> ("SwingFilter",
		"Swing filter (no mean value) for a time series",
		init <size_t> (args ("points"),
			"Set count of neighbor points to smooth by"))
		FILTERS_COMMON(SwingFilter);

//============================================================================//
//      Expose "HilbertFilter" class to Python                                //
//============================================================================//
	POINTERS_COMMON(HilbertFilter)
	class_ <HilbertFilter> ("HilbertFilter",
		"Hilbert transform filter for a time series",
		init <size_t> (args ("size"),
			"Set size of the dataset for the Hilbert transformation"))
		FILTERS_COMMON(HilbertFilter);

//============================================================================//
//      Expose "MinDelayFilter" class to Python                               //
//============================================================================//
	POINTERS_COMMON(MinDelayFilter)
	class_ <MinDelayFilter> ("MinDelayFilter",
		"Minimum group delay filter for a time series",
		init <size_t> (args ("points"),
			"Set count of neighbor points to filter by"))
		FILTERS_COMMON(MinDelayFilter);
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
