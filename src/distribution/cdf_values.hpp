/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#   CALCULATE A CUMULATIVE DISTRIBUTION FUNCTION (CDF) USING DATA OR A MODEL   #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"base.hpp"
# include	"../models/base.hpp"
# include	"../models/kolmogorov.hpp"

//****************************************************************************//
//      Class "CDFValues"                                                     //
//****************************************************************************//
class CDFValues : public BaseDistribution
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	bool theoretical;		// If set, then the CDF is calculated theoretically

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Default constructor                                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	CDFValues (void)
	:	theoretical (false)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors for empirical data                                       //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	CDFValues (
		const Observations &sample		// Observations of a random value
	) : BaseDistribution (sample),
		theoretical (false)
	{}

	CDFValues (
		const vector <double> &data		// Empirical dataset
	) : BaseDistribution (move (vector <double> (data))),
		theoretical (false)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor for a theoretical model                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	CDFValues (
		const Range &range,				// Values range
		const vector <double> &values,	// Unique values
		const BaseModel &model			// Theoretical model of the CDF
	) :	BaseDistribution (range, values),
		theoretical (true)
	{
		// Fill the theoretical CDF table
		for (const auto x : values)
			cdf.push_back (model.CDF (x));
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Find a value for the CDF function for an arbitrary Х                  //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double GetCDF (
		double x						// Argument value
	) const {

		// The argument is less than the minimum value
		if (x < values[0])
			return 0.0;

		// Binary search for the greater value: v[i] <= x
		size_t left = 0;
		size_t right = values.size();
		while (left < right) {
			const size_t mid = (left + right) / 2;
			if (values [mid] <= x)
				left = mid + 1;
			else
				right = mid;
		}
		return cdf [left - 1];
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Indicate that the CDF function has been calculated theoretically      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	bool IsTheoretical (void) const {
		return theoretical;
	}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const CDFValues &object)
{
	auto restore = stream.precision();
	stream.precision (PRECISION);
	if (object.IsTheoretical()) {
		stream << "\nTHEORETICAL CDF VALUES:" << endl;
		stream << "=======================" << endl;
	}
	else {
		stream << "\nEMPIRICAL CDF VALUES:" << endl;
		stream << "=====================" << endl;
	}
	stream << "Data points\t\t\t\t= " << object.Size() << endl;
	stream << object.Domain();
	stream.precision (restore);
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
