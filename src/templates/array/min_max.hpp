/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                              MIN AND MAX VALUES                              #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once

namespace Array
{
//****************************************************************************//
//      Range                                                                 //
//****************************************************************************//
template <typename T>
T Range (
	const T array[],		// Array data
	size_t size				// Array size
){
	using namespace std;
	if (size) {
		auto bounds = minmax_element (array, array + size);
		return *bounds.second - *bounds.first;
	}
	else
		return 0;
}

//****************************************************************************//
//      Minimum and maximum values                                            //
//****************************************************************************//

//============================================================================//
//      Minimum value                                                         //
//============================================================================//
template <typename T>
T Min (
	const T array[],		// Array data
	size_t size				// Array size
){
	using namespace std;
	if (size)
		return *min_element (array, array + size);
	else
		return NAN;
}

//============================================================================//
//      Maximum value                                                         //
//============================================================================//
template <typename T>
T Max (
	const T array[],		// Array data
	size_t size				// Array size
){
	using namespace std;
	if (size)
		return *max_element (array, array + size);
	else
		return NAN;
}

//============================================================================//
//      Both values                                                           //
//============================================================================//
template <typename T>
void MinMax (
	T &min,					// Where to return the min value
	T &max,					// Where to return the max value
	const T array[],		// Array data
	size_t size				// Array size
){
	using namespace std;
	if (size) {
		auto bounds = minmax_element (array, array + size);
		min = *bounds.first;
		max = *bounds.second;
	}
	else {
		min = NAN;
		max = NAN;
	}
}

//****************************************************************************//
//      Minimum and maximum absolute values                                   //
//****************************************************************************//
template <typename T>
inline bool cmp_abs (
	const T& a,				// The first value
	const T& b				// The second value
){
	return fabs (a) < fabs (b);
}

//============================================================================//
//      Minimum absolute value                                                //
//============================================================================//
template <typename T>
T MinAbs (
	const T array[],		// Array data
	size_t size				// Array size
){
	using namespace std;
	if (size)
		return fabs (*min_element (array, array + size, cmp_abs<T>));
	else
		return NAN;
}

//============================================================================//
//      Maximum absolute value                                                //
//============================================================================//
template <typename T>
T MaxAbs (
	const T array[],		// Array data
	size_t size				// Array size
){
	using namespace std;
	if (size)
		return fabs (*max_element (array, array + size, cmp_abs<T>));
	else
		return NAN;
}

//============================================================================//
//      Both values                                                           //
//============================================================================//
template <typename T>
void MinMaxAbs (
	T &min,					// Where to return the min value
	T &max,					// Where to return the max value
	const T array[],		// Array data
	size_t size				// Array size
){
	using namespace std;
	if (size) {
		auto bounds = minmax_element (array, array + size, cmp_abs<T>);
		min = fabs (*bounds.first);
		max = fabs (*bounds.second);
	}
	else {
		min = NAN;
		max = NAN;
	}
}
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
