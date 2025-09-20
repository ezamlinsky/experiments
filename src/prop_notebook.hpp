/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#         CREATE A NOTEBOOK WITH MULTIPLE TABLES OF OBJECT PROPERTIES          #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	<gtkmm.h>
# include	"prop_table.hpp"

// Use shortenings
using namespace Gtk;

//****************************************************************************//
//      Class "PropNotebook"                                                  //
//****************************************************************************//
class PropNotebook : public Notebook
{
//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	PropNotebook (
		const header &columns,	// Column names to display
		const groups &props,	// Groups of properties an object has
		size_t digits			// Number of decimal digits to display for numbers
	){
		// Set position of the tabs
		set_tab_pos (PositionType::POS_LEFT);

		// Process all the available property groups
		size_t i = 1;
		for (const auto &item : props) {

			// Show only groups with a name
			const string &name = item.Name();
			if (name.length()) {

				// Use a group name as a tab label, and make this text bold
				auto label = make_managed <Label> ("<b>" + to_string (i) + ". " + name + "</b>");
				label -> set_use_markup (true);
				label -> set_xalign (0.0);

				// Put one properties table per page
				auto table = make_managed <PropTable> (columns, item.Properties(), digits);

				// Create a page with the group properties aggregated
				append_page (*table, *label);
				i++;
			}
		}
	}
};
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
