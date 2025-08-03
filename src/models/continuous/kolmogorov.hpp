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
		double sum = 0.0;

		// Calculate the PDF function when x is big
		if (x >= threshold) {
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
//      Cumulative Distribution Function (CDF)                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double CDF (
		double x			// Argument value
	) const override final {

		// Below the range
		if (x <= range) return 0.0;

		// Common case
		double sum = 0.0;

		// Calculate the CDF function when x is big
		if (x >= threshold) {
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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Mode of the distribution                                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Mode (void) const override final {
		return 0.735467907916572;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Mean of the distribution                                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Mean (void) const override final {
		return sqrt (0.5 * M_PI) * log (2.0);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Variance of the distribution                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Variance (void) const override final {
		const double temp = log (2.0);
		return 0.5 * M_PI * (M_PI / 6.0 - temp * temp);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Skewness of the distribution                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Skewness (void) const override final {
		const double temp1 = log (2);
		const double temp2 = M_PI / 6.0 - temp1 * temp1;
		const double p = 2.0 * temp1 * (temp1 * temp1 - 0.25 * M_PI);
		const double q = temp2 * sqrt (temp2);
		return (p + 0.4304549205350666) / q;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Kurtosis of the distribution                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Kurtosis (void) const override final {
		return NAN;		// TODO: Not implemented yet
	}
};

//****************************************************************************//
//      Internal constants used by the class                                  //
//****************************************************************************//
const Range Kolmogorov::range = Range (0, INFINITY);
const size_t Kolmogorov::params = 0;
const double Kolmogorov::threshold = sqrt (log (2.0));

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
