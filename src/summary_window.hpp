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
protected:
	ActionBar bar;					// The action bar at the bottom of the window

private:
	VBox box;						// Layout widget

//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Join the window name and title strings                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	static ustring Join (
		const ustring &name,		// Window name
		const ustring &title		// Window title
	){
		if (name.empty())
			return title;
		else
			return name + " - " + title;
	}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors                                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	SummaryWindow (
		const ustring &title,		// Window title
		Widget *content				// Summary content to display
	){
		// Set the window properties
		set_title (title);

		// Create the close button
		auto button = make_managed <Button> ("Close");
		button -> set_image_from_icon_name ("window-close");
		button -> set_tooltip_text ("Close the window");
		button -> signal_clicked().connect (sigc::mem_fun (*this, &SummaryWindow::close));

		// Add it to the action bar
		bar.pack_end (*button);

		// Create the layout
		box.pack_start (*content, true, true);
		box.pack_start (bar, false, false);

		// Add it to the window
		add (box);
	}

	SummaryWindow (
		const ustring &name,		// Window name
		const ustring &title,		// Window title
		Widget *content				// Summary content to display
	) :	SummaryWindow (Join (name, title), content)
	{}
};
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
