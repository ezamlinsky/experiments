/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                 DISTRIBUTION MODEL FOR POISSON DISTRIBUTION                  #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"discrete.hpp"

//****************************************************************************//
//      Class "Poisson"                                                       //
//****************************************************************************//
class Poisson final : public BaseDiscrete
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	static const Range range;		// Function domain where the distribution exists
	static const size_t params;		// Count of distribution parameters
	const double rate;				// Expected rate of occurrences

// Extract the distribution parameters from empirical observations
struct Params {

	// Members
	double rate;					// Expected rate of occurrences

	// Constructor
	Params (
		const Observations &data	// Empirical observations
	){
		// Check if empirical data range is inside the model domain
		if (Poisson::InDomain (data.Domain())) {

			// Extract parameters from the empirical observations
			rate = data.Mean();
			if (rate <= 0.0)
				throw invalid_argument ("Poisson params: Can not estimate the rate value for the dataset");
		}
		else throw invalid_argument ("Poisson params: The data range is outside the distribution domain");
	}
};

//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:
	Poisson (
		const Params &params		// Distribution parameters
	) : Poisson (params.rate)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Probability Mass Function (PMF)                                       //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double PMF (
		size_t x					// Argument value
	) const {
		const double temp = x * log (rate) - rate - lgamma (x + 1);
		return exp (temp);
	}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Poisson (
		double rate					// Expected rate of occurrences
	) :	BaseDiscrete (-1),
		rate (rate)
	{
		if (rate <= 0.0)
			throw invalid_argument ("Poisson: The rate value must be positive");

		// Limit the search range of the distribution quantile levels
		LimitRange();
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor for empirical data                                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Poisson (
		const Observations &data	// Empirical observations
	) : Poisson (Params (data))
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
//      Expected rate of occurrences                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double Rate (void) const {
		return rate;
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

		// Outside the distribution domain
		if (x < range) return 0.0;

		// Common case
		const size_t arg = floor (x);
		return PMF (arg);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Cumulative Distribution Function (CDF)                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double CDF (
		double x					// Argument value
	) const override final {

		// Below the range
		if (x < range) return 0.0;

		// Common case
		const size_t arg = floor (x);
		double sum = 0.0;
		for (size_t i = 0; i <= arg; i++)
			sum += PMF (i);
		return sum;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Mode of the distribution                                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Mode (void) const override final {
		return floor (rate);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Mean of the distribution                                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Mean (void) const override final {
		return rate;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Variance of the distribution                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Variance (void) const override final {
		return rate;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Skewness of the distribution                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Skewness (void) const override final {
		return 1.0 / sqrt (rate);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Kurtosis of the distribution                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Kurtosis (void) const override final {
		return 1.0 / rate;
	}
};

//****************************************************************************//
//      Internal constants used by the class                                  //
//****************************************************************************//
const Range Poisson::range = Range (0.0, INFINITY);
const size_t Poisson::params = 1;

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const Poisson &model)
{
	auto restore = stream.precision();
	stream.precision (PRECISION);
	stream << "\nPOISSON DISTRIBUTION:" << std::endl;
	stream << "=====================" << std::endl;
	stream << static_cast <const BaseDiscrete&> (model);
	stream << "    Expected rate of occurrences\t= " << model.Rate() << endl;
	stream << static_cast <const BaseModel&> (model);
	stream.precision (restore);
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
