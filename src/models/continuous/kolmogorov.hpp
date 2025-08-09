/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                DISTRIBUTION MODEL FOR KOLMOGOROV DISTRIBUTION                #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"continuous.hpp"

// TS elements to compute for PDF and CDF approximation
# define	KOLMOGOROV_N	6

// Adjusted distribution moments
# define	MEAN		sqrt (M_PI_2) * M_LN2
# define	VARIANCE	M_PI_2 * (M_PI / 6 - M_LN2 * M_LN2)
# define	M3			sqrt (M_PI_2) * 0.6761570080272717
# define	M4			M_PI * M_PI * M_PI * M_PI * 7.0 / 720.0

//****************************************************************************//
//      Class "Kolmogorov"                                                    //
//****************************************************************************//
class Kolmogorov final : public BaseContinuous
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	static const Range range;		// Function domain where the distribution exists
	static const size_t params;		// Count of distribution parameters
	static const double threshold;	// The threshold value to improve precision
	static const double mode;		// Mode value of the distribution

//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Calculate the mode values of the distribution                         //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// INFO:	The golden-section search
	static double FindMode (void) {

		// The edge points are 0 and the mean value
		double left = 0.0;
		double right = MEAN;

		// The "Golden ratio"
		const double fib = 0.5 * sqrt (5.0) - 0.5;

		// Find intermediate points that split the range with the "Golden ratio"
		double length = right - left;
		double point1 = right - length * fib;
		double point2 = left + length * fib;

		// Calculate the function at the intermediate points
		double func_point1 = pdf (point1);
		double func_point2 = pdf (point2);

		// Do the golden-section search while the range length shrinks
		double last;
		do {
			// Remember the length of the last range for the exit condition
			last = length;

			if (func_point1 > func_point2) {

				// The maximum resides in the left sub-range
				right = point2;

				// Update the existing right intermediate point
				point2 = point1;
				func_point2 = func_point1;

				// Calculate a new left intermediate point
				length = right - left;
				point1 = right - length * fib;
				func_point1 = pdf (point1);
			}
			else {

				// The maximum resides in the right sub-range
				left = point1;

				// Update the existing left intermediate point
				point1 = point2;
				func_point1 = func_point2;

				// Calculate a new right intermediate point
				length = right - left;
				point2 = left + length * fib;
				func_point2 = pdf (point2);
			}

		} while (length < last);

		// The middle point is the mode
		return 0.5 * (left + right);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Calculate Probability Density Function (PDF)                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	static double pdf (
		double x			// Argument value
	){
		// Calculate the PDF function when x is big
		if (x >= threshold) {
			double sum = 0.0;
			for (int i = 1; i < KOLMOGOROV_N; i++) {
				const double temp = i * x;
				const double exponent = -2.0 * temp * temp;
				if (i % 2)
					sum += i * i * exp (exponent);
				else
					sum -= i * i * exp (exponent);
			}
			return 8.0 * x * sum;
		}

		// Calculate the PDF function when x is small
		else {
			double sum = 0.0;
			for (int i = 1; i < KOLMOGOROV_N; i++) {
				const double p = (2.0 * i - 1.0) * M_PI;
				const double q = 2.0 * x;
				const double temp = p / q;
				const double exponent = -0.5 * temp * temp;
				sum += (temp * temp - 1.0) * exp (exponent);
			}
			return sqrt (2.0 * M_PI) * sum / (x * x);
		}
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Calculate Cumulative Distribution Function (CDF)                      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	static double cdf (
		double x			// Argument value
	){
		// Calculate the CDF function when x is big
		if (x >= threshold) {
			double sum = 0.0;
			for (int i = 1; i < KOLMOGOROV_N; i++) {
				const double temp = i * x;
				const double exponent = -2.0 * temp * temp;
				if (i % 2)
					sum += exp (exponent);
				else
					sum -= exp (exponent);
			}
			return 1.0 - 2.0 * sum;
		}

		// Calculate the CDF function when x is small
		else {
			double sum = 0.0;
			for (int i = 1; i < KOLMOGOROV_N; i++) {
				const double p = (2.0 * i - 1.0) * M_PI;
				const double q = 2.0 * x;
				const double temp = p / q;
				const double exponent = -0.5 * temp * temp;
				sum += exp (exponent);
			}
			return sqrt (2.0 * M_PI) * sum / x;
		}
	}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Kolmogorov (void) = default;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Function domain where the distribution exists                         //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual const Range& Domain (void) const override final {
		return range;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Number of distribution parameters to describe the population          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual size_t Parameters (void) const override final {
		return params;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Probability Density Function (PDF)                                    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double PDF (
		double x			// Argument value
	) const override final {

		// Below the range
		if (x <= range) return 0.0;

		// Common case
		return pdf (x);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Cumulative Distribution Function (CDF)                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double CDF (
		double x			// Argument value
	) const override final {

		// Below the range
		if (x <= range) return 0.0;

		// Common case
		return cdf (x);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Mode of the distribution                                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Mode (void) const override final {
		return mode;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Mean of the distribution                                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual constexpr double Mean (void) const override final {
		return MEAN;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Variance of the distribution                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual constexpr double Variance (void) const override final {
		return VARIANCE;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Skewness of the distribution                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual constexpr double Skewness (void) const override final {
		constexpr double mean = MEAN;
		constexpr double variance = VARIANCE;
		constexpr double temp = mean * mean;
		constexpr double p = M3 + mean * (2.0 * temp - M_PI_2 * M_PI_2);
		constexpr double q = variance * sqrt (variance);
		return p / q;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Kurtosis of the distribution                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual constexpr double Kurtosis (void) const override final {
		constexpr double mean = MEAN;
		constexpr double variance = VARIANCE;
		constexpr double temp = mean * mean;
		constexpr double p = M4 - (4.0 * M3 * mean + temp * (3.0 * temp - M_PI * M_PI_2));
		constexpr double q = variance * variance;
		return p / q;
	}
};

//****************************************************************************//
//      Internal constants used by the class                                  //
//****************************************************************************//
const Range Kolmogorov::range = Range (0, INFINITY);
const size_t Kolmogorov::params = 0;
const double Kolmogorov::threshold = sqrt (M_LN2);
const double Kolmogorov::mode = Kolmogorov::FindMode();

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const Kolmogorov &model)
{
	auto restore = stream.precision();
	stream.precision (PRECISION);
	stream << "\nKOLMOGOROV DISTRIBUTION:" << std::endl;
	stream << "========================" << std::endl;
	stream << static_cast <const BaseContinuous&> (model);
	stream << static_cast <const BaseModel&> (model);
	stream.precision (restore);
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
