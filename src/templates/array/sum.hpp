/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                            NUMERICAL INTEGRATION                             #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once

namespace Array
{
//****************************************************************************//
//      Sums of elements                                                      //
//****************************************************************************//

//============================================================================//
//      Sum of values                                                         //
//============================================================================//
template <typename T>
T Sum (
	const T array[],		// Array data
	size_t size				// Array size
){
	T sum = 0;
	for (size_t i = 0; i < size; i++)
		sum += array[i];
	return sum;
}

//============================================================================//
//      Sum of absolute values                                                //
//============================================================================//
template <typename T>
T SumAbs (
	const T array[],		// Array data
	size_t size				// Array size
){
	T sum = 0;
	for (size_t i = 0; i < size; i++)
		sum += fabs (array[i]);
	return sum;
}

//============================================================================//
//      Sum of squared values                                                 //
//============================================================================//
template <typename T>
T SumSqr (
	const T array[],		// Array data
	size_t size				// Array size
){
	T sum = 0;
	for (size_t i = 0; i < size; i++)
		sum += array[i] * array[i];
	return sum;
}

//============================================================================//
//      Sum of positive and negative values                                   //
//============================================================================//
template <typename T>
void Sum (
	T &pos_sum,				// Sum of positive values
	T &neg_sum,				// Sum of negative values
	size_t &pos_cnt,		// Count of positive values
	size_t &neg_cnt,		// Count of negative values
	const T array[],		// Array data
	size_t size				// Array size
){
	pos_sum = 0;
	neg_sum = 0;
	pos_cnt = 0;
	neg_cnt = 0;
	for (size_t i = 0; i < size; i++)
		if (array[i] >= 0) {
			pos_sum += array[i];
			pos_cnt++;
		}
		else {
			neg_sum += array[i];
			neg_cnt++;
		}
}

//****************************************************************************//
//      Sums of differences                                                   //
//****************************************************************************//

//============================================================================//
//      Sum of signed differences                                             //
//============================================================================//

// Scalar operation
template <typename T>
T SumSignDiff (
	const T array[],		// Array data
	size_t size,			// Array size
	T value					// The value to find a difference
){
	T sum = 0;
	for (size_t i = 0; i < size; i++)
		sum += array[i] - value;
	return sum;
}

// Vector operation
template <typename T>
T SumSignDiff (
	const T array1[],		// The first array
	const T array2[],		// The second array
	size_t size				// Array size
){
	T sum = 0;
	for (size_t i = 0; i < size; i++)
		sum += array1[i] - array2[i];
	return sum;
}

//============================================================================//
//      Sum of absolute differences                                           //
//============================================================================//

// Scalar operation
template <typename T>
T SumAbsDiff (
	const T array[],		// Array data
	size_t size,			// Array size
	T value					// The value to find a difference
){
	T sum = 0;
	for (size_t i = 0; i < size; i++)
		sum += fabs (array[i] - value);
	return sum;
}

// Vector operation
template <typename T>
T SumAbsDiff (
	const T array1[],		// The first array
	const T array2[],		// The second array
	size_t size				// Array size
){
	T sum = 0;
	for (size_t i = 0; i < size; i++)
		sum += fasb (array1[i] - array2[i]);
	return sum;
}

//============================================================================//
//      Sum of squared differences                                            //
//============================================================================//

// Scalar operation
template <typename T>
T SumSqrDiff (
	const T array[],		// Array data
	size_t size,			// Array size
	T value					// The value to find a difference
){
	T sum = 0;
	for (size_t i = 0; i < size; i++) {
		const T diff = array[i] - value;
		sum += diff * diff;
	}
	return sum;
}

// Vector operation
template <typename T>
T SumSqrDiff (
	const T array1[],		// The first array
	const T array2[],		// The second array
	size_t size				// Array size
){
	T sum = 0;
	for (size_t i = 0; i < size; i++) {
		const T diff = array1[i] - array2[i];
		sum += diff * diff;
	}
	return sum;
}

//============================================================================//
//      Sum of cube differences                                               //
//============================================================================//

// Scalar operation
template <typename T>
T SumCubeDiff (
	const T array[],		// Array data
	size_t size,			// Array size
	T value					// The value to find a difference
){
	T sum = 0;
	for (size_t i = 0; i < size; i++) {
		const T diff = array[i] - value;
		sum += diff * diff * diff;
	}
	return sum;
}

//============================================================================//
//      Sum of hypercube differences                                          //
//============================================================================//

// Scalar operation
template <typename T>
T SumHcubeDiff (
	const T array[],		// Array data
	size_t size,			// Array size
	T value					// The value to find a difference
){
	T sum = 0;
	for (size_t i = 0; i < size; i++) {
		const T diff = array[i] - value;
		const T temp = diff * diff;
		sum += temp * temp;
	}
	return sum;
}

//============================================================================//
//      Sum of multiplied differences                                         //
//============================================================================//
template <typename T>
T SumMulDiff (
	const T array1[],		// The first array
	T value1,				// Mean value of the first array
	const T array2[],		// The second array
	T value2,				// Mean value of the second array
	size_t size				// Array size
){
	T sum = 0;
	for (size_t i = 0; i < size; i++) {
		const T diff1 = array1[i] - value1;
		const T diff2 = array2[i] - value2;
		sum += diff1 * diff2;
	}
	return sum;
}

//****************************************************************************//
//      Sum of multiplied values (dot product)                                //
//****************************************************************************//

// Without weight coefficients
template <typename T>
T SumMul (
	const T array1[],		// The first array
	const T array2[],		// The second array
	size_t size				// Array size
){
	T sum = 0;
	for (size_t i = 0; i < size; i++)
		sum += array1[i] * array2[i];
	return sum;
}

// With weight coefficients
template <typename T>
T SumMul (
	const T array1[],		// The first array
	const T array2[],		// The second array
	const T weights[],		// Weight coefficients
	size_t size				// Array size
){
	T sum = 0;
	for (size_t i = 0; i < size; i++)
		sum += array1[i] * array2[i] * weights[i];
	return sum;
}
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
