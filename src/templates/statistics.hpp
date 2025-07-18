/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                            STATISTICAL FUNCTIONS                             #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"array.hpp"

namespace Stats
{
//****************************************************************************//
//      Mean value                                                            //
//****************************************************************************//
template <typename T>
T Mean (
	const T array[],		// Array data
	size_t size				// Array size
){
	if (size)
		return Array::Sum (array, size) / size;
	else
		return NAN;
}

//****************************************************************************//
//      Mean value of deviations                                              //
//****************************************************************************//
# define	DEVIATION_POPULATION(func, sum)										\
template <typename T>															\
T func (																		\
	const T array[],															\
	size_t size,																\
	T value																		\
){																				\
	if (size > 0)																\
		return sum (array, size, value) / size;									\
	else																		\
		return NAN;																\
}
# define	DEVIATION_SAMPLE(func, sum)											\
template <typename T>															\
T func (																		\
	const T array[],															\
	size_t size,																\
	T value																		\
){																				\
	if (size > 1)																\
		return sum (array, size, value) / (size - 1);							\
	else																		\
		return NAN;																\
}

//============================================================================//
//      Mean value of squared deviations                                      //
//============================================================================//
DEVIATION_POPULATION(SqrDevPopulation, Array::SumSqrDiff)
DEVIATION_SAMPLE(SqrDevSample, Array::SumSqrDiff)

//============================================================================//
//      Mean value of absolute deviations                                     //
//============================================================================//
DEVIATION_POPULATION(AbsDevPopulation, Array::SumAbsDiff)
DEVIATION_SAMPLE(AbsDevSample, Array::SumAbsDiff)

//============================================================================//
//      Mean value of signed deviations                                       //
//============================================================================//
DEVIATION_POPULATION(SignDevPopulation, Array::SumSignDiff)
DEVIATION_SAMPLE(SignDevSample, Array::SumSignDiff)

//****************************************************************************//
//      Standard deviation                                                    //
//****************************************************************************//
# define	STD_DEV(func, sum)													\
template <typename T>															\
T func (																		\
	const T array[],															\
	size_t size,																\
	T value																		\
){																				\
	return sqrt (sum (array, size, value));										\
}
STD_DEV(StdDevPopulation, SqrDevPopulation)
STD_DEV(StdDevSample, SqrDevSample)

//****************************************************************************//
//      Standard error                                                        //
//****************************************************************************//
# define	STD_ERR(func, sum)													\
template <typename T>															\
T func (																		\
	const T array[],															\
	size_t size,																\
	T value																		\
){																				\
	if (size > 0)																\
		return sqrt (sum (array, size, value) / size);							\
	else																		\
		return NAN;																\
}
STD_ERR(StdErrPopulation, SqrDevPopulation)
STD_ERR(StdErrSample, SqrDevSample)

//****************************************************************************//
//      Variation                                                             //
//****************************************************************************//
# define	VARIATION(func, sum)												\
template <typename T>															\
T func (																		\
	const T array[],															\
	size_t size,																\
	T value																		\
){																				\
	return sum (array, size, value) / value;									\
}
VARIATION(VariationPopulation, StdDevPopulation)
VARIATION(VariationSample, StdDevSample)

//****************************************************************************//
//      Skewness                                                              //
//****************************************************************************//
template <typename T>
T SkewnessPopulation (
	const T array[],		// Array data
	size_t size,			// Array size
	T value					// The central value
){
	if (size > 0) {
		const T coeff = sqrt (size);
		const T temp1 = Array::SumCubeDiff (array, size, value);
		const T temp2 = Array::SumSqrDiff (array, size, value);
		return coeff * temp1 / (temp2 * sqrt (temp2));
	}
	else
		return NAN;
}
template <typename T>
T SkewnessSample (
	const T array[],		// Array data
	size_t size,			// Array size
	T value					// The central value
){
	if (size > 2) {
		const T coeff = sqrt (size - 1) * size / (size - 2);
		const T temp1 = Array::SumCubeDiff (array, size, value);
		const T temp2 = Array::SumSqrDiff (array, size, value);
		return coeff * temp1 / (temp2 * sqrt (temp2));
	}
	else
		return NAN;
}

//****************************************************************************//
//      Kurtosis                                                              //
//****************************************************************************//
template <typename T>
T KurtosisPopulation (
	const T array[],		// Array data
	size_t size,			// Array size
	T value					// The central value
){
	if (size > 0) {
		const T coeff = size;
		const T temp1 = Array::SumHcubeDiff (array, size, value);
		const T temp2 = Array::SumSqrDiff (array, size, value);
		return coeff * temp1 / (temp2 * temp2) - 3;
	}
	else
		return NAN;
}
template <typename T>
T KurtosisSample (
	const T array[],		// Array data
	size_t size,			// Array size
	T value					// The central value
){
	if (size > 3) {
		const T coeff1 = (size - 1) * size * (size + 1);
		const T coeff2 = (size - 1) * (size - 1);
		const T coeff3 = (size - 2) * (size - 3);
		const T temp1 = Array::SumHcubeDiff (array, size, value);
		const T temp2 = Array::SumSqrDiff (array, size, value);
		return (coeff1 * temp1 / (temp2 * temp2) - 3 * coeff2) / coeff3;
	}
	else
		return NAN;
}

//****************************************************************************//
//      Median value of signed deviations                                     //
//****************************************************************************//
template <typename T>
T MedianSignDev (
	const T array[],		// Array data
	size_t size,			// Array size
	T value					// The central value
){
	T *temp = new T[size];
	for (size_t i = 0; i < size; i++)
		temp[i] = array[i] - value;
	const T median = Array::Median (temp, size);
	delete [] temp;
	return median;
}

//****************************************************************************//
//      Median value of absolute deviations                                   //
//****************************************************************************//
template <typename T>
T MedianAbsDev (
	const T array[],		// Array data
	size_t size,			// Array size
	T value					// The central value
){
	T *temp = new T[size];
	for (size_t i = 0; i < size; i++)
		temp[i] = fabs (array[i] - value);
	const T median = Array::Median (temp, size);
	delete [] temp;
	return median;
}

//****************************************************************************//
//      Median value of squared deviations                                    //
//****************************************************************************//
template <typename T>
T MedianSqrDev (
	const T array[],		// Array data
	size_t size,			// Array size
	T value					// The central value
){
	T *temp = new T[size];
	for (size_t i = 0; i < size; i++) {
		const T val = array[i] - value;
		temp[i] = val * val;
	}
	const T median = Array::Median (temp, size);
	delete [] temp;
	return median;
}
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
