/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#           SET OF ORTHOGONAL FUNCTIONS FOR AN ORTHOGONAL EXPANSION            #
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
	Range range;			// Range of original X values (for validation)
	mvector values;			// Original X values
	vector <mvector> funcs;	// Vector of the orthogonal functions
	double center;			// Center point (mean) of original X values
	double variation;		// Variation around the center of the original X values
	size_t size;			// Number of the X values
	size_t degree;			// Polynomial degree

//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Map a value to the orthogonality domain                               //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double Map (
		double x				// X value to map to the orthogonality domain
	) const {
		return (x - center) / variation;
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
		size_t degree			// Polynomial degree
	) :	range (x, size),
		values (x, size),
		center (Stats::Mean (x, size)),
		variation (max (range.Max() - center, center - range.Min())),
		size (size),
		degree (degree)
	{
		// Check if the requirement for a minimum size is satisfied
		if (size < 2)
			throw invalid_argument ("OrthogonalFunctions: The minimal polynomial size (nodes) is 2");

		// Check if the degree is properly specified
		if (degree > size)
			throw invalid_argument ("OrthogonalFunctions: Polynomial degree should not be greater than the data size");

		// Map the values to the orthogonality domain
		for (size_t i = 0; i < size; i++)
			x[i] = Map (x[i]);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Destructor                                                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual ~OrthogonalFunctions (void) = default;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Check the argument range and map to the orthogonality domain          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double Convert (
		double x				// X value to validate
	) const {

		// Outside the range of original X values
		if (x != range) {
			const string &arg = to_string (x);
			const string &min = to_string (range.Min());
			const string &max = to_string (range.Max());
			throw invalid_argument ("OrthogonalFunctions: Argument value '" + arg + "' is outside the original range [" + min + ","+ max +"]");
		}

		// Map the argument to the orthogonality domain
		return Map (x);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Vector of the orthogonal functions in the set                         //
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
//      Polynomial degree                                                     //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	size_t Degree (void) const {
		return degree;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Range of X values                                                     //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const Model::Range& Domain (void) const {
		return range;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      X values                                                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const mvector& Values (void) const {
		return values;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Number of the X values                                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	size_t Size (void) const {
		return size;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Virtual functions to override in derivative classes                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual mvector FuncValues (double x) const = 0;
};
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
