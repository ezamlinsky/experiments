/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                  A DISTRIBUTION MODEL WITH SCALE PARAMETER                   #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"continuous.hpp"

//****************************************************************************//
//      Class "ScaleDist"                                                     //
//****************************************************************************//
class ScaleDist : public BaseContinuous
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	static const size_t params;	// Count of distribution parameters

protected:
	static const Range range;	// Function domain where the distribution exists
	const double scale;			// Scale of the distribution

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	ScaleDist (
		double scale			// Scale of the distribution
	) :	scale (scale)
	{
		if (scale <= 0.0)
			throw invalid_argument ("Scale: The scale value must be positive");
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Check if the range is inside the model domain                         //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	static bool InDomain (
		const Range &subrange	// Testing range
	){
		return range >= subrange;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Scale of the distribution                                             //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double Scale (void) const {
		return scale;
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
};

//****************************************************************************//
//      Internal constants used by the class                                  //
//****************************************************************************//
const Range ScaleDist::range = Range (0.0, INFINITY);
const size_t ScaleDist::params = 1;

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const ScaleDist &model)
{
	stream << "    Scale\t\t\t\t= " << model.Scale() << endl;
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
