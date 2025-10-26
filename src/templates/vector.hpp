/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                                    VECTOR                                    #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	<stdexcept>
# include	"array.hpp"

// Exceptions
# define	UNIT_ERROR		"Index of a non-zero element of a unit vector is outside the vector size"
# define	ASSIGN_ERROR	"Assignment of different-sized vectors is an error"
# define	ADD_ERROR		"Addition of different-sized vectors is an error"
# define	SUB_ERROR		"Subtraction of different-sized vectors is an error"
# define	MUL_ERROR		"Multiplication of different-sized vectors is an error"
# define	DIV_ERROR		"Division of different-sized vectors is an error"
# define	PRODUCT_ERROR	"Dot product of different-sized vectors is an error"
# define	COMPARE_ERROR	"Comparison of different-sized vectors is an error"
# define	RANGE_ERROR		"Index value is outside the vector size"

//****************************************************************************//
//      Name space "Math"                                                     //
//****************************************************************************//
namespace Math
{

// Use shortenings
using namespace std;

//****************************************************************************//
//      Template of vector class                                              //
//****************************************************************************//
template <typename T>
class Vector
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	T *data;						// Vector coefficients
	size_t size;					// Vector size

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Default constructor                                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	explicit Vector (
		size_t size					// Vector size
	) :	size (size)
	{
		data = new T [size];
		Array::Init (data, size, 0.0);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors with initial value                                       //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Vector (
		T value,					// Vector coefficients
		size_t size					// Vector size
	) :	size (size)
	{
		data = new T [size];
		Array::Init (data, size, value);
	}

	Vector (
		const T array [],			// Vector coefficients
		size_t size					// Vector size
	) :	size (size)
	{
		data = new T [size];
		Array::Copy (data, array, size);
	}

	Vector (
		const vector <T> &array		// Vector coefficients
	) :	Vector (array.data(), array.size())
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Copy constructor                                                      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Vector (
		const Vector &source		// Source vector to copy values from
	) : Vector (source.data, source.size)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Move constructor                                                      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Vector (
		Vector &&source				// Source vector to move values from
	){
		data = source.data;
		size = source.size;
		source.data = nullptr;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Destructor                                                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	~Vector (void) {
		delete [] data;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Zero vector                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void Zero (void) {
		Array::Init (data, size, 0.0);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Unit vector                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void Unit (
		size_t index				// Position of a non-zero element
	){
		if (index >= size)
			throw invalid_argument ("Vector (Unit): " UNIT_ERROR);

		Array::Init (data, size, 0.0);
		data [index] = 1.0;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Vector norms                                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

	// Squared Euclidean norm
	T NormSqr (void) const {
		return Array::SumSqr (data, size);
	}

	// L1 space norm (Manhattan norm)
	T NormL1 (void) const {
		return Array::SumAbs (data, size);
	}

	// L2 space norm (Euclidean norm)
	T NormL2 (void) const {
		return sqrt (NormSqr());
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Unary operators                                                       //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

	// Inversion of the vector coefficients
	void Neg (void) {
		Array::Neg (data, size);
	}

	// The negative vector
	Vector operator- (void) const {
		Vector temp (*this);
		temp.Neg ();
		return temp;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Binary operators                                                      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

//----------------------------------------------------------------------------//
//      Assignment                                                            //
//----------------------------------------------------------------------------//
	Vector& operator= (
		const Vector &source		// Source vector to copy values from
	){
		if (this == &source)
			return *this;

		if (size != source.size)
			throw invalid_argument ("Vector (operator=): " ASSIGN_ERROR);

		Array::Copy (data, source.data, size);
		return *this;
	}

//----------------------------------------------------------------------------//
//      Addition                                                              //
//----------------------------------------------------------------------------//

	// Addition (short variant)
	Vector& operator+= (
		const Vector &source		// Source vector to add
	){
		if (size != source.size)
			throw invalid_argument ("Vector (operator+=): " ADD_ERROR);

		Array::Add (data, source.data, size);
		return *this;
	}

	// Addition (long variant)
	Vector operator+ (
		const Vector &source		// Source vector to add
	) const {
		Vector temp (*this);
		temp += source;
		return temp;
	}

	// Addition with a scale
	void Add (
		const Vector &source,		// Source vector to add
		T value						// Scalar value to use
	){
		if (size != source.size)
			throw invalid_argument ("Vector (Add): " ADD_ERROR);

		Array::ScaledAdd (data, source.data, size, value);
	}

//----------------------------------------------------------------------------//
//      Subtraction                                                           //
//----------------------------------------------------------------------------//

	// Subtraction (short variant)
	Vector& operator-= (
		const Vector &source		// Source vector to subtract
	){
		if (size != source.size)
			throw invalid_argument ("Vector (operator-=): " SUB_ERROR);

		Array::Sub (data, source.data, size);
		return *this;
	}

	// Subtraction (long variant)
	Vector operator- (
		const Vector &source		// Source vector to subtract
	) const {
		Vector temp (*this);
		temp -= source;
		return temp;
	}

	// Subtraction with a scale
	void Sub (
		const Vector &source,		// Source vector to add
		T value						// Scalar value to use
	){
		if (size != source.size)
			throw invalid_argument ("Vector (Sub): " SUB_ERROR);

		Array::ScaledSub (data, source.data, size, value);
	}

//----------------------------------------------------------------------------//
//      Multiplication                                                        //
//----------------------------------------------------------------------------//

	// Multiplication by a scalar (short variant)
	Vector& operator*= (
		T value						// Scalar value to use
	){
		Array::Mul (data, size, value);
		return *this;
	}

	// Multiplication by a scalar (long variants)
	Vector operator* (
		T value						// Scalar value to use
	){
		Vector temp (*this);
		temp *= value;
		return temp;
	}

	// Multiplication by a vector (short variant)
	Vector& operator*= (
		const Vector &source		// Vector to use
	){
		if (size != source.size)
			throw invalid_argument ("Vector (operator*=): " MUL_ERROR);

		Array::Mul (data, source.data, size);
		return *this;
	}

	// Multiplication by a vector (long variants)
	Vector operator* (
		const Vector &source		// Vector to use
	){
		Vector temp (*this);
		temp *= source;
		return temp;
	}

//----------------------------------------------------------------------------//
//      Division                                                              //
//----------------------------------------------------------------------------//

	// Division by a scalar (short variant)
	Vector& operator/= (
		T value						// Scalar value to use
	){
		Array::Div (data, size, value);
		return *this;
	}

	// Division by a scalar (short variants)
	Vector operator/ (
		T value						// Scalar value to use
	){
		Vector temp (*this);
		temp /= value;
		return temp;
	}

	// Division by a vector (short variant)
	Vector& operator/= (
		const Vector &source		// Vector to use
	){
		if (size != source.size)
			throw invalid_argument ("Vector (operator/=): " DIV_ERROR);

		Array::Div (data, source.data, size);
		return *this;
	}

	// Division by a vector (long variants)
	Vector operator/ (
		const Vector &source		// Vector to use
	){
		Vector temp (*this);
		temp /= source;
		return temp;
	}

//----------------------------------------------------------------------------//
//      Dot product                                                           //
//----------------------------------------------------------------------------//
	T DotProduct (
		const Vector &source		// Another vector to compute the dot product
	) const {
		if (size != source.size)
			throw invalid_argument ("Vector (operator*): " PRODUCT_ERROR);

		return Array::SumMul (data, source.data, size);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Logic operations                                                      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

	// Check for equality
	bool operator== (
		const Vector &source		// Source vector to compare with
	) const {
		if (size != source.size)
			throw invalid_argument ("Vector (operator==): " COMPARE_ERROR);

		return Array::IsEqual (data, source.data, size);
	}

	// Check for not equality
	bool operator!= (
		const Vector &source		// Source vector to compare with
	) const {
		if (size != source.size)
			throw invalid_argument ("Vector (operator!=): " COMPARE_ERROR);

		return Array::IsNotEqual (data, source.data, size);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Miscellaneous operations                                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

//----------------------------------------------------------------------------//
//      Coefficients                                                          //
//----------------------------------------------------------------------------//
	operator vector <T> (void) const {
		return vector <T> (data, data + size);
	}

//----------------------------------------------------------------------------//
//      Operator "[]"                                                         //
//----------------------------------------------------------------------------//

	const T& operator[] (
		size_t index				// The element index
	) const {
		if (index >= size)
			throw out_of_range ("Vector (operator[]): " RANGE_ERROR);

		return data [index];
	}

	T& operator[] (
		size_t index				// The element index
	){
		if (index >= size)
			throw out_of_range ("Vector (operator[]): " RANGE_ERROR);

		return data [index];
	}
};
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
