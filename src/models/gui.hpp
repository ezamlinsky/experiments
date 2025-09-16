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
//****************************************************************************//
//      Class "SimpleSummary"                                                 //
//****************************************************************************//
class SimpleSummary : public SummaryWindow
{
//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors                                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	SimpleSummary (
		const ObjectSummary &summary	// Summary info to display
	) :	SummaryWindow (summary.Name(), make_managed <PropTable> (summary.Groups()[0].Properties()))
	{
		// Show the window
		show_all();
	}

	SimpleSummary (
		const ustring &name,			// Window name
		const ObjectSummary &summary	// Summary info to display
	) :	SummaryWindow (name, summary.Name(), make_managed <PropTable> (summary.Groups()[0].Properties()))
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
