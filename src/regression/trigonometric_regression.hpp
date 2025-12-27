/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#        TRIGONOMETRIC REGRESSION BY ORTHOGONAL TRIGONOMETRIC FUNCTIONS        #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"orthogonal_regression.hpp"
# include	"../python_helpers.hpp"

//****************************************************************************//
//      Class "TrigonometricFunctions"                                        //
//****************************************************************************//
class TrigonometricFunctions : public OrthogonalFunctions
{
//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	TrigonometricFunctions (
		double x[],				// X values
		size_t size,			// Number of the X values
		size_t degree			// Polynomial degree
	) : OrthogonalFunctions (x, size, degree)
	{
		// Check if the degree is properly specified
		if (2 * degree > size)
			throw invalid_argument ("TrigonometricFunctions: The degree should not be greater than half of the data size");

		// The first function is an average value
		funcs.push_back (mvector (1.0, size));

		// Compute sines and cosines for different multiplicity factors
		double cos_vals [size];
		double sin_vals [size];
		for (size_t j = 1; j <= degree; j++) {
			for (size_t i = 0; i < size; i++) {
				cos_vals[i] = cos (M_PI * j * x[i]);
				sin_vals[i] = sin (M_PI * j * x[i]);
			}
			funcs.push_back (mvector (cos_vals, size));
			funcs.push_back (mvector (sin_vals, size));
		}
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Calculate values of all the trigonometric functions for the target X  //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual mvector FuncValues (
		double x				// Value to calculate the functions for
	) const override final {

		// Check the argument range and map to the orthogonality domain
		x = Convert (x);

		// The first function is an average value
		vector <double> vals;
		vals.push_back (1.0);

		// Compute sines and cosines for different multiplicity factors
		for (size_t j = 1; j <= degree; j++) {
			vals.push_back (cos (M_PI * j * x));
			vals.push_back (sin (M_PI * j * x));
		}

		// Return the values
		return vals;
	}
};

//****************************************************************************//
//      Class "TrigonometricRegression"                                       //
//****************************************************************************//
class TrigonometricRegression : public OrthogonalRegression
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	double coeff;				// Coefficient for the linear correction

//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Internal constructor                                                  //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	TrigonometricRegression (
		double x[],				// Predictors (independent variables)
		double y[],				// Response (dependent variables)
		size_t size,			// Size of the dataset
		size_t degree			// Polynomial degree
	) :	OrthogonalRegression (y, new TrigonometricFunctions (Sort (x, y, size), size, degree))
	{
		// Calculate the number of connection points to connect
		// the left and right sides of the dataset smoothly
		size_t points = degree == 0 ? size / 2 : size / 4 / degree;
		if (!points) points = 1;

		// Get the left point for smooth connection
		const double left_x = Stats::Mean (x, points);
		const double left_y = Stats::Mean (y, points);

		// Get the right point for smooth connection
		const double right_x = Stats::Mean (x + size - points, points);
		const double right_y = Stats::Mean (y + size - points, points);

		// Calculate the connection line
		const double p = right_y - left_y;
		const double q = right_x - left_x;
		coeff = p / q;

		// Build the connection line
		const mvector trend_line = mvector (x, size) * coeff;

		// Adjust X values before the approximation
		residuals -= trend_line;

		// Approximate the dependent variables by the regression
		Approximate();

		// Adjust the regression with the connection line we eliminated before
		approx += trend_line;
	}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors                                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	TrigonometricRegression (
		vector <double> x,		// Predictors (independent variables)
		vector <double> y,		// Response (dependent variables)
		size_t degree			// Polynomial degree
	) :	TrigonometricRegression (x.data(), y.data(), min (x.size(), y.size()), degree)
	{}

	TrigonometricRegression (
		const pylist &x,		// Predictors (independent variables)
		const pylist &y,		// Response (dependent variables)
		size_t degree			// Polynomial degree
	) : TrigonometricRegression (to_vector (x), to_vector (y), degree)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Regression value for the target argument                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Regression (
		double x				// Value to calculate the regression for
	) const override final {
		const double regression = OrthogonalRegression::Regression (x);
		const double shift = funcs -> Convert (x) * coeff;
		return regression + shift;
	}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const TrigonometricRegression &object)
{
	stream << object.Summary ("Trigonometric regression (MSE)");
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
