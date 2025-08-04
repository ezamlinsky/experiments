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
	const Range range;			// Values range
	double *array;				// Array of observed values
	size_t size;				// Array size

//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Trim the observations from the left side                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void trim_left (
		size_t count			// Count of observations to remove
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
		size_t count			// Count of observations to remove
	){
		size -= count;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Trim the observations from both sides                                 //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void trim_both (
		size_t count			// Count of observations to remove
	){
		trim_left (count);
		trim_right (count);
	}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Observations (
		const double data[],	// Raw observations to work with
		size_t size				// Array size
	) :	range (data, size),
		size (size)
	{
		array = new double [size];
		Array::Copy (array, data, size);
		Array::Sort (array, size);
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
		double percent			// Percent of observations to cut off
	){
		if (0.0 <= percent && percent <= 1.0)
			trim_left (size * percent);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Trim the target percent of observed max values                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void TrimMax (
		double percent			// Percent of observations to cut off
	){
		if (0.0 <= percent && percent <= 1.0)
			trim_right (size * percent);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Trim the target percent of observed min and max values                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void TrimBoth (
		double percent			// Percent of observations to cut off
	){
		if (0.0 <= percent && percent <= 1.0)
			trim_both (size * percent);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Trim all the values less than the target value                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void TrimLess (
		double value			// Threshold value
	){
		trim_left (Array::BinSearchLess (array, size, value) + 1);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Trim all the values less than or equal to the target value            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void TrimLessOrEqual (
		double value			// Threshold value
	){
		trim_left (Array::BinSearchLessOrEqual (array, size, value) + 1);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Trim all the values greater than the target value                     //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void TrimGreater (
		double value			// Threshold value
	){
		trim_right (size - Array::BinSearchGreater (array, size, value));
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Trim all the values greater than or equal to the target value         //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void TrimGreaterOrEqual (
		double value			// Threshold value
	){
		trim_right (size - Array::BinSearchGreaterOrEqual (array, size, value));
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
		double level		// Quantile level to estimate
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
//      Virtual functions to override in derivative classes                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Median (void) const = 0;
	virtual double Mean (void) const = 0;
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
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const Observations &object)
{
	stream << "Data points\t\t\t\t= " << object.Size() << endl;
	stream << object.Domain();
	stream << "\nRobust estimators:" << endl;
	stream << "~~~~~~~~~~~~~~~~~~" << endl;
	stream << "    Median\t\t\t\t= " << object.Median() << endl;
	stream << "    Lower quartile\t\t\t= " << object.LowerQuartile() << endl;
	stream << "    Upper quartile\t\t\t= " << object.UpperQuartile() << endl;
	stream << "    Inter-quartile range\t\t= " << object.InterQuartileRange() << endl;
	stream << "    Midhinge\t\t\t\t= " << object.MidHinge() << endl;
	stream << "    Tukey's trimean\t\t\t= " << object.TriMean() << endl;
	stream << "    Quartile skewness\t\t\t= " << object.QuartileSkewness() << endl;
	stream << "\nStandard estimators:" << endl;
	stream << "~~~~~~~~~~~~~~~~~~~~" << endl;
	stream << "    Mean\t\t\t\t= " << object.Mean() << endl;
	stream << "    Variance\t\t\t\t= " << object.Variance() << endl;
	stream << "    Standard deviation\t\t\t= " << object.StdDev() << endl;
	stream << "    Standard error\t\t\t= " << object.StdErr() << endl;
	stream << "\nDeviations from the mean value:" << endl;
	stream << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	stream << "    Mean of squared deviations\t\t= " << object.MeanSqrDevFromMean() << endl;
	stream << "    Mean of absolute deviations\t\t= " <<  object.MeanAbsDevFromMean() << endl;
	stream << "    Median of squared deviations\t= " << object.MedianSqrDevFromMean() << endl;
	stream << "    Median of absolute deviations\t= " << object.MedianAbsDevFromMean() << endl;
	stream << "    Median of signed deviations\t\t= " << object.MedianSignDevFromMean() << endl;
	stream << "\nDeviations from the median value:" << endl;
	stream << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	stream << "    Mean of squared deviations\t\t= " << object.MeanSqrDevFromMedian() << endl;
	stream << "    Mean of absolute deviations\t\t= " << object.MeanAbsDevFromMedian() << endl;
	stream << "    Mean of signed deviations\t\t= " << object.MeanSignDevFromMedian() << endl;
	stream << "    Median of squared deviations\t= " << object.MedianSqrDevFromMedian() << endl;
	stream << "    Median of absolute deviations\t= " << object.MedianAbsDevFromMedian() << endl;
	stream << "    Median of signed deviations\t\t= " << object.MedianSignDevFromMedian() << endl;
	stream << "\nEstimators around the mean value:" << endl;
	stream << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	stream << "    Variation\t\t\t\t= " << object.VariationAroundMean() << endl;
	stream << "    Skewness\t\t\t\t= " << object.SkewnessAroundMean() << endl;
	stream << "    Kurtosis\t\t\t\t= " << object.KurtosisAroundMean() << endl;
	stream << "    Excess kurtosis\t\t\t= " << object.KurtosisExcessAroundMean() << endl;
	stream << "\nEstimators around the median value:" << endl;
	stream << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	stream << "    Variation\t\t\t\t= " << object.VariationAroundMedian() << endl;
	stream << "    Skewness\t\t\t\t= " << object.SkewnessAroundMedian() << endl;
	stream << "    Kurtosis\t\t\t\t= " << object.KurtosisAroundMedian() << endl;
	stream << "    Excess kurtosis\t\t\t= " << object.KurtosisExcessAroundMedian() << endl;
	stream << "\nSkewness:" << endl;
	stream << "~~~~~~~~~" << endl;
	stream << "    Pearson's skewness\t\t\t= " << object.PearsonSkewness() << endl;
	stream << "    Meeden's skewness\t\t\t= " << object.MeedenSkewness() << endl;
	stream << "\nQuantiles:" << endl;
	stream << "~~~~~~~~~~" << endl;
	stream << "    Quantile (1%)\t\t\t= " << object.Quantile (0.01) << endl;
	stream << "    Quantile (5%)\t\t\t= " << object.Quantile (0.05) << endl;
	stream << "    Quantile (95%)\t\t\t= " << object.Quantile (0.95) << endl;
	stream << "    Quantile (99%)\t\t\t= " << object.Quantile (0.99) << endl;
	stream << "\nDeciles:" << endl;
	stream << "~~~~~~~~" << endl;
	for (int i = 1; i <= 9; i++)
		stream << "    Decile (" << i * 10 << "%)\t\t\t= " << object.Quantile (i / 10.0) << endl;
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
