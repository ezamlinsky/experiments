/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#               SHOW THE GUI WINDOW WITH THE OBJECT INFORMATION                #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"range.hpp"
# include	"../prop_table.hpp"
# include	"../summary_window.hpp"

//****************************************************************************//
//      Name space "Model"                                                    //
//****************************************************************************//
namespace Model
{
// Use shortenings
using ::Range;

//****************************************************************************//
//      Class "RangeWindow"                                                   //
//****************************************************************************//
class RangeWindow : public SummaryWindow
{
//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors                                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	RangeWindow (
		const Range &data				// The object to display
	) :	SummaryWindow ("Range", make_managed <PropTable> (data.Summary()[0].Properties()))
	{
		// Show the window
		show_all();
	}

	RangeWindow (
		const ustring &name,			// Window name
		const Range &data				// The object to display
	) :	SummaryWindow (name, "Range", make_managed <PropTable> (data.Summary()[0].Properties()))
	{
		// Show the window
		show_all();
	}
};

//****************************************************************************//
//      Class "ConfidenceIntervalWindow"                                      //
//****************************************************************************//
class ConfidenceIntervalWindow : public SummaryWindow
{
//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors                                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	ConfidenceIntervalWindow (
		const ConfidenceInterval &data	// The object to display
	) :	SummaryWindow ("Confidence interval", make_managed <PropTable> (data.Summary()[0].Properties()))
	{
		// Show the window
		show_all();
	}

	ConfidenceIntervalWindow (
		const ustring &name,			// Window name
		const ConfidenceInterval &data	// The object to display
	) :	SummaryWindow (name, "Confidence interval", make_managed <PropTable> (data.Summary()[0].Properties()))
	{
		// Show the window
		show_all();
	}
};
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
