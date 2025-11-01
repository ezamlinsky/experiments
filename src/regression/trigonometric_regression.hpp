/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#        TRIGONOMETRIC REGRESSION BY ORTHOGONAL TRIGONOMETRIC FUNCTIONS        #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
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
	) : OrthogonalFunctions (x, size, degree, M_PI)
	{
		// The first function is an average value
		funcs.push_back (mvector (1.0, size));

		// Compute sines and cosines for different multiplicity factors
		double cos_vals [size];
		double sin_vals [size];
		for (size_t j = 1; j <= degree; j++) {
			for (size_t i = 0; i < size; i++) {
				cos_vals[i] = cos (j * x[i]);
				sin_vals[i] = sin (j * x[i]);
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
			vals.push_back (cos (j * x));
			vals.push_back (sin (j * x));
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
//      Private methods                                                       //
//============================================================================//
private:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Adjust values for the approximation                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	static void AdjustValues (
		mvector &data,			// Values to correct with a linear correction algorithm
		double coeff			// Coefficient for the linear correction
	){
		for (size_t i = 0; i < data.Size(); i++)
			data [i] += coeff * i;
	}

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
		// Adjust X values before the approximation
		const double coeff = (y[size - 1] - y[0]) / (size - 1);
		AdjustValues (residuals, -coeff);

		// Approximate the dependent variables by the regression
		Approximate();

		// Adjust the regression values after the approximation
		AdjustValues (approx, +coeff);
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
