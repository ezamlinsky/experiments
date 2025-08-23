/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#              THE WRAPPER CODE TO EXPOSE C++ FUNCTIONS TO PYTHON              #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"gui.hpp"

//****************************************************************************//
//      Python module initialization functions                                //
//****************************************************************************//
BOOST_PYTHON_MODULE (gui) {

	// Use shortenings
	using namespace boost::python;

	// 'Show' functions for different objects
	void (*Show1)(const Population &population)	= &Show;
	void (*Show2)(const Sample &sample)			= &Show;

//============================================================================//
//      Expose "Show" functions to Python                                     //
//============================================================================//
	def ("show", Show1, "Show a GUI window with a population summary");
	def ("show", Show2, "Show a GUI window with a sample summary");
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
