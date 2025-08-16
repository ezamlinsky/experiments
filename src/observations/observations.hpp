/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                        OBSERVATIONS OF A RANDOM VALUE                        #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"../templates/array.hpp"
# include	"../templates/statistics.hpp"
# include   "../models/range.hpp"

//****************************************************************************//
//      Class "Observations"                                                  //
//****************************************************************************//
class Observations
{
//============================================================================//
//      Members                                                               //
//============================================================================//
protected:
	Range range;					// Values range
	double *array;					// Array of observed values
	size_t size;					// Array size
	double mean;					// Mean value
	double median;					// Median value

//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Trim the observations from the left side                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void trim_left (
		size_t count				// Count of observations to remove
	){
		double *target = array;
		double *source = array + count;
		size_t shift_count = size - count;
		for (size_t i = 0; i < shift_count; i++)
			target [i] = source [i];
		size -= count;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Trim the observations from the right side                             //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void trim_right (
		size_t count				// Count of observations to remove
	){
		size -= count;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Trim the observations from both sides                                 //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void trim_both (
		size_t count				// Count of observations to remove
	){
		trim_left (count);
		trim_right (count);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Store raw observations                                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Observations (
		const double data[],		// Raw observations to work with
		size_t size					// Array size
	) :	range (data, size),
		array (new double [size]),
		size (size)
	{
		// Sort the observations
		Array::Copy (array, data, size);
		Array::Sort (array, size);
	}

//============================================================================//
//      Protected methods                                                     //
//============================================================================//
protected:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Store transformed observations                                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Observations (
		double data[],				// Transformed observations to store
		size_t size					// Array size
	) :	range (data, size),
		array (data),
		size (size)
	{
		// Sort the observations
		Array::Sort (array, size);

		// Estimate the mean and the median values
		Observations::mean = Stats::Mean (array, size);
		Observations::median = Quantile (0.5);
	}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors                                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

	// Statistical sample
	Observations (
		const vector <double> data	// Observations of a random value
	) :	Observations (data.data(), data.size())
	{
		// Estimate the mean and the median values
		Observations::mean = Stats::Mean (array, size);
		Observations::median = Quantile (0.5);
	}

	// Statistical population
	Observations (
		const vector <double> data,	// Observations of a random value
		double mean,				// Population mean
		double median				// Population median
	) :	Observations (data.data(), data.size())
	{
		// Set the mean and the median values
		Observations::mean = mean;
		Observations::median = median;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Copy constructor                                                      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Observations (
		const Observations &source	// The source object to copy
	) :	range (source.range),
		array (new double [source.size]),
		size (source.size),
		mean (source.mean),
		median (source.median)
	{
		// Copy the data
		Array::Copy (array, source.array, source.size);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Move constructor                                                      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Observations (
		Observations &&source		// The source object to move
	) :	range (source.range),
		array (source.array),
		size (source.size),
		mean (source.mean),
		median (source.median)
	{
		// Reset the original object
		source.array = NULL;
		source.size = 0;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Destructor                                                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual ~Observations (void) {
		delete [] array;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Trim the target percent of observed min values                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void TrimMin (
		double percent				// Percent of observations to cut off
	){
		if (0.0 <= percent && percent <= 1.0)
			trim_left (size * percent);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Trim the target percent of observed max values                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void TrimMax (
		double percent				// Percent of observations to cut off
	){
		if (0.0 <= percent && percent <= 1.0)
			trim_right (size * percent);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Trim the target percent of observed min and max values                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void TrimBoth (
		double percent				// Percent of observations to cut off
	){
		if (0.0 <= percent && percent <= 1.0)
			trim_both (size * percent);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Trim all the values less than the target value                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void TrimLess (
		double value				// Threshold value
	){
		trim_left (Array::BinSearchLess (array, size, value) + 1);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Trim all the values less than or equal to the target value            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void TrimLessOrEqual (
		double value				// Threshold value
	){
		trim_left (Array::BinSearchLessOrEqual (array, size, value) + 1);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Trim all the values greater than the target value                     //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void TrimGreater (
		double value				// Threshold value
	){
		trim_right (size - Array::BinSearchGreater (array, size, value));
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Trim all the values greater than or equal to the target value         //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void TrimGreaterOrEqual (
		double value				// Threshold value
	){
		trim_right (size - Array::BinSearchGreaterOrEqual (array, size, value));
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Add a value to all the observations                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void Add (
		double value				// The value to add
	){
		range.Shift (value);
		mean += value;
		median += value;
		Array::Add (array, size, value);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Subtract a value from all the observations                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void Sub (
		double value				// The value to subtract
	){
		range.Shift (-value);
		mean += -value;
		median += -value;
		Array::Sub (array, size, value);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Multiply all the observations by a value                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void Mul (
		double value				// The value to multiply by
	){
		range.Scale (value);
		mean *= value;
		median *= value;
		Array::Mul (array, size, value);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Divide all the observations by a value                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void Div (
		double value				// The value to divide by
	){
		range.Scale (1.0 / value);
		mean *= 1.0 / value;
		median *= 1.0 / value;
		Array::Div (array, size, value);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Count of observed values in the dataset                               //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	size_t Size (void) const {
		return size;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Ranked dataset                                                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	vector <double> Data (void) const {
		return vector <double> (array, array + size);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Values range                                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const Range& Domain (void) const {
		return range;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Quantile value for the target level                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double Quantile (
		double level				// Quantile level to estimate
	) const {

		// Check if the level is correct
		if (0.0 <= level && level <= 1.0) {

			// Get the index value for the target level
			const double index = (size - 1) * level;

			// We do linear interpolation when a value resides between
			// two empirical points
			const size_t pos = index;
			const double gain = index - pos;
			if (gain)
				return (1 - gain) * array [pos] + gain * array [pos + 1];
			else
				return array [pos];
		}
		else
			throw invalid_argument ("Quantile: Level must be in the range [0..1]");
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Lower quartile of the dataset                                         //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double LowerQuartile (void) const {
		return Quantile (0.25);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Upper quartile of the dataset                                         //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double UpperQuartile (void) const {
		return Quantile (0.75);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Inter-quartile range (IQR) of the dataset                             //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double InterQuartileRange (void) const {
		return UpperQuartile() - LowerQuartile();
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Mid-hinge of the dataset                                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double MidHinge (void) const {
		return 0.5 * (UpperQuartile() + LowerQuartile());
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Tukey's tri-mean of the dataset                                       //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double TriMean (void) const {
		return 0.5 * (Median() + MidHinge());
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Quartile skewness of the dataset                                      //
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
//      Median of the dataset                                                 //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double Median (void) const {
		return median;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Mean of the dataset                                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double Mean (void) const {
		return mean;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Median of squared deviations from the mean value                      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double MedianSqrDevFromMean (void) const {
		return Stats::MedianSqrDev (array, size, Mean());
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Median of absolute deviations from the mean value                     //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double MedianAbsDevFromMean (void) const {
		return Stats::MedianAbsDev (array, size, Mean());
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Median of signed deviations from the mean value                       //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double MedianSignDevFromMean (void) const {
		return Stats::MedianSignDev (array, size, Mean());
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Median of squared deviations from the median value                    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double MedianSqrDevFromMedian (void) const {
		return Stats::MedianSqrDev (array, size, Median());
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Median of absolute deviations from the median value                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double MedianAbsDevFromMedian (void) const {
		return Stats::MedianAbsDev (array, size, Median());
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Median of signed deviations from the median value                     //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double MedianSignDevFromMedian (void) const {
		return Stats::MedianSignDev (array, size, Median());
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Excess kurtosis around the mean value                                 //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double KurtosisExcessAroundMean (void) const {
		return KurtosisAroundMean() - 3.0;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Excess kurtosis around the median value                               //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double KurtosisExcessAroundMedian (void) const {
		return KurtosisAroundMedian() - 3.0;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Pearson's skewness estimator of the dataset                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double PearsonSkewness (void) const {
		return 3.0 * (Mean() - Median()) / StdDev();
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Meeden's skewness estimator of the dataset                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double MeedenSkewness (void) const {
		return (Mean() - Median()) / MeanAbsDevFromMedian();
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Convert an instance of a derived class to the base class              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const Observations& data (void) const {
		return *this;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Summary of the object                                                 //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	vector <PropGroup> Summary (void) const {

		// Create the summary storage
		vector <PropGroup> summary;

		// Robust estimators
		PropGroup robust ("Robust estimators");
		robust.Append ("Median", Median());
		robust.Append ("Lower quartile", LowerQuartile());
		robust.Append ("Upper quartile", UpperQuartile());
		robust.Append ("Inter-quartile range", InterQuartileRange());
		robust.Append ("Midhinge", MidHinge());
		robust.Append ("Tukey's trimean", TriMean());
		robust.Append ("Quartile skewness", QuartileSkewness());
		summary.push_back (robust);

		// Standard estimators
		PropGroup standard ("Standard estimators");
		standard.Append ("Mean", Mean());
		standard.Append ("Variance", Variance());
		standard.Append ("Standard deviation", StdDev());
		standard.Append ("Standard error", StdErr());
		summary.push_back (standard);

		// Deviations from the median value
		PropGroup mean_dev ("Deviations from the median value");
		mean_dev.Append ("Mean of squared deviations", MeanSqrDevFromMean());
		mean_dev.Append ("Mean of absolute deviations", MeanAbsDevFromMean());
		mean_dev.Append ("Median of squared deviations", MedianSqrDevFromMean());
		mean_dev.Append ("Median of absolute deviations", MedianAbsDevFromMean());
		mean_dev.Append ("Median of signed deviations", MedianSignDevFromMean());
		summary.push_back (mean_dev);

		// Deviations from the median value
		PropGroup median_dev ("Deviations from the median value");
		median_dev.Append ("Mean of squared deviations", MeanSqrDevFromMedian());
		median_dev.Append ("Mean of absolute deviations", MeanAbsDevFromMedian());
		median_dev.Append ("Mean of signed deviations", MeanSignDevFromMedian());
		median_dev.Append ("Median of squared deviations", MedianSqrDevFromMedian());
		median_dev.Append ("Median of absolute deviations", MedianAbsDevFromMedian());
		median_dev.Append ("Median of signed deviations", MedianSignDevFromMedian());
		summary.push_back (median_dev);

		// Estimators around the mean value
		PropGroup mean_est ("Estimators around the mean value");
		mean_est.Append ("Variation", VariationAroundMean());
		mean_est.Append ("Skewness", SkewnessAroundMean());
		mean_est.Append ("Kurtosis", KurtosisAroundMean());
		mean_est.Append ("Excess kurtosis", KurtosisExcessAroundMean());
		summary.push_back (mean_est);

		// Estimators around the median value
		PropGroup median_est ("Estimators around the median value");
		median_est.Append ("Variation", VariationAroundMedian());
		median_est.Append ("Skewness", SkewnessAroundMedian());
		median_est.Append ("Kurtosis", KurtosisAroundMedian());
		median_est.Append ("Excess kurtosis", KurtosisExcessAroundMedian());
		summary.push_back (median_est);

		// Skewness
		PropGroup skewness ("Skewness");
		skewness.Append ("Pearson's skewness", PearsonSkewness());
		skewness.Append ("Meeden's skewness", MeedenSkewness());
		summary.push_back (skewness);

		// Important quantiles
		PropGroup quantiles ("Important quantiles");
		quantiles.Append ("Quantile (1%)", Quantile (0.01));
		quantiles.Append ("Quantile (5%)", Quantile (0.05));
		quantiles.Append ("Quantile (95%)", Quantile (0.95));
		quantiles.Append ("Quantile (99%)", Quantile (0.99));
		summary.push_back (quantiles);

		// Deciles
		PropGroup deciles ("Deciles");
		for (int i = 1; i <= 9; i++) {
			const string name = string ("Decile (") + to_string (i * 10) + "%)";
			deciles.Append (name, Quantile (i / 10.0));
		}
		summary.push_back (deciles);

		// Return the summary
		return summary;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Virtual functions to override in derivative classes                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Variance (void) const = 0;
	virtual double StdDev (void) const = 0;
	virtual double StdErr (void) const = 0;
	virtual double MeanSqrDevFromMean (void) const = 0;
	virtual double MeanAbsDevFromMean (void) const = 0;
	virtual double MeanSqrDevFromMedian (void) const = 0;
	virtual double MeanAbsDevFromMedian (void) const = 0;
	virtual double MeanSignDevFromMedian (void) const = 0;
	virtual double VariationAroundMean (void) const = 0;
	virtual double SkewnessAroundMean (void) const = 0;
	virtual double KurtosisAroundMean (void) const = 0;
	virtual double VariationAroundMedian (void) const = 0;
	virtual double SkewnessAroundMedian (void) const = 0;
	virtual double KurtosisAroundMedian (void) const = 0;
	virtual void Show (void) const = 0;
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const Observations &object)
{
	stream << "Data points\t\t\t\t= " << object.Size() << endl;
	stream << object.Domain();
	for (const auto &group : object.Summary())
		stream << group;
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
