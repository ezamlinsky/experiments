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
# include	"base.hpp"

// TS elements to compute for PDF and CDF approximation
# define	KOLMOGOROV_N	100	// TODO: Check this threshold

//****************************************************************************//
//      Class "Kolmogorov"                                                    //
//****************************************************************************//
class Kolmogorov final : public BaseModel
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	static const double mean;		// Mean of the distribution
	static const double variance;	// Variance of the distribution
	static const double threshold;	// The threshold value to improve precision

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Kolmogorov (void) : BaseModel (Range (-INFINITY, INFINITY))
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Probability Density Function (PDF)                                    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double PDF (
		double x			// Argument value
	) const override {

		// Non positive argument
		if (x <= 0.0)
			return 0.0;

		// Common case
		else {
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
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Cumulative Distribution Function (CDF)                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double CDF (
		double x			// Argument value
	) const override {

		// Non positive argument
		if (x <= 0.0)
			return 0.0;

		// Common case
		else {
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
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Mean of the distribution                                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Mean (void) const override {
		return mean;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Variance of the distribution                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Variance (void) const override {
		return variance;
	}
};

//****************************************************************************//
//      Internal constants used by the class                                  //
//****************************************************************************//
const double Kolmogorov::mean = sqrt (0.5 * M_PI) * log (2.0);
const double Kolmogorov::variance = 0.5 * M_PI * (M_PI / 6.0 - log (2.0) * log (2.0));
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
	stream << static_cast <const BaseModel&> (model);
	stream.precision (restore);
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
