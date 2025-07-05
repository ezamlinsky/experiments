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
# include	"bins.hpp"

//****************************************************************************//
//      Python module initialization functions                                //
//****************************************************************************//
BOOST_PYTHON_MODULE (distribution) {

	// Use shortenings
	using namespace boost::python;

//============================================================================//
//      Expose Bins class to Python                                           //
//============================================================================//
	class_ <Bins> ("Bins", init <double> ())
	.add_property ("Raw",			&Bins::Raw,
		"Raw number of observations")
	.add_property ("Default",		&Bins::Default,
		"Default (square root) choice")
	.add_property ("Sturges",		&Bins::Sturges,
		"Sturges's rule")
	.add_property ("Rice",			&Bins::Rice,
		"Rice rule")
	.add_property ("TerrellScott",	&Bins::TerrellScott,
		"Terrell-Scott rule");
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
