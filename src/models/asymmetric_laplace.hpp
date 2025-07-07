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
# include	"continuous.hpp"

//****************************************************************************//
//      Class "AsymmetricLaplace"                                             //
//****************************************************************************//
class AsymmetricLaplace final : public Continuous
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	const double asymmetry;	// Asymmetry of the distribution

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	AsymmetricLaplace (
		double location,	// Location of the distribution
		double scale,		// Scale of the distribution
		double asymmetry	// Asymmetry of the distribution
	) : Continuous (location, scale),
		asymmetry (asymmetry)
	{
		if (asymmetry <= 0.0)
			throw invalid_argument ("AsymmetricLaplace: The asymmetry value must be positive");
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Asymmetry of the distrsibution                                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double Asymmetry (void) const {
		return asymmetry;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Probability Density Function (PDF)                                    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double PDF (
		double x			// Argument value
	) const override {
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
		double x			// Argument value
	) const override {
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
//      Mean of the distribution                                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Mean (void) const override {
		const double temp = asymmetry * asymmetry;
		return location + (1.0 - temp) / asymmetry * scale;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Variance of the distribution                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Variance (void) const override {
		const double temp = asymmetry * asymmetry;
		const double sqr = temp * temp;
		const double coeff = scale * scale;
		return (1.0 + sqr) / temp * coeff;
	}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const AsymmetricLaplace &model)
{
	auto restore = stream.precision();
	stream.precision (PRECISION);
	stream << "\nASYMMETRIC LAPLACE DISTRIBUTION:" << std::endl;
	stream << "================================" << std::endl;
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
