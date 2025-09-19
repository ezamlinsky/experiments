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
//      Name space "Model"                                                    //
//****************************************************************************//
namespace Model
{
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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Summary of the object                                                 //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const ObjectSummary Summary (void) const {

		// Create the summary storage
		const Range &range = static_cast <const Range&> (*this);
		ObjectSummary summary = range.Summary();
		summary.Name ("Confidence Interval");

		// Confidence interval
		PropGroup &group = summary.Groups()[0];
		group.Prepend ("Value", Value());
		group.Prepend ("Confidence level (%)", Level() * 100);
		group.Append ("Relative error (%)", RelError() * 100);

		// Return the summary
		return summary;
	}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const ConfidenceInterval &object)
{
	stream << object.Summary();
	return stream;
}
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
