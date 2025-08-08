/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                     CALCULATE INCOMPLETE GAMMA FUNCTIONS                     #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	<cmath>

//****************************************************************************//
//      Normalized upper incomplete gamma function                            //
//****************************************************************************//
double NormalizedUpperIncompleteGamma (
	double arg,					// Argument value
	size_t count				// Iterations count
){
	if (arg <= 0.0)
		throw invalid_argument ("NormalizedUpperIncompleteGamma: The argument value must be positive");

	const double arg_log = log (arg);
	double sum = 0.0;
	for (size_t i = 0; i < count; i++) {
		const double temp = i * arg_log - arg - lgamma (i + 1.0);
		sum += exp (temp);
	}
	return sum;
}
//****************************************************************************//
//      Normalized lower incomplete gamma function                            //
//****************************************************************************//
double NormalizedLowerIncompleteGamma (
	double arg,					// Argument value
	size_t count				// Iterations count
){
	return 1.0 - NormalizedUpperIncompleteGamma (arg, count);
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
