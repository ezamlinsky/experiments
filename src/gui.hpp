/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                 USE THE GUI MODE TO DISPLAY OBJECT SUMMARIES                 #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# include	<thread>
# include	"observations/gui.hpp"

//****************************************************************************//
//      Class "GUI"                                                           //
//****************************************************************************//
class GUI
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	RefPtr <Application> app;	// GTK application
	thread *th;					// A thread for the GTK event loop

//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Runner of the GTK event loop                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void run (void) {
		app -> run ();
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      A signal handler for a window remove event                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void remove (
		Window *window			// The removed window
	){
		delete window;
	}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	GUI (void) : app (Application::create ("org.gtkmm.application"))
	{
		// Track all window-removed signals to release allocated memory
		app -> signal_window_removed().connect (sigc::mem_fun (*this, &GUI::remove));

		// By default, no windows are displayed, so keep the loop running
		app -> hold ();

		// Start the GTK event loop in a separate thread,
		// and do not block the Python execution cycle
		th = new thread (&GUI::run, this);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Destructor                                                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	~GUI (void) {

		// Unlock the GTK event loop from being ended
		app -> release ();

		// Wait for its end
		th -> join();

		// Deactivate the thread
		delete th;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Add a GUI window to the GTK event loop                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void add (
		Window &window			// The GUI window to display
	){
		// Add a new window to the Gtk event loop
		app -> add_window (window);
	}
} gui;

//****************************************************************************//
//      Show summary GUI windows for different objects                        //
//****************************************************************************//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Statistical population                                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void Show (const Population &population) {
	gui.add (*new SummaryWindow ("POPULATION", population));
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Statistical sample                                                    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void Show (const Sample &sample) {
	gui.add (*new SummaryWindow ("SAMPLE", sample));
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
