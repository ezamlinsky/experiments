/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#               DISTRIBUTION MODEL FOR STUDENT’S T-DISTRIBUTION                #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"special_beta.hpp"
# include	"base.hpp"

//****************************************************************************//
//      Class "StandardT"                                                     //
//****************************************************************************//
class StandardT final : public BaseModel
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	const SpecialBeta beta;			// Special beta function
	const size_t df;				// Degrees of freedom

// Extract the distribution parameters from empirical observations
struct Params {

	// Members
	size_t df;						// Degrees of freedom

	// Constructor
	Params (
		const Observations &data	// Empirical observations
	){
		// Extract parameters from the empirical observations
		const double temp = data.Variance();

		// Find degrees of freedom for these parameters
		if (temp <= 2.0 * M_PI) {
			const double temp1 = 2.0 * temp;
			const double temp2 = temp - 1.0;
			df = round (temp1 / temp2);
		}
		else
			df = 1;
	}
};

//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:
	StandardT (
		const Params &params		// Distribution parameters
	) : StandardT (params.df)
	{}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	StandardT (
		size_t df					// Degrees of freedom
	) : BaseModel (Range (-INFINITY, INFINITY)),
		beta (SpecialBeta (0.5 * df, 0.5)),
		df (df)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor for empirical data                                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	StandardT (
		const Observations &data	// Empirical observations
	) : StandardT (Params (data))
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Degrees of freedom of the distribution                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	size_t DF (void) const {
		return df;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Probability Density Function (PDF)                                    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double PDF (
		double x					// Argument value
	) const override {
		const double arg = 1.0 + x * x / df;
		const double temp = -0.5 * (df + 1.0) * log (arg) - beta.BetaLog();
		return exp (temp) / sqrt (df);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Cumulative Distribution Function (CDF)                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double CDF (
		double x					// Argument value
	) const override {
		const double arg = df / (df + x * x);
		if (x < 0.0)
			return 0.5 * beta.RegIncompleteBeta (arg);
		else
			return 1.0 - 0.5 * beta.RegIncompleteBeta (arg);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Mode of the distribution                                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Mode (void) const override {
		return 0.0;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Mean of the distribution                                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Mean (void) const override {
		if (df > 1)
			return 0.0;
		else
			return NAN;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Variance of the distribution                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Variance (void) const override {
		if (df > 2)
			return df / (df - 2.0);
		else if (df > 1)
			return INFINITY;
		else
			return NAN;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Clone the distribution model                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual unique_ptr <const BaseModel> clone (void) const {
		return unique_ptr <const BaseModel> (new StandardT (*this));
	}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const StandardT &model)
{
	auto restore = stream.precision();
	stream.precision (PRECISION);
	stream << "\nSTUDENT’S T-DISTRIBUTION:" << std::endl;
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
