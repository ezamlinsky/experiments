/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                              BINARY OPERATIONS                               #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once

namespace Array
{
//****************************************************************************//
//      Scalar arithmetic operations                                          //
//****************************************************************************//

//============================================================================//
//      In-place operations                                                   //
//============================================================================//

// Forward operation
# define	SCALAR_INPLACE_FORWARD(fname, operation)							\
template <typename T>															\
T* fname (																		\
	T array[],																	\
	size_t size,																\
	T value																		\
){																				\
	for (size_t i = 0; i < size; i++)											\
		array[i] operation value;												\
	return array;																\
}

// Reverse operation
# define	SCALAR_INPLACE_REVERSE(fname, operation)							\
template <typename T>															\
T* fname (																		\
	T array[],																	\
	size_t size,																\
	T value																		\
){																				\
	for (size_t i = 0; i < size; i++)											\
		array[i] = value operation array[i];									\
	return array;																\
}

//============================================================================//
//      Normal operations                                                     //
//============================================================================//

// Forward operation
# define	SCALAR_NORMAL_FORWARD(fname, operation)								\
template <typename T>															\
T* fname (																		\
	T result[],																	\
	const T source[],															\
	size_t size,																\
	T value																		\
){																				\
	for (size_t i = 0; i < size; i++)											\
		result[i] = source[i] operation value;									\
	return result;																\
}

// Reverse operation
# define	SCALAR_NORMAL_REVERSE(fname, operation)								\
template <typename T>															\
T* fname (																		\
	T result[],																	\
	const T source[],															\
	size_t size,																\
	T value																		\
){																				\
	for (size_t i = 0; i < size; i++)											\
		result[i] = value operation source[i];									\
	return result;																\
}

//****************************************************************************//
//      Vector arithmetic operations                                          //
//****************************************************************************//

//============================================================================//
//      In-place operations                                                   //
//============================================================================//

// Forward operation
# define	VECTOR_INPLACE_FORWARD(fname, operation)							\
template <typename T>															\
T* fname																		\
(																				\
	T array[],																	\
	const T source[],															\
	size_t size																	\
){																				\
	for (size_t i = 0; i < size; i++)											\
		array[i] operation source[i];											\
	return array;																\
}

// Reverse operation
# define	VECTOR_INPLACE_REVERSE(fname, operation)							\
template <typename T>															\
T* fname																		\
(																				\
	T array[],																	\
	const T source[],															\
	size_t size																	\
){																				\
	for (size_t i = 0; i < size; i++)											\
		array[i] = source[i] operation array[i];								\
	return array;																\
}

//============================================================================//
//      Normal operations                                                     //
//============================================================================//

// Forward operation
# define	VECTOR_NORMAL_FORWARD(fname, operation)								\
template <typename T>															\
T* fname																		\
(																				\
	T result[],																	\
	const T source1[],															\
	const T source2[],															\
	size_t size																	\
){																				\
	for (size_t i = 0; i < size; i++)											\
		result[i] = source1[i] operation source2[i];							\
	return result;																\
}

// Reverse operation
# define	VECTOR_NORMAL_REVERSE(fname, operation)								\
template <typename T>															\
T* fname																		\
(																				\
	T result[],																	\
	const T source1[],															\
	const T source2[],															\
	size_t size																	\
){																				\
	for (size_t i = 0; i < size; i++)											\
		result[i] = source2[i] operation source1[i];							\
	return result;																\
}

//****************************************************************************//
//      Scaled arithmetic operations                                          //
//****************************************************************************//

// In-place operation
# define	SCALED_INPLACE(fname, operation)									\
template <typename T>															\
T* fname																		\
(																				\
	T array[],																	\
	const T source[],															\
	size_t size,																\
	T value																		\
){																				\
	for (size_t i = 0; i < size; i++)											\
		array[i] operation source[i] * value;									\
	return array;																\
}

// Normal operation
# define	SCALED_NORMAL(fname, operation)										\
template <typename T>															\
T* fname																		\
(																				\
	T result[],																	\
	const T source1[],															\
	const T source2[],															\
	size_t size,																\
	T value																		\
){																				\
	for (size_t i = 0; i < size; i++)											\
		result[i] = source1[i] operation source2[i] * value;					\
	return result;																\
}


//****************************************************************************//
//      Initialization                                                        //
//****************************************************************************//
SCALAR_INPLACE_FORWARD(Init, =)

//****************************************************************************//
//      Copying arrays                                                        //
//****************************************************************************//
VECTOR_INPLACE_FORWARD(Copy, =)

//****************************************************************************//
//      Addition                                                              //
//****************************************************************************//

// Scalar addition
SCALAR_INPLACE_FORWARD(Add, +=)
SCALAR_NORMAL_FORWARD(Add, +)

// Vector addition
VECTOR_INPLACE_FORWARD(Add, +=)
VECTOR_NORMAL_FORWARD(Add, +)

//****************************************************************************//
//      Scaled addition                                                       //
//****************************************************************************//

// In-place operation
SCALED_INPLACE(ScaledAdd, +=)

// Normal operation
SCALED_NORMAL(ScaledAdd, +)

//****************************************************************************//
//      Subtraction                                                           //
//****************************************************************************//

// Scalar subtraction
SCALAR_INPLACE_FORWARD(Sub, -=)
SCALAR_NORMAL_FORWARD(Sub, -)

// Vector subtraction
VECTOR_INPLACE_FORWARD(Sub, -=)
VECTOR_NORMAL_FORWARD(Sub, -)

//****************************************************************************//
//      Reverse subtraction                                                   //
//****************************************************************************//

// Scalar reverse subtraction
SCALAR_INPLACE_REVERSE(ReverseSub, -)
SCALAR_NORMAL_REVERSE(ReverseSub, -)

// Vector reverse subtraction
VECTOR_INPLACE_REVERSE(ReverseSub, -)
VECTOR_NORMAL_REVERSE(ReverseSub, -)

//****************************************************************************//
//      Scaled subtraction                                                    //
//****************************************************************************//

// In-place operation
SCALED_INPLACE(ScaledSub, -=)

// Normal operation
SCALED_NORMAL(ScaledSub, -)

//****************************************************************************//
//      Multiplication                                                        //
//****************************************************************************//

// Scalar multiplication
SCALAR_INPLACE_FORWARD(Mul, *=)
SCALAR_NORMAL_FORWARD(Mul, *)

// Vector multiplication
VECTOR_INPLACE_FORWARD(Mul, *=)
VECTOR_NORMAL_FORWARD(Mul, *)

//****************************************************************************//
//      Division                                                              //
//****************************************************************************//

// Scalar division
SCALAR_INPLACE_FORWARD(Div, /=)
SCALAR_NORMAL_FORWARD(Div, /)

// Vector division
VECTOR_INPLACE_FORWARD(Div, /=)
VECTOR_NORMAL_FORWARD(Div, /)

//****************************************************************************//
//      Reverse division                                                      //
//****************************************************************************//

// Scalar reverse division
SCALAR_INPLACE_REVERSE(ReverseDiv, /)
SCALAR_NORMAL_REVERSE(ReverseDiv, /)

// Vector reverse division
VECTOR_INPLACE_REVERSE(ReverseDiv, /)
VECTOR_NORMAL_REVERSE(ReverseDiv, /)

//****************************************************************************//
//      Power                                                                 //
//****************************************************************************//
template <typename T>
T* Pow (
	T result[],
	const T source[],
	size_t size,
	T power
){
	for (size_t i = 0; i < size; i++)
		result[i] = pow (source[i], power);
	return result;
}

//****************************************************************************//
//      Check for equality                                                    //
//****************************************************************************//
template <typename T>
bool IsEqual (
	const T source1[],
	const T source2[],
	size_t size
){
	for (size_t i = 0; i < size; i++)
		if (source1[i] != source2[i])
			return false;
	return true;
}

//****************************************************************************//
//      Check for not equality                                                //
//****************************************************************************//
template <typename T>
bool IsNotEqual (
	const T source1[],
	const T source2[],
	size_t size
){
	for (size_t i = 0; i < size; i++)
		if (source1[i] != source2[i])
			return true;
	return false;
}
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
