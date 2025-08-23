/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#               SHOW THE GUI WINDOW FOR THE OBSERVATIONS SUMMARY               #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"population.hpp"
# include	"sample.hpp"
# include	"../prop_table.hpp"

// Use shortenings
using Glib::ustring;

//****************************************************************************//
//      Class "SummaryWindow"                                                 //
//****************************************************************************//
class SummaryWindow : public Window
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:

	// Multiple tab widget for different groups of properties
	struct Tabs : public Notebook
	{
		// Constructor
		Tabs (
			const vector <PropGroup> &groups	// Groups of properties an object has
		){
			// Set position of the tabs
			set_tab_pos (PositionType::POS_LEFT);

			// Process all the available property groups
			const size_t size = groups.size();
			for (size_t i = 0; i < size; i++) {

				// Use a group name as a tab label, and make this text bold
				auto label = make_managed <Label> ("<b>" + to_string (i + 1) + ". " + groups[i].Name() + "</b>");
				label -> set_use_markup (true);
				label -> set_xalign (0.0);

				// Put one properties table per page
				auto table = make_managed <PropTable> (groups[i].Properties());

				// Create a page with the group properties aggregated
				append_page (*table, *label);
			}
		}
	};
	Tabs notebook;					// Tabs for groups of properties
	Button button;					// Close button
	ActionBar bar;					// The action bar at the bottom of the window
	VBox box;						// Layout widget

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors                                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	SummaryWindow (
		const ustring &title,		// Window titles
		const Observations &data	// The summary data to display
	) :	notebook (data.Summary()),
		button ("Close")
	{
		// Set the window properties
		set_title (title + " (" + to_string (data.Size()) + " data points)");

		// Create the close button
		button.set_image_from_icon_name ("window-close");
		button.set_tooltip_text ("Close the window");
		button.signal_clicked().connect (sigc::mem_fun (*this, &SummaryWindow::close));

		// Add it to the action bar
		bar.pack_end (button);

		// Create the layout
		box.pack_start (notebook, true, true);
		box.pack_start (bar, false, false);

		// Add it to the window
		add (box);

		// Show the main window
		show_all();
	}
};
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
