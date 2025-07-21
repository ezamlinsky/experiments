/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                 DISTRIBUTION MODEL FOR THE BETA DISTRIBUTION                 #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"special_beta.hpp"
# include	"base.hpp"

//****************************************************************************//
//      Class "Beta"                                                          //
//****************************************************************************//
class Beta final : public BaseModel
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	static const Range range;		// Function domain where the distribution exists
	const SpecialBeta beta;			// Special beta function
	const double shape1;			// The first shape parameter
	const double shape2;			// The second shape parameter

// Extract the distribution parameters from empirical observations
struct Params {

	// Members
	double shape1;					// The first shape parameter
	double shape2;					// The second shape parameter

	// Constructor
	Params (
		const Observations &data	// Empirical observations
	){
		// Check if empirical data range is inside the model domain
		if (Beta::InDomain (data.Domain())) {

			// Extract parameters from the empirical observations
			const double mean = data.Mean();
			const double variance = data.Variance();

			// Find the shapes for these parameters
			const double temp = 1.0 - mean;
			shape1 = mean * mean * temp / variance - mean;
			shape2 = mean * temp * temp / variance - temp;
		}
		else
			throw invalid_argument ("Beta params: The data range is outside the distribution domain");
	}
};

//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:
	Beta (
		const Params &params		// Distribution parameters
	) : Beta (params.shape1, params.shape2)
	{}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Beta (
		double shape1,				// The first shape parameter
		double shape2				// The second shape parameter
	) : beta (SpecialBeta (shape1, shape2)),
		shape1 (shape1),
		shape2 (shape2)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor for empirical data                                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Beta (
		const Observations &data	// Empirical observations
	) : Beta (Params (data))
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Check if the range is inside the model domain                         //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	static bool InDomain (
		const Range &subrange	// Testing range
	){
		return range.IsInside (subrange);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      The first shape parameter of the distribution                         //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	size_t Shape1 (void) const {
		return shape1;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      The second shape parameter of the distribution                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	size_t Shape2 (void) const {
		return shape2;
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

		// Negative argument
		if (x < 0.0)
			return NAN;

		// Handle cases where log(0) would occur
		else if (x == 0.0) {
			if (shape1 < 1.0)
				return INFINITY;
			else if (shape1 > 1.0)
				return 0.0;
			else
				return shape2;
		}
		else if (x == 1.0) {
			if (shape2 < 1.0)
				return INFINITY;
			else if (shape2 > 1.0)
				return 0.0;
			else
				return shape1;
		}

		// Common case
		else {
			const double t1 = (shape1 - 1.0) * log (x) + (shape2 - 1.0) * log (1.0 - x);
			const double t2 = beta.BetaLog();
			const double temp = t1 - t2;
			return exp (temp);
		}
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Cumulative Distribution Function (CDF)                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double CDF (
		double x					// Argument value
	) const override final {
		return beta.RegIncompleteBeta (x);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Mode of the distribution                                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Mode (void) const override final {
		if (shape1 < 1.0 && shape2 < 1.0)
			return NAN;
		else if (shape1 > 1.0 && shape2 > 1.0) {
			const double temp1 = shape1 - 1.0;
			const double temp2 = shape1 + shape2 - 2.0;
			return temp1 / temp2;
		}
		else if (shape1 < shape2)
			return 0.0;
		else if (shape1 > shape2)
			return 1.0;
		else
			return Mean();
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Mean of the distribution                                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Mean (void) const override final {
		return shape1 / (shape1 + shape2);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Variance of the distribution                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Variance (void) const override final {
		const double temp = shape1 + shape2;
		return shape1 * shape2 / (temp * temp * (temp + 1.0));
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Clone the distribution model                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual unique_ptr <const BaseModel> clone (void) const override final {
		return unique_ptr <const BaseModel> (new Beta (*this));
	}
};

//****************************************************************************//
//      Internal constants used by the class                                  //
//****************************************************************************//
const Range Beta::range = Range (0.0, 1.0);

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const Beta &model)
{
	auto restore = stream.precision();
	stream.precision (PRECISION);
	stream << "\nBETA DISTRIBUTION:" << std::endl;
	stream << "==================" << std::endl;
	stream << "\nParameters:" << endl;
	stream << "~~~~~~~~~~~" << endl;
	stream << "    Shape #1\t\t\t\t= " << model.Shape1() << endl;
	stream << "    Shape #2\t\t\t\t= " << model.Shape2() << endl;
	stream << static_cast <const BaseModel&> (model);
	stream.precision (restore);
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
