/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#        CALCULATE PDF AND CDF FOR A DISCRETE PROBABILITY DISTRIBUTION         #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"../python_helpers.hpp"
# include	"../models/base.hpp"
# include	"../observations/observations.hpp"

//****************************************************************************//
//      Class "DiscreteDistribution"                                          //
//****************************************************************************//
class DiscreteDistribution
{
//============================================================================//
//      Members                                                               //
//============================================================================//
protected:
	Range range;						// Values range
	vector <double> values;				// Unique values
	vector <double> pdf;				// Computed values of a PDF function
	vector <double> cdf;				// Computed values of a CDF function

//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:
	void Init (
		const vector <double> &data		// Empirical data for the calculation
	){
		// Calculate empirical discrete PDF and CDF values
		size_t count = 0;
		size_t total = 0;
		double last_val = data [0];
		double last_cdf = 0.0;
		const size_t size = data.size();
		for (size_t i = 0; i < size; i++) {

			// Get the current value to compare with the last one checked
			const double cur_val = data [i];
			if (cur_val != last_val) {

				// Found another unique value
				total += count;
				const double curr_cdf = double (total) / double (size);
				values.push_back (last_val);
				pdf.push_back (curr_cdf - last_cdf);
				cdf.push_back (curr_cdf);

				// Remember the last value of the CDF function for the next PDF value
				last_cdf = curr_cdf;
				count = 1;
			}

			// The same value
			else count++;

			// Update the last checked value
			last_val = cur_val;
		}

		// Finalize the PDF and CDF tables
		total += count;
		const double curr_cdf = double (total) / double (size);
		values.push_back (last_val);
		pdf.push_back (curr_cdf - last_cdf);
		cdf.push_back (curr_cdf);
	}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Default constructor                                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	DiscreteDistribution (void) = default;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor for a theoretical model                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	DiscreteDistribution (
		const vector <double> &values,	// Unique values
		const BaseModel &model			// Theoretical model
	) :	range (values),
		values (values)
	{
		// Fill the theoretical CDF table
		double last_cdf = model.CDF (range.Min());
		for (const auto x : values) {
			const double cur_cdf = model.CDF (x);
			pdf.push_back (cur_cdf - last_cdf);
			cdf.push_back (cur_cdf);
			last_cdf = cur_cdf;
		}
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors for empirical data                                       //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	DiscreteDistribution (
		const Observations &sample		// Empirical dataset
	) : range (sample.Domain())
	{
		// Extract the ranked dataset
		const vector <double> &data = sample.Data();

		// Check if the dataset is not empty
		if (data.empty())
			throw invalid_argument ("BaseDiscrete: There are no empirical observations to calculate PDF and CDF functions");

		// Calculate empirical discrete PDF and CDF values
		Init (data);
	}

	DiscreteDistribution (
		vector <double> &&data			// Empirical dataset
	) : range (data)
	{
		// Check if the dataset is not empty
		if (data.empty())
			throw invalid_argument ("BaseDiscrete: There are no empirical observations to calculate PDF and CDF functions");

		// Sort the sample
		sort (data.begin(), data.end());

		// Calculate empirical discrete PDF and CDF values
		Init (data);
	}

	DiscreteDistribution (
		const vector <double> &data		// Empirical dataset
	) :	DiscreteDistribution (vector <double> (data))
	{}

	DiscreteDistribution (
		const list &py_list				// Empirical dataset
	) : DiscreteDistribution (to_vector (py_list))
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Destructor                                                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual ~DiscreteDistribution (void) = default;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Domain of the CDF                                                     //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const Range& Domain (void) const {
		return range;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Count of CDF data points for which we have values                     //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	size_t Size (void) const {
		return cdf.size();
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Unique values in the dataset                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const vector <double>& Values (void) const {
		return values;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Values of the PDF function for the dataset                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const vector <double>& PDF (void) const {
		return pdf;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Values of the CDF function for the dataset                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const vector <double>& CDF (void) const {
		return cdf;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Find a value of the PDF function for an arbitrary Х                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double GetPDF (
		double x						// Argument value
	) const {

		 // Find the greatest element that is still less than the target value
		const size_t index = Array::BinSearchLess (values.data(), values.size(), x);
		return index != static_cast <size_t> (-1) ? pdf [index] : 0.0;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Find a value of the CDF function for an arbitrary Х                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double GetCDF (
		double x						// Argument value
	) const {

		 // Find the greatest element that is still less than the target value
		const size_t index = Array::BinSearchLess (values.data(), values.size(), x);
		return index != static_cast <size_t> (-1) ? cdf [index] : 0.0;
	}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const DiscreteDistribution &object)
{
	auto restore = stream.precision();
	stream.precision (PRECISION);
	stream << "\nDISCRETE DISTRIBUTION:" << endl;
	stream << "======================" << endl;
	stream << "Bins\t\t\t\t\t= " << object.Size() << endl;
	stream << object.Domain();
	stream.precision (restore);
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
