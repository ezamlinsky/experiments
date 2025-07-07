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

// Value of the step out of a point where the derivative is infinite
# define	STEP_OUTSIDE		0.001

// The number of iterations for the Newton solve method
# define	NEWTON_ITERATIONS	8

//****************************************************************************//
//      Class "BaseModel"                                                     //
//****************************************************************************//
class BaseModel
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	const Range range;

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	BaseModel (
		const Range &range	// Function domain where the distribution exists
	) : range (range)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Destructor                                                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual ~BaseModel (void) = default;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Function domain where the distribution exists                         //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const Range& Domain (void) const {
		return range;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Generate random values from the distribution                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	vector <double> Generate (
		size_t count		// Size of the sample to generate
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
//      Quantile value for the target level                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double Quantile (
		double level		// Quantile level to estimate
	) const {

		// Check if the level is correct
		if (0.0 <= level and level <= 1.0) {

			// The first approach is the mean value if exists
			double x = Mean();
			if (isinf (x)) x = 0;

			// Find a solution using the Newton solve method
			int i = NEWTON_ITERATIONS;
			while (i--) {

				// Calculate the function and its derivative
				const double func = CDF (x);
				const double der = PDF (x);

				// Check the distance between the function and the target value
				const double diff = func - level;

				// Calculate a step value to move for the next point
				double step = diff / der;

				// If the derivative is infinite in the target point
				// then do a step outside the pit
				if (der == INFINITY)
					step = +STEP_OUTSIDE * diff;
				else if (der == -INFINITY)
					step = -STEP_OUTSIDE * diff;

				// A new approach
				x -= step;

				// Clamp the value inside the distribution domain
				x = range.Clamp (x);
			}
			return x;
		}
		else
			throw invalid_argument ("Quantile: Level must be in the range [0..1]");
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Median of the distribution                                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double Median (void) const {
		return Quantile (0.5);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Lower quartile of the distribution                                    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double LowerQuartile (void) const {
		return Quantile (0.25);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Upper quartile of the distribution                                    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double UpperQuartile (void) const {
		return Quantile (0.75);
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
//      Virtual functions to override in derivative classes                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double PDF (double x) const = 0;
	virtual double CDF (double x) const = 0;
	virtual double Mean (void) const = 0;
	virtual double Variance (void) const = 0;
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const BaseModel &model)
{
	auto restore = stream.precision();
	stream.precision (PRECISION);
	stream << model.Domain();
	stream << "\nStandard metrics:" << endl;
	stream << "~~~~~~~~~~~~~~~~~" << endl;
	stream << "    Mean\t\t\t\t= " << model.Mean() << endl;
	stream << "    Variance\t\t\t\t= " << model.Variance() << endl;
	stream << "    Standard deviation\t\t\t= " << model.StdDev() << endl;
	stream << "\nRobust metrics:" << endl;
	stream << "~~~~~~~~~~~~~~~" << endl;
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
