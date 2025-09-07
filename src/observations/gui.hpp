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
# include	"../prop_notebook.hpp"
# include	"../summary_window.hpp"
# include	"../models/gui.hpp"

//****************************************************************************//
//      Name space "Observation"                                              //
//****************************************************************************//
namespace Observation
{
// Use shortenings
using CI_item = pair <int, ustring>;
using CI_items = vector <CI_item>;

//****************************************************************************//
//      Class "CI_button"                                                     //
//****************************************************************************//
class CI_button : public ComboBox
{
//============================================================================//
//      Estimator type                                                        //
//============================================================================//
public:
	enum Estimator {
		MEAN,						// Mean value
		MEDIAN,						// Median value
		LOWER_QUARTILE,				// Lower quartile value
		UPPER_QUARTILE				// Upper quartile value
	};

//============================================================================//
//      Members                                                               //
//============================================================================//
private:

	// Storage for the combo box items
	struct Storage : public ListStore
	{
		// Data model for combo box items
		struct Columns : public TreeModelColumnRecord
		{
			// Members
			TreeModelColumn <int> id;			// Item id
			TreeModelColumn <ustring> name;		// Human-readable item name

			// Constructor
			Columns (void) {
				add (id);
				add (name);
			}
		};
		const Columns columns;		// Combo box columns

		// Constructor
		Storage (
			const CI_items &items	// List of items to display
		){
			// Set types of columns
			set_column_types (columns);

			// Fill the storage with data
			for (const auto &opt : items) {
				auto row = *append();
				row.set_value (columns.id, opt.first);
				row.set_value (columns.name, opt.second);
			}
		}
	};
	RefPtr <Storage> storage;		// Data storage for combo box items

//============================================================================//
//      Private methods                                                       //
//============================================================================//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Create combo box items                                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	static CI_items CreateItems (void) {
		CI_items items;
		items.push_back (CI_item (MEAN, "Mean"));
		items.push_back (CI_item (MEDIAN, "Median"));
		items.push_back (CI_item (LOWER_QUARTILE, "Lower quartile"));
		items.push_back (CI_item (UPPER_QUARTILE, "Upper quartile"));
		return items;
	}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	CI_button (void) : storage (new Storage (CreateItems())) {

		// Set combo box items
		set_model (storage);

		// Set columns
		pack_start (storage -> columns.name);

		// Set the mean as the default item
		set_active (MEAN);
	}
};

//****************************************************************************//
//      Class "CI_Window"                                                     //
//****************************************************************************//
class CI_Window : public Dialog
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	CI_button ci;					// Confidence level estimator
	Label label;					// Label text for the confidence level
	SpinButton level;				// Confidence level
	HBox vbox;						// Layout widget

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	CI_Window (
		Window &parent				// Transient parent for the window
	) :	Dialog ("Confidence Interval for...", parent, true),
		label ("Level (%):"),
		level (Adjustment::create (95, 0.1, 99.9, 0.1, 1, 0.0), 0.0, 1)
	{
		// Make the dialog window a fixed size
		set_resizable (false);
		set_default_response (RESPONSE_OK);

		// Set properties of the box container
		vbox.set_border_width (10);
		vbox.set_spacing (10);

		// Pack all the controls in the box container
		vbox.pack_start (ci);
		vbox.pack_start (label);
		vbox.pack_start (level);

		// Show all widgets within the box container
		vbox.show_all();

		// Get the content area of the dialog and pack widgets
		Box *content = get_content_area();
		content -> pack_start (vbox);

		// Add "OK" action button
		Button *ok = add_button ("OK", RESPONSE_OK);
		ok -> set_image_from_icon_name ("dialog-ok-apply");

		// Add "Cancel" action button
		Button *cancel = add_button ("Cancel", RESPONSE_CANCEL);
		cancel -> set_image_from_icon_name ("dialog-cancel");
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Estimator type                                                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	CI_button::Estimator Type (void) const {
		return static_cast <CI_button::Estimator> (ci.get_active_row_number());
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Confidence level                                                      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double Level (void) const {
		return level.get_value () / 100.0;
	}
};

//****************************************************************************//
//      Class "ObservationsWindow"                                            //
//****************************************************************************//
class ObservationsWindow : public SummaryWindow
{
//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Universal constructors                                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	ObservationsWindow (
		const ustring &title,		// Window title
		const Observations &data	// The summary data to display
	) : SummaryWindow (title + " (" + to_string (data.Size()) + " data points)", make_managed <PropNotebook> (data.Summary()))
	{}

	ObservationsWindow (
		const ustring &name,		// Window name
		const ustring &title,		// Window title
		const Observations &data	// The summary data to display
	) : SummaryWindow (name, title + " (" + to_string (data.Size()) + " data points)", make_managed <PropNotebook> (data.Summary()))
	{}
};

//****************************************************************************//
//      Class "PopulationWindow"                                              //
//****************************************************************************//
class PopulationWindow : public ObservationsWindow
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	const Population population;	// Statistical population

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors                                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	PopulationWindow (
		const Population &data		// The summary data to display
	) :	ObservationsWindow ("Population", data),
		population (data)
	{
		// Show the window
		show_all();
	}

	PopulationWindow (
		const ustring &name,		// Window name
		const Population &data		// The summary data to display
	) :	ObservationsWindow (name, "Population", data),
		population (data)
	{
		// Show the window
		show_all();
	}
};

//****************************************************************************//
//      Class "SampleWindow"                                                  //
//****************************************************************************//
class SampleWindow : public ObservationsWindow
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	const Sample sample;			// Statistical sample

//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Show a dialog window to set the options of the confidence interval    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void CI_Dialog (void) {

		// Create a dialog with CI configuration controls
		CI_Window dialog (*this);

		// Run the dialog
		switch (dialog.run())
		{
			// Handle "OK" button click
			case RESPONSE_OK: {

				// Confidence level
				double level = dialog.Level();

				// Check the estimator to calculate
				switch (dialog.Type()) {

					// Mean value
					case CI_button::MEAN:
						get_application() -> add_window (*new Model::ConfidenceIntervalWindow ("Mean", sample.Mean_CI (level)));
						break;

					// Median value
					case CI_button::MEDIAN:
						get_application() -> add_window (*new Model::ConfidenceIntervalWindow ("Median", sample.Median_CI (level)));
						break;

					// Lower quartile value
					case CI_button::LOWER_QUARTILE:
						get_application() -> add_window (*new Model::ConfidenceIntervalWindow ("Lower quartile", sample.LowerQuartile_CI (level)));
						break;

					// Upper quartile value
					case CI_button::UPPER_QUARTILE:
						get_application() -> add_window (*new Model::ConfidenceIntervalWindow ("Upper quartile", sample.UpperQuartile_CI (level)));
						break;

					// Nothing selected
					default:
						break;
				}
				break;
			}

			// Default action
			default:
				break;
		}
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Add extra buttons to the summary window                               //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void ExtraButtons (void) {

		// Create the confidence interval button
		Button *button = new Button ("Confidence interval");
		button -> set_image_from_icon_name ("view-statistics");
		button -> set_tooltip_text ("Confidence interval for mean and median values");
		button -> signal_clicked().connect (sigc::mem_fun (*this, &SampleWindow::CI_Dialog));

		// Add it to the action bar
		bar.pack_start (*button);
	}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors                                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	SampleWindow (
		const Sample &data			// The summary data to display
	) :	ObservationsWindow ("Sample", data),
		sample (data)
	{
		// Add the confidence interval button
		ExtraButtons();

		// Show the window
		show_all();
	}

	SampleWindow (
		const ustring &name,		// Window name
		const Sample &data			// The summary data to display
	) :	ObservationsWindow (name, "Sample", data),
		sample (data)
	{
		// Add the confidence interval button
		ExtraButtons();

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
