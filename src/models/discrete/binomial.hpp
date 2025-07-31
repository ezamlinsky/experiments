/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                 DISTRIBUTION MODEL FOR BINOMIAL DISTRIBUTION                 #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"discrete.hpp"

//****************************************************************************//
//      Class "Binomial"                                                      //
//****************************************************************************//
class Binomial final : public BaseDiscrete
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	static const size_t params;		// Count of distribution parameters
	const Range range;				// Function domain where the distribution exists
	const double gamma_log;			// Logarithm of the gamma function
	const double probability;		// Probability of a successful trial
	const size_t trials;			// Number of trials

// Extract the distribution parameters from empirical observations
struct Params {

	// Members
	double probability;				// Probability of a successful trial
	size_t trials;					// Number of trials

	// Constructor
	Params (
		const Observations &data	// Empirical observations
	){
		// Extract parameters from the empirical observations
		const double mean = data.Mean();
		const double variance = data.Variance();
		const size_t data_max = data.Domain().Max();

		// Find the probability for these parameters
		probability = 1.0 - variance / mean;

		// Check if the found probability is correct
		if (0.0 <= probability && probability <= 1.0) {

			 // Find the number of trials
			const size_t trials_max = round (mean / probability);
			trials = max (data_max, trials_max);
		}
		else
			throw invalid_argument ("Binomial: Can not estimate the probability value for the dataset");
	}
};

//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:
	Binomial (
		const Params &params		// Distribution parameters
	) : Binomial (params.trials, params.probability)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Probability Mass Function (PMF)                                       //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double PMF (
		size_t x					// Argument value
	) const {
		const double t1 = x * log (probability) - lgamma (x + 1);
		const double t2 = (trials - x) * log (1.0 - probability) - lgamma (trials - x + 1);
		const double temp = gamma_log + t1 + t2;
		return exp (temp);
	}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Binomial (
		size_t trials,				// Number of trials
		double probability			// Probability of a successful trial
	) :	range (0, double (trials)),
		gamma_log (lgamma (trials + 1)),
		probability (probability),
		trials (trials)
	{
		if (trials == 0.0)
			throw invalid_argument ("Binomial: The number of trials must be positive");
		if (probability < 0.0 && probability > 1.0)
			throw invalid_argument ("Binomial: The probability of a successful trial must be in the range [0..1]");
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor for empirical data                                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Binomial (
		const Observations &data	// Empirical observations
	) : Binomial (Params (data))
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Number of trials                                                      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double Trials (void) const {
		return trials;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Probability of a successful trial                                     //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double Probability (void) const {
		return probability;
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
		if (x != range) return 0.0;

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

		// Above the range
		if (x >= range) return 1.0;

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
		const double temp = (trials + 1) * probability;
		return floor (temp);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Mean of the distribution                                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Mean (void) const override final {
		return trials * probability;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Variance of the distribution                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Variance (void) const override final {
		return trials * probability * (1.0 - probability);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Skewness of the distribution                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Skewness (void) const override final {
		const double temp = 1.0 - probability;
		const double p = temp - probability;
		const double q = sqrt (trials * probability * temp);
		return p / q;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Kurtosis of the distribution                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Kurtosis (void) const override final {
		const double temp = 1.0 - probability;
		const double p = 1.0 - 6.0 * probability * temp;
		const double q = trials * probability * temp;
		return p / q;
	}
};

//****************************************************************************//
//      Internal constants used by the class                                  //
//****************************************************************************//
const size_t Binomial::params = 2;

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const Binomial &model)
{
	auto restore = stream.precision();
	stream.precision (PRECISION);
	stream << "\nBINOMIAL DISTRIBUTION:" << std::endl;
	stream << "======================" << std::endl;
	stream << static_cast <const BaseDiscrete&> (model);
	stream << "    Number of trials\t\t\t= " << model.Trials() << endl;
	stream << "    Success probability\t\t\t= " << model.Probability() << endl;
	stream << static_cast <const BaseModel&> (model);
	stream.precision (restore);
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
