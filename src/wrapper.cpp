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
	void (*RangeShow1)(const ::Range &range)						= &Show;
	void (*RangeShow2)(const string &name, const ::Range &range)	= &Show;
	def ("show", RangeShow1, "Show a GUI window with a range summary");
	def ("show", RangeShow2, "Show a GUI window with a range summary and a custom title");

//============================================================================//
//      Expose "Show" functions for the "ConfidenceInterval" object           //
//============================================================================//
	void (*CIShow1)(const ConfidenceInterval &range)						= &Show;
	void (*CIShow2)(const string &name, const ConfidenceInterval &range)	= &Show;
	def ("show", CIShow1, "Show a GUI window with a confidence interval summary");
	def ("show", CIShow2, "Show a GUI window with a confidence interval summary and a custom title");

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
