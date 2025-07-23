/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#               DISTRIBUTION MODEL FOR CHI-SQUARED DISTRIBUTION                #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"special_gamma.hpp"

//****************************************************************************//
//      Class "ChiSquared"                                                    //
//****************************************************************************//
class ChiSquared final : public SpecialGamma
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:

// Extract the distribution parameters from empirical observations
struct Params {

	// Members
	size_t df;						// Degrees of freedom

	// Constructor
	Params (
		const Observations &data	// Empirical observations
	){
		// Check if empirical data range is inside the model domain
		if (SpecialGamma::InDomain (data.Domain())) {

			// Extract parameters from the empirical observations
			const double variance = data.Variance();

			// Find the degrees of freedom
			df = max (1.0, round (0.5 * variance));
		}
		else
			throw invalid_argument ("Chi-squared params: The data range is outside the distribution domain");
	}
};

//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:
	ChiSquared (
		const Params &params		// Distribution parameters
	) : ChiSquared (params.df)
	{}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	ChiSquared (
		size_t df					// Degrees of freedom
	) : SpecialGamma (df, 2.0)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor for empirical data                                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	ChiSquared (
		const Observations &data	// Empirical observations
	) : ChiSquared (Params (data))
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Degrees of freedom of the distribution                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	size_t DF (void) const {
		return gamma_shape;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Mode of the distribution                                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Mode (void) const override final {
		if (gamma_shape >= 2)
			return gamma_shape - 2;
		else
			return 0;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Mean of the distribution                                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Mean (void) const override final {
		return gamma_shape;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Variance of the distribution                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Variance (void) const override final {
		return 2.0 * gamma_shape;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Clone the distribution model                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual unique_ptr <const BaseModel> clone (void) const override final {
		return unique_ptr <const BaseModel> (new ChiSquared (*this));
	}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const ChiSquared &model)
{
	auto restore = stream.precision();
	stream.precision (PRECISION);
	stream << "\nCHI-SQUARED DISTRIBUTION:" << std::endl;
	stream << "=========================" << std::endl;
	stream << "\nParameters:" << endl;
	stream << "~~~~~~~~~~~" << endl;
	stream << "    Degrees of freedom\t\t\t= " << model.DF() << endl;
	stream << static_cast <const BaseModel&> (model);
	stream.precision (restore);
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
