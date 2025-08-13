/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#               DISTRIBUTION MODEL FOR EXPONENTIAL DISTRIBUTION                #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"../confidence_interval.hpp"
# include	"scale_dist.hpp"
# include	"chi_squared.hpp"

//****************************************************************************//
//      Class "Exponential"                                                   //
//****************************************************************************//
class Exponential final : public ScaleDist
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:

// Extract the distribution parameters from empirical observations
struct Params {

	// Members
	double scale;					// Scale of the distribution

	// Constructor
	Params (
		const Observations &data	// Empirical observations
	){
		// Check if empirical data range is inside the model domain
		if (Exponential::InDomain (data.Domain())) {

			// Extract parameters from the empirical observations
			scale = data.Median() / M_LN2;
			if (scale <= 0.0)
				throw invalid_argument ("Exponential params: Can not estimate the scale value for the dataset");
		}
		else
			throw invalid_argument ("Exponential params: The data range is outside the distribution domain");
	}
};

//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:
	Exponential (
		const Params &params		// Distribution parameters
	) : Exponential (params.scale)
	{}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Exponential (
		double scale				// Scale of the distribution
	) : ScaleDist (scale)
	{
		if (scale <= 0.0)
			throw invalid_argument ("Exponential: The scale value must be positive");
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor for empirical data                                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Exponential (
		const Observations &data	// Empirical observations
	) : Exponential (Params (data))
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Confidence interval of the mean value                                 //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	ConfidenceInterval Mean_ConfidenceInterval (
		double level,				// Confidence level
		size_t size					// Sample size
	){
		// Check if the level is correct
		if (0.0 <= level && level <= 1.0) {
			const auto dist = ChiSquared (2 * size);
			const double temp = 2 * scale * size;
			const double alpha = 1.0 - level;
			const double lower = temp / dist.Quantile (1.0 - 0.5 * alpha);
			const double upper = temp / dist.Quantile (0.5 * alpha);
			return ConfidenceInterval (level, scale, Range (lower, upper));
		}
		else
			throw invalid_argument ("Mean_ConfidenceInterval: The confidence level must be in the range [0..1]");
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Probability Density Function (PDF)                                    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double PDF (
		double x					// Argument value
	) const override final {

		// Below the range
		if (x < range) return 0.0;

		// Common case
		const double arg = x / scale;
		return exp (-arg) / scale;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Cumulative Distribution Function (CDF)                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double CDF (
		double x					// Argument value
	) const override final {

		// Below the range
		if (x <= range) return 0.0;

		// Common case
		const double arg = x / scale;
		return 1.0 - exp (-arg);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Mode of the distribution                                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Mode (void) const override final {
		return 0.0;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Mean of the distribution                                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Mean (void) const override final {
		return scale;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Variance of the distribution                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Variance (void) const override final {
		return scale * scale;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Skewness of the distribution                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Skewness (void) const override final {
		return 2.0;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Kurtosis of the distribution                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Kurtosis (void) const override final {
		return 9.0;
	}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const Exponential &model)
{
	auto restore = stream.precision();
	stream.precision (PRECISION);
	stream << "\nEXPONENTIAL DISTRIBUTION:" << std::endl;
	stream << "=========================" << std::endl;
	stream << static_cast <const BaseContinuous&> (model);
	stream << static_cast <const ScaleDist&> (model);
	stream << static_cast <const BaseModel&> (model);
	stream.precision (restore);
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
