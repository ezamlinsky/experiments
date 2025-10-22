/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                       MEDIAN VALUE AND SORT FUNCTIONS                        #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once

namespace Array
{
//****************************************************************************//
//      QuickSort                                                             //
//****************************************************************************//
template <typename T>
void Sort (
	T array[],				// Array data
	size_t size				// Array size
){
	using namespace std;
	sort (array, array + size);
}

//****************************************************************************//
//      Median value                                                          //
//****************************************************************************//

//============================================================================//
//      Of sorted array                                                       //
//============================================================================//
template <typename T>
T MedianSorted (
	const T array[],		// Array data
	size_t size				// Array size
){
	if (size > 0) {
		const size_t middle = size / 2;
		if (size % 2)
			return array [middle];
		else
			return (array [middle] + array [middle - 1]) / 2;
	}
	else
		return NAN;
}

//============================================================================//
//      Of non constant array                                                 //
//============================================================================//
template <typename T>
T Median (
	T array[],				// Array data
	size_t size				// Array size
){
	if (size > 1) {
		const size_t middle = size / 2;
		nth_element (array, array + middle, array + size);
		nth_element (array, array + middle - 1, array + size);
	}
	return MedianSorted (array, size);
}

//============================================================================//
//      Of constant array                                                     //
//============================================================================//
template <typename T>
T Median (
	const T array[],		// Array data
	size_t size				// Array size
){
	T *temp = new T [size];
	Array::Copy (temp, array, size);
	const T median = Median (temp, size);
	delete [] temp;
	return median;
}
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
