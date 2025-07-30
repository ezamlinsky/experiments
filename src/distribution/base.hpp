/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#         CONTAIN A SAMPLE AND A REFERENCE DISTRIBUTION FOR COMPARISON         #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	<cmath>
# include	"distribution.hpp"

//****************************************************************************//
//      Class "BaseComparator"                                                //
//****************************************************************************//
class BaseComparator
{
//============================================================================//
//      Members                                                               //
//============================================================================//
protected:
	Distribution sample;					// Sample distribution to compare
	Distribution reference;					// Reference distribution

//============================================================================//
//      Protected methods                                                     //
//============================================================================//
protected:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors                                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

	// Discrete distribution
	BaseComparator (
		const Observations &data			// Observations of a random value
	) : sample (data)
	{}

	// Discrete distribution
	BaseComparator (
		const vector <double> &data			// Empirical data
	) : sample (data)
	{}

	// Continuous distribution
	BaseComparator (
		const Observations &data,			// Observations of a random value
		size_t bins							// Bins count for a histogram
	) : sample (data, bins)
	{}

	// Continuous distribution
	BaseComparator (
		const vector <double> &data,		// Empirical data
		size_t bins							// Bins count for a histogram
	) : sample (data, bins)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Load a distribution model as a reference for the distribution test    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	template <typename T>
	void ReferenceModel (
		const T &model						// Theoretical model
	){
		// Check if empirical data range is inside the model domain
		const Range &model_range = model.Domain();
		if (model_range.IsInside (sample.Domain())) {

			// Set the distribution model
			reference = Distribution (model, sample.Values());
		}
		else
			throw invalid_argument ("ReferenceModel: The sample data range is outside the distribution model domain");
	}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Return sample distribution                                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const Distribution& Sample (void) const {
		return sample;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Return reference distribution                                         //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const Distribution& Reference (void) const {
		return reference;
	}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const BaseComparator &object)
{
	auto restore = stream.precision();
	stream.precision (PRECISION);
	stream << "\nDISTRIBUTION COMPARATOR:" << endl;
	stream << "========================" << endl;
	stream << "Sample data points\t\t\t= " << object.Sample().Bins() << endl;
	stream << "Reference data points\t\t\t= " << object.Reference().Bins() << endl;
	stream.precision (restore);
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
