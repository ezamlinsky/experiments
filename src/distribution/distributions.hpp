/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#           AGGREGATE MULTIPLE STATISTICAL DISTRIBUTIONS IN ONE SET            #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# include	"continuous_distribution.hpp"

//****************************************************************************//
//      Class "Distributions"                                                 //
//****************************************************************************//
class Distributions
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	vector <shared_ptr <const ContinuousDistribution> > array;

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Default constructor                                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Distributions (void) = default;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Trivial constructor that appends a continuous distribution            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Distributions (
		const ContinuousDistribution &dist	// The distribution to append
	){
		Add (dist);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors for empirical data                                       //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Distributions (
		const vector <double> &data,		// Empirical dataset
		size_t bins							// Bins count for a histogram
	){
		shared_ptr <const ContinuousDistribution> ptr (new ContinuousDistribution (data, bins));
		array.push_back (ptr);
	}

	Distributions (
		const list &py_list,				// Empirical dataset
		size_t bins							// Bins count for a histogram
	) : Distributions (to_vector (py_list), bins)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor for a theoretical model                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Distributions (
		const BaseModel &model,				// Theoretical model of the CDF
		const Range &range,					// Values range
		size_t bins							// Bins count for a histogram
	){
		shared_ptr <const ContinuousDistribution> ptr (new ContinuousDistribution (model, range, bins));
		array.push_back (ptr);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors for empirical data and a theoretical model               //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Distributions (
		const vector <double> &data,		// Empirical dataset
		const BaseModel &model,				// Theoretical model of the CDF
		size_t bins							// Bins count for a histogram
	){
		shared_ptr <const ContinuousDistribution> first (new ContinuousDistribution (data, bins));
		array.push_back (first);
		shared_ptr <const ContinuousDistribution> second (new ContinuousDistribution (model, first -> Domain(), bins));
		array.push_back (second);
	}

	Distributions (
		const list &py_list,				// Empirical dataset
		const BaseModel &model,				// Theoretical model of the CDF
		size_t bins							// Bins count for a histogram
	) : Distributions (to_vector (py_list), model, bins)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors for empirical data only (a sample and a reference)       //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	Distributions (
		const vector <double> &sample,		// Empirical sample data
		const vector <double> &reference,	// Empirical reference data
		size_t bins							// Bins count for a histogram
	){
		shared_ptr <const ContinuousDistribution> first (new ContinuousDistribution (sample, bins));
		array.push_back (first);
		shared_ptr <const ContinuousDistribution> second (new ContinuousDistribution (reference, bins));
		array.push_back (second);
	}

	Distributions (
		const list &sample,					// Empirical sample data
		const list &reference,				// Empirical reference data
		size_t bins							// Bins count for a histogram
	) : Distributions (to_vector (sample), to_vector (reference), bins)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Count of stored distribution in the set                               //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	int Size (void) const {
		return array.size();
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Add a continuous distribution to the set                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void Add (
		const ContinuousDistribution &dist	// The distribution to add
	){
		shared_ptr <const ContinuousDistribution> ptr (new ContinuousDistribution (dist));
		array.push_back (ptr);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Get a continuous distribution by its index in the set                 //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const ContinuousDistribution& Get (
		size_t index						// Index of a distribution in the set
	) const {
		return *(array.at (index));
	}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const Distributions &object)
{
	auto restore = stream.precision();
	stream.precision (PRECISION);
	stream << "\nSET OF DISTRIBUTIONS:" << endl;
	stream << "=====================" << endl;
	stream << "Distributions\t\t\t\t= " << object.Size() << endl;
	stream.precision (restore);
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
