/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                           BINARY SEARCH FUNCTIONS                            #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once

namespace Array
{
//****************************************************************************//
//      Binary search in sorted array                                         //
//****************************************************************************//
# define	BIN_SEARCH(func, cond, res)											\
template <typename T>															\
size_t func (																	\
	const T array[],															\
	size_t size,																\
	T value																		\
){																				\
	size_t left = 0;															\
	size_t right = size;														\
	while (left < right) {														\
		const size_t median = (left + right) / 2;								\
		if (array [median] cond value)											\
			left = median + 1;													\
		else																	\
			right = median;														\
	}																			\
	return (res);																\
}

// Find the greatest element that is still less than the target value
BIN_SEARCH(BinSearchLess, <, left - 1)

// Find the greatest element that is still less than or equal to the target value
BIN_SEARCH(BinSearchLessOrEqual, <=, left - 1)

// Find the smallest element that is still greater than or equal to the target value
BIN_SEARCH(BinSearchGreater, <=, left)

// Find the smallest element that is still greater than the target value
BIN_SEARCH(BinSearchGreaterOrEqual, <, left)
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
