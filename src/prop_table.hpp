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
	PropTable (
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
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
