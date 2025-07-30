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
# include	"continuous.hpp"
# include	"chi_squared.hpp"

//****************************************************************************//
//      Class "Exponential"                                                   //
//****************************************************************************//
class Exponential final : public BaseContinuous
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	static const Range range;		// Function domain where the distribution exists
	static const size_t params;		// Count of distribution parameters
	const double scale;				// Scale of the distribution

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
			scale = data.Median() / log (2);
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
	) : scale (scale)
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
//      Check if the range is inside the model domain                         //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	static bool InDomain (
		const Range &subrange		// Testing range
	){
		return range >= subrange;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Scale of the distribution                                             //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double Scale (void) const {
		return scale;
	}

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
//      Clone the distribution model                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual unique_ptr <const BaseModel> clone (void) const override final {
		return unique_ptr <const BaseModel> (new Exponential (*this));
	}
};

//****************************************************************************//
//      Internal constants used by the class                                  //
//****************************************************************************//
const Range Exponential::range = Range (0.0, INFINITY);
const size_t Exponential::params = 1;

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
	stream << "    Scale\t\t\t\t= " << model.Scale() << endl;
	stream << static_cast <const BaseModel&> (model);
	stream.precision (restore);
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
