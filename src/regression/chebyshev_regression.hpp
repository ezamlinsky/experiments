/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#          POLYNOMIAL REGRESSION BY ORTHOGONAL CHEBYSHEV POLYNOMIALS           #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# include	"orthogonal_regression.hpp"
# include	"../python_helpers.hpp"

//****************************************************************************//
//      Class "ChebyshevPolynomials"                                           //
//****************************************************************************//
class ChebyshevPolynomials : public OrthogonalFunctions
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	mvector weights;			// Weight coefficients

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
			*p0 = arg * *p1 * 2 - *p2;

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
	ChebyshevPolynomials (
		double x[],				// X values
		size_t size,			// Number of the X values
		size_t degree			// Polynomial degree
	) : OrthogonalFunctions (x, size, degree, 0.99),
		weights (size)
	{
		// Calculate the weight coefficients
		for (size_t i = 0; i < size; i++)
			weights[i] = 1.0 / (sqrt (1 - x[i]) * sqrt (1 + x[i]));

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
//      Squared Euclidean norm of the orthogonal function                     //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double NormSqr (
		size_t index			// Index of the function
	) const override final {
		return funcs.at (index).NormSqr (weights);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Dot product of the orthogonal function                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double DotProduct (
		size_t index,			// Index of the function
		const mvector &vector	// Another vector to compute the dot product
	) const override final {
		return funcs.at (index).DotProduct (vector, weights);
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
//      Class "ChebyshevRegression"                                            //
//****************************************************************************//
class ChebyshevRegression : public OrthogonalRegression
{
//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Internal constructor                                                  //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	ChebyshevRegression (
		double x[],				// Predictors (independent variables)
		double y[],				// Response (dependent variables)
		size_t size,			// Size of the dataset
		size_t degree			// Polynomial degree
	) :	OrthogonalRegression (y, new ChebyshevPolynomials (Sort (x, y, size), size, degree))
	{
		// Approximate the dependent variables by the regression
		Approximate ();
	}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors                                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	ChebyshevRegression (
		vector <double> x,		// Predictors (independent variables)
		vector <double> y,		// Response (dependent variables)
		size_t degree			// Polynomial degree
	) :	ChebyshevRegression (x.data(), y.data(), min (x.size(), y.size()), degree)
	{}

	ChebyshevRegression (
		const pylist &x,		// Predictors (independent variables)
		const pylist &y,		// Response (dependent variables)
		size_t degree			// Polynomial degree
	) : ChebyshevRegression (to_vector (x), to_vector (y), degree)
	{}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const ChebyshevRegression &object)
{
	stream << object.Summary ("Chebyshev regression (MSE)");
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
