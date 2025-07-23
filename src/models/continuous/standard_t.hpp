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
# include	"continuous.hpp"

//****************************************************************************//
//      Class "StandardT"                                                     //
//****************************************************************************//
class StandardT final : public BaseContinuous
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	static const Range range;		// Function domain where the distribution exists
	const SpecialBeta beta;			// Special beta function
	const size_t df;				// Degrees of freedom

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	StandardT (
		size_t df					// Degrees of freedom
	) : beta (SpecialBeta (0.5 * df, 0.5)),
		df (df)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Degrees of freedom of the distribution                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	size_t DF (void) const {
		return df;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Function domain where the distribution exists                         //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual const Range& Domain (void) const override final {
		return range;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Probability Density Function (PDF)                                    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double PDF (
		double x					// Argument value
	) const override final {
		const double arg = 1.0 + x * x / df;
		const double temp = -0.5 * (df + 1.0) * log (arg) - beta.BetaLog();
		return exp (temp) / sqrt (df);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Cumulative Distribution Function (CDF)                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double CDF (
		double x					// Argument value
	) const override final {
		const double arg = df / (df + x * x);
		if (x < 0.0)
			return 0.5 * beta.RegIncompleteBeta (arg);
		else
			return 1.0 - 0.5 * beta.RegIncompleteBeta (arg);
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
		if (df > 1)
			return 0.0;
		else
			return INFINITY;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Variance of the distribution                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Variance (void) const override final {
		if (df > 2)
			return df / (df - 2.0);
		else
			return INFINITY;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Clone the distribution model                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual unique_ptr <const BaseModel> clone (void) const override final {
		return unique_ptr <const BaseModel> (new StandardT (*this));
	}
};

//****************************************************************************//
//      Internal constants used by the class                                  //
//****************************************************************************//
const Range StandardT::range = Range (-INFINITY, INFINITY);

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
