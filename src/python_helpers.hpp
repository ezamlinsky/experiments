/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                  HELPER FUNCTIONS TO CONNECT C++ AND PYTHON                  #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	<vector>
# include	<boost/python.hpp>

// Use shortenings
using namespace std;
using namespace boost::python;
using pylist = boost::python::list;

//****************************************************************************//
//      Convert a Python list to a standard array                             //
//****************************************************************************//
inline vector <double> to_vector (
	const pylist &py_list			// The Python list to convert
){
	using lit = boost::python::stl_input_iterator <double>;
	return vector <double> (lit (py_list), lit ());
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
