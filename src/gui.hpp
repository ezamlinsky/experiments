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
# include	"models/gui.hpp"

//****************************************************************************//
//      Class "GUI"                                                           //
//****************************************************************************//
class GUI
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	RefPtr <Application> app;		// GTK application
	thread *th;						// A thread for the GTK event loop

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
		Window *window				// The removed window
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
		Window &window				// The GUI window to display
	){
		// Add a new window to the Gtk event loop
		app -> add_window (window);
	}
} gui;

//****************************************************************************//
//      Show summary GUI windows for different objects                        //
//****************************************************************************//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Range                                                                 //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void Show (
	const ::Range &range			// The object to display
){
	gui.add (*new Model::SimpleSummary (range.Summary()));
}
void Show (
	const string &name,				// Window name
	const ::Range &range			// The object to display
){
	gui.add (*new Model::SimpleSummary (name, range.Summary()));
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Confidence Interval                                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void Show (
	const ConfidenceInterval &ci	// The object to display
){
	gui.add (*new Model::SimpleSummary (ci.Summary()));
}
void Show (
	const string &name,				// Window name
	const ConfidenceInterval &ci	// The object to display
){
	gui.add (*new Model::SimpleSummary (name, ci.Summary()));
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Statistical population                                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void Show (
	const Population &population	// The summary data to display
){
	gui.add (*new Observation::PopulationWindow (population));
}
void Show (
	const string &name,				// Window name
	const Population &population	// The summary data to display
){
	gui.add (*new Observation::PopulationWindow (name, population));
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Statistical sample                                                    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void Show (
	const Sample &sample			// The summary data to display
){
	gui.add (*new Observation::SampleWindow (sample));
}
void Show (
	const string &name,				// Window name
	const Sample &sample			// The summary data to display
){
	gui.add (*new Observation::SampleWindow (name, sample));
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
