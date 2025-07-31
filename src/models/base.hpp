/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                  BASE CLASS FOR ALL THE DISTRIBUTION MODELS                  #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	<cmath>
# include	<random>
# include	"range.hpp"
# include	"../observations/observations.hpp"

//****************************************************************************//
//      Class "BaseModel"                                                     //
//****************************************************************************//
class BaseModel
{
//============================================================================//
//      Members                                                               //
//============================================================================//
public:

// Distribution type
enum DistType {
	DISCRETE,					// Discrete distribution
	CONTINUOUS					// Continuous distribution
};

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	BaseModel (void) = default;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Destructor                                                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual ~BaseModel (void) = default;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Generate random values from the distribution                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	vector <double> Generate (
		size_t count			// Size of the sample to generate
	){
		// The accumulator for random values
		vector <double> result;

		// Obtain a seed for the random number engine
		random_device rd;

		// Seed the standard mersenne twister engine
		mt19937 gen (rd());

		// Collect uniformly distributed random values in the range [0, 1)
		uniform_real_distribution <> uniform (0.0, 1.0);
		while (count--) {

			// Translate uniform distribution into the target distribution
			const double rnd = uniform (gen);
			result.push_back (Quantile (rnd));
		}

		// Return all the generated random values
		return result;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Median of the distribution                                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double Median (void) const {
		return Quartile (0.5);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Lower quartile of the distribution                                    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double LowerQuartile (void) const {
		return Quartile (0.25);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Upper quartile of the distribution                                    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double UpperQuartile (void) const {
		return Quartile (0.75);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Inter-quartile range (IQR) of the distribution                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double InterQuartileRange (void) const {
		return UpperQuartile() - LowerQuartile();
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Mid-hinge of the distribution                                         //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double MidHinge (void) const {
		return 0.5 * (UpperQuartile() + LowerQuartile());
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Tukey's tri-mean of the distribution                                  //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double TriMean (void) const {
		return 0.5 * (Median() + MidHinge());
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Quartile skewness of the distribution                                 //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double QuartileSkewness (void) const {
		const double lower = LowerQuartile();
		const double upper = UpperQuartile();
		const double range = upper - lower;
		if (range)
			return (upper + lower - 2.0 * Median()) / range;
		else
			return 0.0;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Standard deviation of the distribution                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double StdDev (void) const {
		return sqrt (Variance());
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Variation around the mean value                                       //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double Variation (void) const {
		return StdDev() / Mean();
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Virtual functions to override in derivative classes                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual DistType Type (void) const = 0;
	virtual const Range& Domain (void) const = 0;
	virtual size_t Parameters (void) const = 0;
	virtual double Quartile (double level) const = 0;
	virtual double Quantile (double level) const = 0;
	virtual double PDF (double x) const = 0;
	virtual double CDF (double x) const = 0;
	virtual double Mode (void) const = 0;
	virtual double Mean (void) const = 0;
	virtual double Variance (void) const = 0;
	virtual double Skewness (void) const = 0;
	virtual double Kurtosis (void) const = 0;
	virtual const BaseModel& data (void) const = 0;
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const BaseModel &model)
{
	auto restore = stream.precision();
	stream.precision (PRECISION);
	stream << model.Domain();
	stream << "\nStandard estimators:" << endl;
	stream << "~~~~~~~~~~~~~~~~~~~~" << endl;
	stream << "    Mode\t\t\t\t= " << model.Mode() << endl;
	stream << "    Mean\t\t\t\t= " << model.Mean() << endl;
	stream << "    Variance\t\t\t\t= " << model.Variance() << endl;
	stream << "    Standard deviation\t\t\t= " << model.StdDev() << endl;
	stream << "    Variation\t\t\t\t= " << model.Variation() << endl;
	stream << "    Skewness\t\t\t\t= " << model.Skewness() << endl;
	stream << "    Kurtosis\t\t\t\t= " << model.Kurtosis() << endl;
	stream << "\nRobust estimators:" << endl;
	stream << "~~~~~~~~~~~~~~~~~~" << endl;
	stream << "    Median\t\t\t\t= " << model.Median() << endl;
	stream << "    Lower quartile\t\t\t= " << model.LowerQuartile() << endl;
	stream << "    Upper quartile\t\t\t= " << model.UpperQuartile() << endl;
	stream << "    Inter-quartile range\t\t= " << model.InterQuartileRange() << endl;
	stream << "    Midhinge\t\t\t\t= " << model.MidHinge() << endl;
	stream << "    Tukey's trimean\t\t\t= " << model.TriMean() << endl;
	stream << "    Quartile skewness\t\t\t= " << model.QuartileSkewness() << endl;
	stream << "\nQuantiles:" << endl;
	stream << "~~~~~~~~~~" << endl;
	stream << "    Quantile (1%)\t\t\t= " << model.Quantile (0.01) << endl;
	stream << "    Quantile (5%)\t\t\t= " << model.Quantile (0.05) << endl;
	stream << "    Quantile (95%)\t\t\t= " << model.Quantile (0.95) << endl;
	stream << "    Quantile (99%)\t\t\t= " << model.Quantile (0.99) << endl;
	stream << "\nDeciles:" << endl;
	stream << "~~~~~~~~" << endl;
	for (int i = 1; i <= 9; i++)
		stream << "    Decile (" << i * 10 << "%)\t\t\t= " << model.Quantile (i / 10.0) << endl;
	stream.precision (restore);
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
