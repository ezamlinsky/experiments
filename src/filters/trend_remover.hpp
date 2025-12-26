/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#        LINEAR TREND CANCELLATION ALGORITHM FOR TRIGONOMETRIC EXPANSION       #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	<vector>
# include	"../templates/statistics.hpp"

// Use shortenings
using namespace std;

//****************************************************************************//
//      Class "TrendRemover"                                                  //
//****************************************************************************//
class TrendRemover
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	const double * const args;	// Values of the independent argument
	size_t size;				// Size of the dataset
	double coeff;				// Tangent of the trend line

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	TrendRemover (
		const double x[],		// Independent argument (time)
		const double y[],		// Dependent argument (values)
		size_t size,			// Size of the dataset
		size_t points			// Connection points to smooth by
	) :	args (x),
		size (size),
		coeff (0.0)
	{
		// Check if the size is set
		if (size == 0)
			throw invalid_argument ("TrendRemover: The size of the dataset should not be zero");

		// Check if the number of connection points is set
		if (points == 0)
			throw invalid_argument ("TrendRemover: The number of connection points should not be zero");

		// Check the limit for the connection points number
		if (points > size / 2)
			throw invalid_argument ("TrendRemover: The connection points should be less than or equal to half the size of the dataset");

		// Get the left point for smooth connection
		const double left_x = Stats::Mean (x, points);
		const double left_y = Stats::Mean (y, points);

		// Get the right point for smooth connection
		const double right_x = Stats::Mean (x + size - points, points);
		const double right_y = Stats::Mean (y + size - points, points);

		// Calculate the connection (trend) line
		const double p = right_y - left_y;
		const double q = right_x - left_x;
		coeff = p / q;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      The trend line                                                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	vector <double> Line (void) const {
		vector <double> line (args, args + size);
		Array::Mul (line.data(), size, coeff);
		return line;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Tangent of the trend line                                             //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double Coeff (void) const {
		return coeff;
	}
};
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
