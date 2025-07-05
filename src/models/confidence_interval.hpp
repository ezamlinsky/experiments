/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#              DEFINE A CONFIDENCE INTERVAL FOR THE TARGET VALUE               #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"range.hpp"

//****************************************************************************//
//      Class "ConfidenceInterval"                                            //
//****************************************************************************//
class ConfidenceInterval : public Range
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	const double level;		// Confidence level
	const double value;		// The estimated value

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	ConfidenceInterval (
		double level,		// Confidence level of the confidence interval
		double value,		// The value of the confidence interval
		Range range			// Range value of the confidence interval
	) : Range (range),
		level (level),
		value (value)
	{
		if (value < min || value > max)
			throw invalid_argument ("Confidence Interval: Passed value is out of range [min..max]");
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Confidence level of the confidence interval                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double Level (void) const {
		return level;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      The central value of the confidence interval                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double Value (void) const {
		return value;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Relative error of the value                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double RelError (void) const {
		return Length() / value;
	}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const ConfidenceInterval &interval)
{
	auto restore = stream.precision();
	stream.precision (PRECISION);
	stream << "\nCONFIDENCE INTERVAL:" << endl;
	stream << "====================" << endl;
	stream << "Confidence level\t\t\t= " << interval.Level() << endl;
	stream << "Value\t\t\t\t\t= " << interval.Value() << endl;
	stream << "Relative error (%)\t\t\t= " << fixed << setprecision (3) << interval.RelError() * 100 << endl;
	stream << static_cast <const Range&> (interval);
	stream.precision (restore);
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
