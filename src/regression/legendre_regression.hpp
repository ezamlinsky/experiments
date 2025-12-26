/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#           POLYNOMIAL REGRESSION BY ORTHOGONAL LEGENDRE POLYNOMIALS           #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"orthogonal_regression.hpp"
# include	"../python_helpers.hpp"

//****************************************************************************//
//      Class "LegendrePolynomials"                                           //
//****************************************************************************//
class LegendrePolynomials : public OrthogonalFunctions
{
//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Evaluate recurrent polynomials for the target X value                 //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	template <typename T>
	static vector <T> RecurrentPolynomials (
		T arg,					// Argument (X values)
		T temp,					// Temporary buffer for the recurrent algorithm
		T poly0,				// 0 degree polynomial
		T poly1,				// 1 degree polynomial
		size_t degree			// Polynomial degree
	){
		// Set of all the orthogonal polynomials
		vector <T> res;

		// Add the initial polynomials to the set
		res.push_back (poly0);
		res.push_back (poly1);

		// Evaluate the next recurrent polynomials until the target degree is achieved
		T *p2 = &poly0;
		T *p1 = &poly1;
		T *p0 = &temp;
		for (size_t j = 2; j <= degree; j++) {

			// Evaluate the polynomial value
			*p0 = (arg * *p1 * (2 * j - 1) - *p2 * (j - 1)) / j;

			// Add it to the set
			res.push_back (*p0);

			// Swap the buffers for the next round
			T *ptr = p2;
			p2 = p1;
			p1 = p0;
			p0 = ptr;
		}

		// Return the set of orthogonal polynomials to the caller
		return res;
	}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	LegendrePolynomials (
		double x[],				// X values
		size_t size,			// Number of the X values
		size_t degree			// Polynomial degree
	) : OrthogonalFunctions (x, size, degree)
	{
		// Initial polynomials for the recurrent polynomials evaluation procedure
		mvector poly (size);
		mvector poly0 (1.0, size);
		mvector poly1 (x, size);
		mvector args (x, size);

		// Store all the orthogonal polynomials as the orthogonal basis
		vector <mvector> polynomials = RecurrentPolynomials (args, poly, poly0, poly1, degree);
		for (auto &function : polynomials)
			funcs.push_back (function);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Calculate values of all the orthogonal polynomials for the target X   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual mvector FuncValues (
		double x				// Value to calculate the functions for
	) const override final {

		// Check the argument range and map to the orthogonality domain
		x = Convert (x);

		// Evaluate recurrent polynomials for the target X value
		return RecurrentPolynomials (x, 0.0, 1.0, x, degree);
	}
};

//****************************************************************************//
//      Class "LegendreRegression"                                            //
//****************************************************************************//
class LegendreRegression : public OrthogonalRegression
{
//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Internal constructor                                                  //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	LegendreRegression (
		double x[],				// Predictors (independent variables)
		double y[],				// Response (dependent variables)
		size_t size,			// Size of the dataset
		size_t degree			// Polynomial degree
	) :	OrthogonalRegression (y, new LegendrePolynomials (Sort (x, y, size), size, degree))
	{
		// Approximate the dependent variables by the regression
		Approximate();
	}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors                                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	LegendreRegression (
		vector <double> x,		// Predictors (independent variables)
		vector <double> y,		// Response (dependent variables)
		size_t degree			// Polynomial degree
	) :	LegendreRegression (x.data(), y.data(), min (x.size(), y.size()), degree)
	{}

	LegendreRegression (
		const pylist &x,		// Predictors (independent variables)
		const pylist &y,		// Response (dependent variables)
		size_t degree			// Polynomial degree
	) : LegendreRegression (to_vector (x), to_vector (y), degree)
	{}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const LegendreRegression &object)
{
	stream << object.Summary ("Legendre regression (MSE)");
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
