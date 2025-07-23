/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#   CALCULATE A CUMULATIVE DISTRIBUTION FUNCTION (CDF) USING DATA OR A MODEL   #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"discrete.hpp"
# include	"../models/continuous/kolmogorov.hpp"

//****************************************************************************//
//      Class "CDFValues"                                                     //
//****************************************************************************//
class CDFValues : public DiscreteDistribution
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	bool theoretical;		// If set, then the CDF is calculated theoretically

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Default constructor                                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	CDFValues (void)
	:	theoretical (false)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor for a theoretical model                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	CDFValues (
		const vector <double> &values,	// Unique values
		const BaseModel &model			// Theoretical model of the CDF
	) :	DiscreteDistribution (values, model),
		theoretical (true)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors for empirical data                                       //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	CDFValues (
		const Observations &sample		// Observations of a random value
	) : DiscreteDistribution (sample),
		theoretical (false)
	{}

	CDFValues (
		const vector <double> &data		// Empirical dataset
	) : DiscreteDistribution (vector <double> (data)),
		theoretical (false)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Indicate that the CDF function has been calculated theoretically      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	bool IsTheoretical (void) const {
		return theoretical;
	}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const CDFValues &object)
{
	auto restore = stream.precision();
	stream.precision (PRECISION);
	if (object.IsTheoretical()) {
		stream << "\nTHEORETICAL CDF VALUES:" << endl;
		stream << "=======================" << endl;
	}
	else {
		stream << "\nEMPIRICAL CDF VALUES:" << endl;
		stream << "=====================" << endl;
	}
	stream << "Data points\t\t\t\t= " << object.Size() << endl;
	stream << object.Domain();
	stream.precision (restore);
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
