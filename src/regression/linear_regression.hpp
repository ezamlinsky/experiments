/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                 CLASSICAL LINEAR REGRESSION OVER THE DATASET                 #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# include	"legendre_regression.hpp"

//****************************************************************************//
//      Class "LinearRegression"                                              //
//****************************************************************************//
class LinearRegression : public OrthogonalRegression
{
//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Internal constructor                                                  //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	LinearRegression (
		double x[],				// Predictors (independent variables)
		double y[],				// Response (dependent variables)
		size_t size				// Size of the dataset
	) :	OrthogonalRegression (y, new LegendrePolynomials (Sort (x, y, size), size, 0))
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
	LinearRegression (
		vector <double> x,		// Predictors (independent variables)
		vector <double> y		// Response (dependent variables)
	) :	LinearRegression (x.data(), y.data(), min (x.size(), y.size()))
	{}

	LinearRegression (
		const pylist &x,		// Predictors (independent variables)
		const pylist &y			// Response (dependent variables)
	) : LinearRegression (to_vector (x), to_vector (y))
	{}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const LinearRegression &object)
{
	stream << object.Summary ("Linear regression (MSE)");
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
