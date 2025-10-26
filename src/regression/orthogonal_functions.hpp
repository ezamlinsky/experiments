/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#         SET OF ORTHOGONAL FUNCTIONS FOR AN ORTHOGONAL DECOMPOSITION          #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	<stdexcept>
# include	"../templates/statistics.hpp"
# include	"../templates/vector.hpp"
# include	"../models/range.hpp"

// Use shortenings
using namespace std;
using namespace Model;
using namespace Math;
using mvector = Vector <double>;

//****************************************************************************//
//      Class "OrthogonalFunctions"                                           //
//****************************************************************************//
class OrthogonalFunctions
{
//============================================================================//
//      Members                                                               //
//============================================================================//
protected:

	Range range;			// Original range of X values (for validation)
	mvector args;			// X values are mapped to the orthogonality domain
	vector <mvector> funcs;	// Vector of the orthogonal functions
	double center;			// Center point (mean) of original X values
	double variation;		// Variation around the center of the original X values
	double domain;			// Range of the symmetrical orthogonality domain
	size_t size;			// Number of the X values
	size_t degree;			// Polynomial degree

//============================================================================//
//      Protected methods                                                     //
//============================================================================//
protected:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Map a value to the orthogonality domain                               //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double ToOrthogonalityDomain (
		double x				// X value to map to the orthogonality domain
	) const {

		// Outside the original range of X values
		if (x != range) {
			const string &arg = to_string (x);
			const string &min = to_string (range.Min());
			const string &max = to_string (range.Max());
			throw invalid_argument ("OrthogonalFunctions: Argument value '" + arg + "' is outside the original range [" + min + ","+ max +"]");
		}

		// Map the value to the orthogonality domain
		return domain * (x - center) / variation;
	}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	OrthogonalFunctions (
		double x[],				// X values to map to the orthogonality domain
		size_t size,			// Number of the X values
		size_t degree,			// Polynomial degree
		double domain			// Range of the symmetrical orthogonality domain
	) :	range (x, size),
		args (size),
		center (Stats::Mean (x, size)),
		variation (max (range.Max() - center, center - range.Min())),
		domain (domain),
		size (size),
		degree (degree)
	{
		// Check if the degree is properly specified
		if (degree > size)
			throw invalid_argument ("OrthogonalFunctions: Polynomial degree should not be greater than the data size");

		// Map the values to the orthogonality domain
		for (size_t i = 0; i < size; i++)
			x[i] = ToOrthogonalityDomain (x[i]);

		// Convert them to a vector for the evaluation of the polynomials
		args = mvector (x, size);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Destructor                                                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual ~OrthogonalFunctions (void) = default;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Values of the orthogonal functions for the X values                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const vector <mvector>& Functions (void) const {
		return funcs;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Number of the orthogonal functions in the set                         //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	size_t Count (void) const {
		return funcs.size();
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Number of the X values                                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	size_t Size (void) const {
		return size;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Polynomial degree                                                     //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	size_t Degree (void) const {
		return degree;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Virtual functions to override in derivative classes                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual mvector Values (double x) const = 0;
};
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
