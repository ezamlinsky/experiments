/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#            DISTRIBUTION MODEL FOR ASYMMETRIC LAPLACE DISTRIBUTION            #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"continuous_distribution.hpp"

//****************************************************************************//
//      Class "AsymmetricLaplace"                                             //
//****************************************************************************//
class AsymmetricLaplace final : public Continuous
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	static const size_t params;		// Count of distribution parameters
	const double asymmetry;			// Asymmetry of the distribution

// Extract the distribution parameters from empirical observations
struct Params {

	// Members
	double location;				// Location of the distribution
	double scale;					// Scale of the distribution
	double asymmetry;				// Asymmetry of the distribution

	// Constructor
	Params (
		const Observations &data	// Empirical observations
	){
		// Check if empirical data range is inside the model domain
		if (Continuous::InDomain (data.Domain())) {

			// Extract parameters from the empirical observations
			const double mean = data.Mean();
			const double variance = data.Variance();
			const double skewness = data.SkewnessAroundMean();

			// The first approximation is the standard coefficient of asymmetry
			asymmetry = 1.0;
			for (int i = 0; i < 8; i++) {

				// Temporary variables for effective computation
				const double pow2 = asymmetry * asymmetry;
				const double pow3 = pow2 * asymmetry;
				const double pow4 = pow2 * pow2;
				const double temp1 = 1.0 + pow4;
				const double temp2 = temp1 * temp1;
				const double temp3 = sqrt (temp1);

				// Calculate the function and its derivative
				const double func = 2.0 * (1.0 - pow3) * (1.0 + pow3) / (temp1 * temp3);
				const double der = -12.0 * pow3 * (1.0 + pow2) / (temp2 * temp3);

				// Check the distance between the function and the target value
				const double diff = func - skewness;

				// Calculate a step value to move for the next point
				double step = diff / der;

				// A new approximation
				asymmetry -= step;
			}

			// Check if the found asymmetry coefficient is correct
			if (asymmetry > 0) {

				// Compute location and scale of the distribution
				const double temp1 = asymmetry * asymmetry;
				const double temp2 = temp1 * temp1;
				scale = sqrt ((variance * temp1) / (1.0 + temp2));
				location = mean - (1.0 - temp1) * scale / asymmetry;
			}
			else
				throw invalid_argument ("Asymmetric Laplace params: Can not estimate the asymmetry coefficient for the dataset");
		}
		else
			throw invalid_argument ("Asymmetric Laplace params: The data range is outside the distribution domain");
	}
};

//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:
	AsymmetricLaplace (
		const Params &params		// Distribution parameters
	) : AsymmetricLaplace (params.location, params.scale, params.asymmetry)
	{}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	AsymmetricLaplace (
		double location,			// Location of the distribution
		double scale,				// Scale of the distribution
		double asymmetry			// Asymmetry of the distribution
	) : Continuous (location, scale),
		asymmetry (asymmetry)
	{
		if (asymmetry <= 0.0)
			throw invalid_argument ("AsymmetricLaplace: The asymmetry value must be positive");
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor for empirical data                                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	AsymmetricLaplace (
		const Observations &data	// Empirical observations
	) : AsymmetricLaplace (Params (data))
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Asymmetry of the distrsibution                                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double Asymmetry (void) const {
		return asymmetry;
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
		const double temp = asymmetry * asymmetry;
		const double coeff = asymmetry / (1.0 + temp);
		if (x < location)
			return coeff / scale * exp (arg / asymmetry);
		else
			return coeff / scale * exp (-arg * asymmetry);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Cumulative Distribution Function (CDF)                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double CDF (
		double x					// Argument value
	) const override final {
		const double arg = (x - location) / scale;
		const double temp = asymmetry * asymmetry;
		if (x < location) {
			const double coeff = temp / (1.0 + temp);
			return coeff * exp (arg / asymmetry);
		}
		else {
			const double coeff = 1.0 / (1.0 + temp);
			return 1.0 - coeff * exp (-arg * asymmetry);
		}
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
		const double temp = asymmetry * asymmetry;
		return location + (1.0 - temp) / asymmetry * scale;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Variance of the distribution                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Variance (void) const override final {
		const double temp = asymmetry * asymmetry;
		const double sqr = temp * temp;
		const double coeff = scale * scale;
		return (1.0 + sqr) / temp * coeff;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Clone the distribution model                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual unique_ptr <const BaseModel> clone (void) const override final {
		return unique_ptr <const BaseModel> (new AsymmetricLaplace (*this));
	}
};

//****************************************************************************//
//      Internal constants used by the class                                  //
//****************************************************************************//
const size_t AsymmetricLaplace::params = 3;

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const AsymmetricLaplace &model)
{
	auto restore = stream.precision();
	stream.precision (PRECISION);
	stream << "\nASYMMETRIC LAPLACE DISTRIBUTION:" << std::endl;
	stream << "================================" << std::endl;
	stream << static_cast <const BaseContinuous&> (model);
	stream << static_cast <const Continuous&> (model);
	stream << "    Asymmetry\t\t\t\t= " << model.Asymmetry() << endl;
	stream << static_cast <const BaseModel&> (model);
	stream.precision (restore);
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
