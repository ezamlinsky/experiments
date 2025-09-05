/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                  SHOW THE SUMMARY GUI WINDOW WITH CONTROLS                   #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	<gtkmm.h>

// Use shortenings
using namespace Gtk;
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
	VBox box;						// Layout widget

protected:
	ActionBar bar;					// The action bar at the bottom of the window

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	SummaryWindow (
		const ustring &title,		// Window title
		Widget *content				// Summary content to display
	){
		// Set the window properties
		set_title (title);

		// Create the close button
		auto close_button = make_managed <Button> ("Close");
		close_button -> set_image_from_icon_name ("window-close");
		close_button -> set_tooltip_text ("Close the window");
		close_button -> signal_clicked().connect (sigc::mem_fun (*this, &SummaryWindow::close));

		// Add it to the action bar
		bar.pack_end (*close_button);

		// Create the layout
		box.pack_start (*content, true, true);
		box.pack_start (bar, false, false);

		// Add it to the window
		add (box);
	}
};
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
