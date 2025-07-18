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
void fname (																	\
	T array[],																	\
	size_t size,																\
	T value																		\
){																				\
	for (size_t i = 0; i < size; i++)											\
		array[i] operation value;												\
}

// Reverse operation
# define	SCALAR_INPLACE_REVERSE(fname, operation)							\
template <typename T>															\
void fname (																	\
	T array[],																	\
	size_t size,																\
	T value																		\
){																				\
	for (size_t i = 0; i < size; i++)											\
		array[i] = value operation array[i];									\
}

//============================================================================//
//      Normal operations                                                     //
//============================================================================//

// Forward operation
# define	SCALAR_NORMAL_FORWARD(fname, operation)								\
template <typename T>															\
void fname (																	\
	T result[],																	\
	const T source[],															\
	size_t size,																\
	T value																		\
){																				\
	for (size_t i = 0; i < size; i++)											\
		result[i] = source[i] operation value;									\
}

// Reverse operation
# define	SCALAR_NORMAL_REVERSE(fname, operation)								\
template <typename T>															\
void fname (																	\
	T result[],																	\
	const T source[],															\
	size_t size,																\
	T value																		\
){																				\
	for (size_t i = 0; i < size; i++)											\
		result[i] = value operation source[i];									\
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
void fname																		\
(																				\
	T array[],																	\
	const T source[],															\
	size_t size																	\
){																				\
	for (size_t i = 0; i < size; i++)											\
		array[i] operation source[i];											\
}

// Reverse operation
# define	VECTOR_INPLACE_REVERSE(fname, operation)							\
template <typename T>															\
void fname																		\
(																				\
	T array[],																	\
	const T source[],															\
	size_t size																	\
){																				\
	for (size_t i = 0; i < size; i++)											\
		array[i] = source[i] operation array[i];								\
}

//============================================================================//
//      Normal operations                                                     //
//============================================================================//

// Forward operation
# define	VECTOR_NORMAL_FORWARD(fname, operation)								\
template <typename T>															\
void fname																		\
(																				\
	T result[],																	\
	const T source1[],															\
	const T source2[],															\
	size_t size																	\
){																				\
	for (size_t i = 0; i < size; i++)											\
		result[i] = source1[i] operation source2[i];							\
}

// Reverse operation
# define	VECTOR_NORMAL_REVERSE(fname, operation)								\
template <typename T>															\
void fname																		\
(																				\
	T result[],																	\
	const T source1[],															\
	const T source2[],															\
	size_t size																	\
){																				\
	for (size_t i = 0; i < size; i++)											\
		result[i] = source2[i] operation source1[i];							\
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
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
