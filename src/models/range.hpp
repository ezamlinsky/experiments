/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                  CONTAIN A VALID RANGE FOR A NUMERIC VALUE                   #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	<stdexcept>
# include	<iostream>
# include	<iomanip>
# include	<vector>
# include	<algorithm>
# include	"../templates/array.hpp"

// Displayed precision for floating-point numbers
# define	PRECISION	16

// Use shortenings
using namespace std;

//****************************************************************************//
//      Class "Range"                                                         //
//****************************************************************************//
class Range
{
//============================================================================//
//      Members                                                               //
//============================================================================//
protected:
	double min;					// Min value of the range
	double max;					// Max value of the range

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Default constructor                                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Range (void)
	:	min (0.0),
		max (0.0)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Trivial constructor                                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Range (
		double min,				// Min value of the range
		double max				// Max value of the range
	) : min (min),
		max (max)
	{
		if (!(min <= max))
			throw invalid_argument ("Range: Passed invalid min and max values");
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors by empirical data                                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Range (
		const double data[],		// Dataset to find its range
		size_t size
	) :	Range()
	{
		if (size) Array::MinMax (min, max, data, size);
	}

	Range (
		const vector <double> &data	// Dataset to find its range
	) : Range (data.data(), data.size())
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Range min value                                                       //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double Min (void) const {
		return min;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Range max value                                                       //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double Max (void) const {
		return max;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Length of the range                                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double Length (void) const {
		return max - min;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Middle point of the range                                             //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double MidRange (void) const {
		return 0.5 * (max + min);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Clamp the given value to be within the range [min..max]               //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double Clamp (
		double value			// The value to fix
	) const {
		if (value < min) value = min;
		if (value > max) value = max;
		return value;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Split the range into subranges (bins)                                 //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	vector <double> Split (
		size_t bins				// The number of bins to create for the range
	) const {
		if (bins != 0) {
			vector <double> result;
			result.push_back (min);
			for (size_t i = 0; i < bins; i++) {
				const size_t index = i + 1;
				const double x = ((bins - index) * min + index * max) / bins;
				result.push_back (x);
			}
			return result;
		}
		else
			throw invalid_argument ("Split: The number of bins must be positive");
	}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const Range &range)
{
	auto restore = stream.precision();
	stream.precision (PRECISION);
	stream << "\nRange:" << endl;
	stream << "~~~~~~" << endl;
	stream << "    Min value\t\t\t\t= " << range.Min() << endl;
	stream << "    Max value\t\t\t\t= " << range.Max() << endl;
	stream << "    Range length\t\t\t= " << range.Length() << endl;
	stream << "    Middle point\t\t\t= " << range.MidRange() << endl;
	stream.precision (restore);
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
