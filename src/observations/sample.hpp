/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                              STATISTICAL SAMPLE                              #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"../python_helpers.hpp"
# include	"observations.hpp"
# include	"../models/confidence_interval.hpp"
# include	"../models/discrete/binomial.hpp"
# include	"../models/continuous/normal.hpp"

//****************************************************************************//
//      Class "Sample"                                                        //
//****************************************************************************//
class Sample : public Observations
{
//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Private constructor                                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Sample (
		double data[],				// Transformed observations to store
		size_t size					// Array size
	) :	Observations (data, size)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Confidence interval for the quantile                                  //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	ConfidenceInterval QuantileConfidenceInterval (
		double p,					// Quantile level to estimate
		double level				// Confidence level
	) const {
		// Check if the level is correct
		if (0.0 <= level && level <= 1.0) {
			const auto dist = Binomial (size, p);
			const double value = Quantile (p);
			const double alpha = 1.0 - level;
			const size_t quantile1 = dist.FloorQuantile (0.5 * alpha);
			const size_t quantile2 = dist.CeilQuantile (1.0 - 0.5 * alpha);
			const double lower = array [quantile1];
			const double upper = array [quantile2];
			const double precise_level = dist.CDF (quantile2) - dist.CDF (quantile1);
			return ConfidenceInterval (precise_level, value, Range (lower, upper));
		}
		else
			throw invalid_argument ("QuantileConfidenceInterval: The confidence level must be in the range [0..1]");
	}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors                                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Sample (
		const vector <double> data	// Sample data
	) :	Observations (data)
	{}

	Sample (
		const pylist &py_list		// Sample data
	) : Sample (to_vector (py_list))
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Copy constructor                                                      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Sample (
		const Sample &source		// The source object to copy
	) :	Observations (static_cast <const Observations&> (source))
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Move constructor                                                      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Sample (
		Sample &&source				// The source object to move
	) :	Observations (static_cast <Observations&&> (source))
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Absolute value transformation                                         //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Sample Abs (void) const {

		// Create a new array for transformed data
		double *data = new double [size];

		// Return transformed observations
		return Sample (Array::Abs (data, array, size), size);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Logarithmic transformation                                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Sample Log (void) const {

		// Check if the transformation is possible for the data
		if (range <= 0.0)
			throw invalid_argument ("Log: Can not transform observations with non-positive values");

		// Create a new array for transformed data
		double *data = new double [size];

		// Return transformed observations
		return Sample (Array::Log (data, array, size), size);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Exponential transformation                                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Sample Exp (void) const {

		// Create a new array for transformed data
		double *data = new double [size];

		// Return transformed observations
		return Sample (Array::Exp (data, array, size), size);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Power transformation                                                  //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Sample Pow (
		double power				// The power to use for the transformation
	) const {

		// Check if the transformation is possible for the data
		if (range <= 0.0)
			throw invalid_argument ("Pow: Can not transform observations with non-positive values");

		// Create a new array for transformed data
		double *data = new double [size];

		// Return transformed observations
		return Sample (Array::Pow (data, array, size, power), size);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Box-Cox transformation                                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Sample BoxCox (
		double power				// The power to use for the transformation
	) const {

		// Check if the transformation is possible for the data
		if (range <= 0.0)
			throw invalid_argument ("BoxCox: Can not transform observations with non-positive values");

		// Special case
		if (power == 0.0)
			return Log();

		// Create a new array for transformed data
		double *data = new double [size];

		// Fill it with the transformed data
		for (size_t i = 0; i < size; i++)
			data [i] = (pow (array[i], power) - 1.0) / power;

		// Return transformed observations
		return Sample (data, size);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Variance of the dataset                                               //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Variance (void) const override final {
		return Stats::SqrDevSample (array, size, Mean());
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Standard deviation of the dataset                                     //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double StdDev (void) const override final {
		return Stats::StdDevSample (array, size, Mean());
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Standard error of the dataset                                         //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double StdErr (void) const override final {
		return Stats::StdErrSample (array, size, Mean());
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Mean of squared deviations from the mean value                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double MeanSqrDevFromMean (void) const override final {
		return Stats::SqrDevSample (array, size, Mean());
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Mean of absolute deviations from the mean value                       //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double MeanAbsDevFromMean (void) const override final {
		return Stats::AbsDevSample (array, size, Mean());
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Mean of squared deviations from the median value                      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double MeanSqrDevFromMedian (void) const override final {
		return Stats::SqrDevSample (array, size, Median());
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Mean of absolute deviations from the median value                     //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double MeanAbsDevFromMedian (void) const override final {
		return Stats::AbsDevSample (array, size, Median());
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Mean of signed deviations from the median value                       //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double MeanSignDevFromMedian (void) const override final {
		return Stats::SignDevSample (array, size, Median());
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Variation around the mean value                                       //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double VariationAroundMean (void) const override final {
		return Stats::VariationSample (array, size, Mean());
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Skewness around the mean value                                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double SkewnessAroundMean (void) const override final {
		return Stats::SkewnessSample (array, size, Mean());
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Kurtosis around the mean value                                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double KurtosisAroundMean (void) const override final {
		return Stats::KurtosisSample (array, size, Mean());
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Variation around the median value                                     //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double VariationAroundMedian (void) const override final {
		return Stats::VariationSample (array, size, Median());
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Skewness around the median value                                      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double SkewnessAroundMedian (void) const override final {
		return Stats::SkewnessSample (array, size, Median());
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Kurtosis around the median value                                      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double KurtosisAroundMedian (void) const override final {
		return Stats::KurtosisSample (array, size, Median());
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Confidence interval for the mean                                      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	ConfidenceInterval Mean_CI (
		double level				// Confidence level
	) const {
		// Check if the level is correct
		if (0.0 <= level && level <= 1.0) {
			const auto dist = Normal (0.0, 1.0);
			const double std_error = StdErr();
			const double alpha = 1.0 - level;
			const double quantile = dist.Quantile (1.0 - 0.5 * alpha);
			const double lower = mean - std_error * quantile;
			const double upper = mean + std_error * quantile;
			return ConfidenceInterval (level, mean, Range (lower, upper));
		}
		else
			throw invalid_argument ("MeanConfidenceInterval: The confidence level must be in the range [0..1]");
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Confidence interval for the median                                    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	ConfidenceInterval Median_CI (
		double level				// Confidence level
	) const {
		return QuantileConfidenceInterval (0.5, level);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Confidence interval for the lower quartile                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	ConfidenceInterval LowerQuartile_CI (
		double level				// Confidence level
	) const {
		return QuantileConfidenceInterval (0.25, level);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Confidence interval for the upper quartile                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	ConfidenceInterval UpperQuartile_CI (
		double level				// Confidence level
	) const {
		return QuantileConfidenceInterval (0.75, level);
	}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const Sample &object)
{
	stream << object.Summary ("Sample");
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
