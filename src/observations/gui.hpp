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
# include	<format>
# include	<gtkmm.h>

// Use shortenings
using namespace std;
using namespace Gtk;
using Glib::ustring;
using Glib::RefPtr;

//****************************************************************************//
//      Class "PropsTable"                                                    //
//****************************************************************************//
class PropsTable : public TreeView
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:

	// Storage for the table of properties
	struct Storage : public ListStore
	{
		// Data model for properties columns
		struct Columns : public TreeModelColumnRecord
		{
			// Members
			TreeModelColumn <ustring> property;
			TreeModelColumn <ustring> value;

			// Constructor
			Columns (void) {
				add (property);
				add (value);
			}
		};
		const Columns columns;		// Table columns

		// Constructor
		Storage (
			const properties &props	// List of properties to display
		){
			// Set types of columns
			set_column_types (columns);

			// Fill the storage with data
			for (const auto &opt : props) {
				auto row = *append();
				row.set_value (columns.property, ustring (opt.first + ":"));
				row.set_value (columns.value, ustring (format("{:.15g}", opt.second)));
			}
		}
	};
	RefPtr <Storage> storage;		// Data storage for table data

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors                                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	PropsTable (
		const properties &props		// List of properties to display
	) : storage (new Storage (props))
	{
		// Set table data
		set_model (storage);

		// Set properties of the tree view widget
		set_grid_lines (TREE_VIEW_GRID_LINES_BOTH);
		set_rubber_banding (true);

		// Create "Property" column
		append_column ("Property", storage -> columns.property);
		TreeView::Column *prop_column = get_column (0);
		prop_column -> set_min_width (220);
		prop_column -> set_expand (false);

		// Create "Value" column
		append_column ("Value", storage -> columns.value);
		TreeView::Column *value_column = get_column (1);
		value_column -> set_min_width (180);
		value_column -> set_expand (true);

		// Make property names bold
		CellRendererText *prop_renderer = reinterpret_cast <CellRendererText*> (prop_column -> get_first_cell());
		prop_renderer -> property_weight() = 700;
		prop_renderer -> set_padding (20, 0);

		// Make values formatted well
		CellRendererText *value_renderer = reinterpret_cast <CellRendererText*> (value_column -> get_first_cell());
		value_renderer -> set_padding (10, 0);
	}
};

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
				auto table = make_managed <PropsTable> (groups[i].Properties());

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
