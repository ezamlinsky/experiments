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
