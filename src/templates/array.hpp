/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                               ARRAY FUNCTIONS                                #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# ifdef	__AVX__
	# define	SIMD_VSIZE	32
# else
	# define	SIMD_VSIZE	16
# endif
# include	<cmath>
# include	<algorithm>

namespace Array
{
//****************************************************************************//
//      Blend two vectors                                                     //
//****************************************************************************//

//============================================================================//
//      Double precision vectors                                              //
//============================================================================//
typedef double vdbl __attribute__ ((
	vector_size (SIMD_VSIZE),
	aligned (SIMD_VSIZE)
));
inline vdbl blend_vectors (
	vdbl src1,
	vdbl src2,
	int count
){
	typedef int64_t ivect __attribute__ ((
		vector_size (SIMD_VSIZE),
		aligned (SIMD_VSIZE)
	));
# ifdef	__AVX__
	ivect indices = {0, 1, 2, 3};
# else
	ivect indices = {0, 1};
# endif
	ivect mask = indices < count;
	return mask ? src1 : src2;
}

//============================================================================//
//      Single precision vectors                                              //
//============================================================================//
typedef float vflt __attribute__ ((
	vector_size (SIMD_VSIZE),
	aligned (SIMD_VSIZE)
));
inline vflt blend_vectors (
	vflt a,
	vflt b,
	int count
){
	typedef int32_t ivect __attribute__ ((
		vector_size (SIMD_VSIZE),
		aligned (SIMD_VSIZE)
	));
# ifdef	__AVX__
	ivect indices = {0, 1, 2, 3, 4, 5, 6, 7};
# else
	ivect indices = {0, 1, 2, 3};
# endif
	ivect mask = indices < count;
	return mask ? a : b;
}
}

//****************************************************************************//
//      Other functions                                                       //
//****************************************************************************//
# include	"array/unary.hpp"
# include	"array/binary.hpp"
# include	"array/min_max.hpp"
# include	"array/bin_search.hpp"
# include	"array/sum.hpp"
# include	"array/median.hpp"
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
