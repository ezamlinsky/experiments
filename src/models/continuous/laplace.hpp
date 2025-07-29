/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                 DISTRIBUTION MODEL FOR LAPLACE DISTRIBUTION                  #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"../confidence_interval.hpp"
# include	"continuous_distribution.hpp"
# include	"chi_squared.hpp"

//****************************************************************************//
//      Class "Laplace"                                                       //
//****************************************************************************//
class Laplace final : public Continuous
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
			location = data.Median();
			scale = data.MedianAbsDevFromMedian() / log (2);
		}
		else
			throw invalid_argument ("Laplace params: The data range is outside the distribution domain");
	}
};

//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:
	Laplace (
		const Params &params		// Distribution parameters
	) : Laplace (params.location, params.scale)
	{}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Laplace (
		double location,			// Location of the distribution
		double scale				// Scale of the distribution
	) : Continuous (location, scale)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor for empirical data                                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Laplace (
		const Observations &data	// Empirical observations
	) : Laplace (Params (data))
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Mean Absolute Deviation (MAD) of the distribution                     //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double MAD (void) const {
		return scale;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Confidence interval of the Mean Absolute Deviation (MAD)              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	ConfidenceInterval MAD_ConfidenceInterval (
		double level,				// Confidence level
		size_t size					// Sample size
	){
		if (0.0 <= level && level <= 1.0)
		{
			const auto dist = ChiSquared (2 * size);
			const double temp = 2 * scale * size;
			const double alpha = 1 - level;
			const double lower = temp / dist.Quantile (1.0 - 0.5 * alpha);
			const double upper = temp / dist.Quantile (0.5 * alpha);
			return ConfidenceInterval (level, scale, Range (lower, upper));
		}
		else
			throw invalid_argument ("MAD_ConfidenceInterval: The confidence level must be in the range [0..1]");
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
		return 0.5 * exp (-fabs (arg)) / scale;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Cumulative Distribution Function (CDF)                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double CDF (
		double x					// Argument value
	) const override final {
		const double arg = (x - location) / scale;
		if (arg < 0.0)
			return 0.5 * exp (arg);
		else
			return 1 - 0.5 * exp (-arg);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Mode of the distribution                                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Mode (void) const override final {
		return location;
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
		return 2.0 * scale * scale;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Clone the distribution model                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual unique_ptr <const BaseModel> clone (void) const override final {
		return unique_ptr <const BaseModel> (new Laplace (*this));
	}
};

//****************************************************************************//
//      Internal constants used by the class                                  //
//****************************************************************************//
const size_t Laplace::params = 2;

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const Laplace &model)
{
	auto restore = stream.precision();
	stream.precision (PRECISION);
	stream << "\nLAPLACE DISTRIBUTION:" << std::endl;
	stream << "=====================" << std::endl;
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
