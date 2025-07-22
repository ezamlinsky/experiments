/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#        CALCULATE PDF AND CDF FOR A DISCRETE PROBABILITY DISTRIBUTION         #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"base.hpp"

//****************************************************************************//
//      Class "DiscreteDistribution"                                          //
//****************************************************************************//
class DiscreteDistribution : public BaseDistribution
{
//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Default constructor                                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	DiscreteDistribution (void) = default;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors for empirical data                                       //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	DiscreteDistribution (
		const vector <double> &data		// Empirical dataset
	) :	BaseDistribution (move (vector <double> (data)))
	{}

	DiscreteDistribution (
		const list &py_list				// Empirical dataset
	) : DiscreteDistribution (to_vector (py_list))
	{}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const DiscreteDistribution &object)
{
	auto restore = stream.precision();
	stream.precision (PRECISION);
	stream << "\nDISCRETE DISTRIBUTION:" << endl;
	stream << "======================" << endl;
	stream << "Bins\t\t\t\t\t= " << object.Size() << endl;
	stream << object.Domain();
	stream.precision (restore);
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
