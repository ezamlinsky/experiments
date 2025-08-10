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
//      Normalized upper incomplete gamma function for an integer argument    //
//****************************************************************************//
double NormalizedUpperIncompleteGamma (
	double arg,					// Argument value
	size_t count				// Iterations count
){
	// Check if the argument value is correct
	if (arg <= 0.0)
		throw invalid_argument ("NormalizedUpperIncompleteGamma: The argument value must be positive");

	// Compute the finite sum
	const double arg_log = log (arg);
	double sum = 0.0;
	for (size_t i = 0; i < count; i++) {
		const double temp = i * arg_log - arg - lgamma (i + 1.0);
		sum += exp (temp);
	}
	return sum;
}

//****************************************************************************//
//      Normalized upper incomplete gamma function for a half argument        //
//****************************************************************************//
double NormalizedUpperIncompleteGammaHalf (
	double arg,					// Argument value
	size_t count				// Iterations count
){
	// Check if the argument value is correct
	if (arg <= 0.0)
		throw invalid_argument ("NormalizedUpperIncompleteGammaHalf: The argument value must be positive");

	// Compute the finite sum
	const double arg_log = log (arg);
	double sum = 0.0;
	for (size_t i = 0; i < count; i++) {
		const double temp = (i + 0.5) * arg_log - arg - lgamma (i + 1.5);
		sum += exp (temp);
	}
	return erfc (sqrt (arg)) + sum;
}

//****************************************************************************//
//      Normalized lower incomplete gamma function for an integer argument    //
//****************************************************************************//
double NormalizedLowerIncompleteGamma (
	double arg,					// Argument value
	size_t count				// Iterations count
){
	return 1.0 - NormalizedUpperIncompleteGamma (arg, count);
}

//****************************************************************************//
//      Normalized lower incomplete gamma function for a half argument        //
//****************************************************************************//
double NormalizedLowerIncompleteGammaHalf (
	double arg,					// Argument value
	size_t count				// Iterations count
){
	return 1.0 - NormalizedUpperIncompleteGammaHalf (arg, count);
}

//****************************************************************************//
//      Normalized lower incomplete gamma function for a real argument        //
//****************************************************************************//
double NormalizedLowerIncompleteGamma (
	double arg,					// Argument value
	double shape				// The shape value
){
	// Check if the argument value is correct
	if (arg <= 0.0)
		throw invalid_argument ("NormalizedUpperIncompleteGamma: The argument value must be positive");
	if (shape <= 0.0)
		throw invalid_argument ("NormalizedLowerIncompleteGamma: The shape value must be positive");

	// Special case
	if (exp (shape - arg) == 0.0)
		return 1.0;

	// Common case
	const double arg_log = log (arg);
	double gamma_log = lgamma (shape);
	double sum = 0.0;
	double last;
	size_t i = 0;
	do {
		last = sum;
		gamma_log += log (shape + i);
		const double current = exp ((shape + i) * arg_log - arg - gamma_log);
		sum += current;
		i++;
	} while (sum > last);
	return sum;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
