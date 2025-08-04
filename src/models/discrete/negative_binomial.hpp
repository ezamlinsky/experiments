/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#            DISTRIBUTION MODEL FOR NEGATIVE BINOMIAL DISTRIBUTION             #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"discrete.hpp"

//****************************************************************************//
//      Class "NegativeBinomial"                                              //
//****************************************************************************//
class NegativeBinomial final : public BaseDiscrete
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	static const Range range;		// Function domain where the distribution exists
	static const size_t params;		// Count of distribution parameters
	const double gamma_log;			// Logarithm of the gamma function
	const double probability;		// Success probability
	const size_t successes;			// Number of successes

// Extract the distribution parameters from empirical observations
struct Params {

	// Members
	double probability;				// Success probability
	size_t successes;				// Number of successes

	// Constructor
	Params (
		const Observations &data	// Empirical observations
	){
		// Extract parameters from the empirical observations
		const double mean = data.Mean();
		const double variance = data.Variance();

		// Find the probability for these parameters
		probability = mean / variance;

		// Check if the found probability is correct
		if (0.0 <= probability && probability <= 1.0) {

			 // Find the number of successes
			 successes = round (mean * probability / (1.0 - probability));
		}
		else throw invalid_argument ("NegativeBinomial params: Can not estimate the probability value for the dataset");
	}
};

//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:
	NegativeBinomial (
		const Params &params		// Distribution parameters
	) : NegativeBinomial (params.successes, params.probability)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Probability Mass Function (PMF)                                       //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double PMF (
		size_t x					// Argument value
	) const {
		const double t1 = successes * log (probability) - gamma_log;
		const double t2 = x * log (1.0 - probability) - lgamma (x + 1);
		const double temp = lgamma (x + successes) + t1 + t2;
		return exp (temp);
	}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	NegativeBinomial (
		size_t successes,			// Number of successes
		double probability			// Success probability
	) :	gamma_log (lgamma (successes)),
		probability (probability),
		successes (successes)
	{
		// Check if the number of successes is correct
		if (successes == 0.0)
			throw invalid_argument ("NegativeBinomial: The number of successes must be positive");

		// Check if the probability value is correct
		if (probability < 0.0 && probability > 1.0)
			throw invalid_argument ("NegativeBinomial: The success probability must be in the range [0..1]");

		// Init the CDF cache
		Init (0);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor for empirical data                                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	NegativeBinomial (
		const Observations &data	// Empirical observations
	) : NegativeBinomial (Params (data))
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
//      Success probability                                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double Probability (void) const {
		return probability;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Number of successes                                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	size_t Successes (void) const {
		return successes;
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
		if (successes > 1)
			return floor ((successes - 1) * (1.0 - probability) / probability);
		else
			return 0.0;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Mean of the distribution                                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Mean (void) const override final {
		const double p = successes * (1.0 - probability);
		const double q = probability;
		return p / q;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Variance of the distribution                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Variance (void) const override final {
		const double p = successes * (1.0 - probability);
		const double q = probability * probability;
		return p / q;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Skewness of the distribution                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Skewness (void) const override final {
		const double p = 2.0 - probability;
		const double q = sqrt (successes * (1.0 - probability));
		return p / q;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Kurtosis of the distribution                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Kurtosis (void) const override final {
		const double temp = 6.0 / successes + 3.0;
		const double p = probability * probability;
		const double q = successes * (1.0 - probability);
		return temp + p / q;
	}
};

//****************************************************************************//
//      Internal constants used by the class                                  //
//****************************************************************************//
const Range NegativeBinomial::range = Range (0.0, INFINITY);
const size_t NegativeBinomial::params = 2;

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const NegativeBinomial &model)
{
	auto restore = stream.precision();
	stream.precision (PRECISION);
	stream << "\nNEGATIVE BINOMIAL DISTRIBUTION:" << std::endl;
	stream << "===============================" << std::endl;
	stream << static_cast <const BaseDiscrete&> (model);
	stream << "    Success probability\t\t\t= " << model.Probability() << endl;
	stream << "    Number of successes\t\t\t= " << model.Successes() << endl;
	stream << static_cast <const BaseModel&> (model);
	stream.precision (restore);
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
