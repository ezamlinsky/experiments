/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                   CREATE A GUI TABLE OF OBJECT PROPERTIES                    #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	<format>
# include	<gtkmm.h>
# include	"prop_group.hpp"

// Use shortenings
using namespace Gtk;
using Glib::ustring;
using Glib::RefPtr;

//****************************************************************************//
//      Popup menu                                                            //
//****************************************************************************//
class TableMenu : public Menu
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	TreeView &table;				// Tree view is connected with this popup menu
	TreeModelColumn <ustring> col;	// Column to copy a value from
	MenuItem copy_item;				// "Copy" popup menu item

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	TableMenu (
		TreeView &viewer,				// Tree view is connected with this popup menu
		TreeModelColumn <ustring> col	// Column to copy a value from
	) :	table (viewer),
		col (col),
		copy_item ("_Copy", true)
	{
		// Set properties of "Copy" popup menu item
		copy_item.signal_activate().connect (sigc::mem_fun (*this, &TableMenu::copy_value));
		copy_item.set_tooltip_text ("Copy selected value to the clipboard");
		append (copy_item);

		// Set properties of popup menu
		accelerate (viewer);

		// Show control elements
		show_all();
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Copy a selected row to the clipboard                                  //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void copy_value (void) const {

		// Get the selected row
		auto iter = table.get_selection() -> get_selected();
		if (iter)
		{
			// Copy the value field to the clipboard
			Clipboard::get() -> set_text (iter -> get_value (col));
		}
	}
};

//****************************************************************************//
//      Class "PropTable"                                                     //
//****************************************************************************//
class PropTable : public TreeView
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
			TreeModelColumn <ustring> property;		// Property name
			TreeModelColumn <ustring> value;		// Property value (numeric)

			// Constructor
			Columns (void) {
				add (property);
				add (value);
			}
		};
		const Columns columns;			// Table columns

		// Constructor
		Storage (
			const properties &props,	// List of properties to display
			size_t digits				// Number of decimal digits to display for numbers
		){
			// Set types of columns
			set_column_types (columns);

			// Fill the storage with data
			for (const auto &opt : props) {
				auto row = *append();
				row.set_value (columns.property, ustring (opt.first + ":"));
				row.set_value (columns.value, ustring (format ("{:.{}g}", opt.second, digits)));
			}
		}
	};
	RefPtr <Storage> storage;			// Data storage for table data
	TableMenu menu;						// Popup menu for the table

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	PropTable (
		const header &columns,			// Column names to display
		const properties &props,		// List of properties to display
		size_t digits					// Number of decimal digits to display for numbers
	) : storage (new Storage (props, digits)),
		menu (*this, storage -> columns.value)
	{
		// Set table data
		set_model (storage);

		// Set properties of the tree view widget
		set_grid_lines (TREE_VIEW_GRID_LINES_BOTH);
		set_rubber_banding (true);

		// Create "Property" column
		append_column (columns.first, storage -> columns.property);
		TreeView::Column *prop_column = get_column (0);
		prop_column -> set_min_width (220);
		prop_column -> set_expand (false);

		// Create "Value" column
		append_column (columns.second, storage -> columns.value);
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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Copy a selected row to the clipboard                                  //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	bool on_key_press_event (
		GdkEventKey* event				// Describes a key press
	){
		// Check for CTRL+C
		if (event -> keyval == GDK_KEY_c && (event -> state & GDK_CONTROL_MASK))
		{
			// Get the selected row
			auto iter = get_selection() -> get_selected();
			if (iter)
			{
				// Copy the value field to the clipboard
				Clipboard::get() -> set_text (iter -> get_value (storage -> columns.value));
			}
			return true;
		}

		// Allow other handlers to handle this signal
		return false;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Show a pop-up menu when the user releases the right mouse button      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	bool on_button_release_event (
		GdkEventButton* event			// Describes a button press
	){
		// Show the popup menu
		if (event -> button == 3) {
			menu.popup_at_pointer (reinterpret_cast <GdkEvent*> (event));
			return true;
		}

		// Allow other handlers to handle this signal
		return false;
	}
};
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
