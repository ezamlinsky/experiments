/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#            DISTRIBUTION MODEL FOR NORMAL (GAUSSIAN) DISTRIBUTION             #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	<boost/math/special_functions/erf.hpp>
# include	"../confidence_interval.hpp"
# include	"continuous_distribution.hpp"
# include	"chi_squared.hpp"
# include	"standard_t.hpp"

//****************************************************************************//
//      Class "Normal"                                                        //
//****************************************************************************//
class Normal final : public Continuous
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	static const size_t params;		// Count of distribution parameters

// Extract the distribution parameters from empirical observations
struct Params {

	// Members
	double location;				// Location of the distribution
	double scale;					// Scale of the distribution

	// Constructor
	Params (
		const Observations &data	// Empirical observations
	){
		// Check if empirical data range is inside the model domain
		if (Continuous::InDomain (data.Domain())) {

			// Extract parameters from the empirical observations
			const double median = data.Median();
			const double deviation = data.MedianAbsDevFromMedian();

			// Find the location and the scale for these parameters
			location = median;
			scale = deviation / (M_SQRT2 * boost::math::erf_inv (0.5));
		}
		else
			throw invalid_argument ("Normal params: The data range is outside the distribution domain");
	}
};

//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:
	Normal (
		const Params &params		// Distribution parameters
	) : Normal (params.location, params.scale)
	{}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Normal (
		double location,			// Location of the distribution
		double scale				// Scale of the distribution
	) : Continuous (location, scale)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor for empirical data                                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Normal (
		const Observations &data	// Empirical observations
	) : Normal (Params (data))
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Confidence interval for the mean                                      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	ConfidenceInterval Mean_ConfidenceInterval (
		double level,				// Confidence level
		size_t size					// Sample size
	){
		// Check if the level is correct
		if (0.0 <= level && level <= 1.0) {
			const auto dist = StandardT (size);
			const double temp = scale / sqrt (size);
			const double alpha = 1.0 - level;
			const double quantile = dist.Quantile (1.0 - 0.5 * alpha);
			const double lower = location - temp * quantile;
			const double upper = location + temp * quantile;
			return ConfidenceInterval (level, location, Range (lower, upper));
		}
		else
			throw invalid_argument ("Mean_ConfidenceInterval: The confidence level must be in the range [0..1]");
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Confidence interval for the variance                                  //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	ConfidenceInterval Variance_ConfidenceInterval (
		double level,				// Confidence level
		size_t size					// Sample size
	){
		// Check if the level is correct
		if (0.0 <= level && level <= 1.0) {
			const auto dist = ChiSquared (size);
			const double temp = scale * scale * size;
			const double alpha = 1.0 - level;
			const double lower = temp / dist.Quantile (1.0 - 0.5 * alpha);
			const double upper = temp / dist.Quantile (0.5 * alpha);
			return ConfidenceInterval (level, scale * scale, Range (lower, upper));
		}
		else
			throw invalid_argument ("Variance_ConfidenceInterval: The confidence level must be in the range [0..1]");
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
		double x					// Argument value
	) const override final {
		const double arg = (x - location) / scale;
		return exp (-0.5 * arg * arg) / (scale * M_SQRT2 * sqrt (M_PI));
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Cumulative Distribution Function (CDF)                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double CDF (
		double x					// Argument value
	) const override final {
		const double arg = (x - location) / scale;
		return 0.5 * (1.0 + erf (arg / M_SQRT2));
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Mean of the distribution                                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Mean (void) const override final {
		return location;
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
		return 0.0;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Kurtosis of the distribution                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Kurtosis (void) const override final {
		return 3.0;
	}
};

//****************************************************************************//
//      Internal constants used by the class                                  //
//****************************************************************************//
const size_t Normal::params = 2;

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const Normal &model)
{
	auto restore = stream.precision();
	stream.precision (PRECISION);
	stream << "\nNORMAL (GAUSSIAN) DISTRIBUTION:" << std::endl;
	stream << "===============================" << std::endl;
	stream << static_cast <const BaseContinuous&> (model);
	stream << static_cast <const Continuous&> (model);
	stream << static_cast <const BaseModel&> (model);
	stream.precision (restore);
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
