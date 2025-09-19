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
//      Show a simple GUI window with summary information                     //
//****************************************************************************//
# define	SHOW_SIMPLE_SUMMARY1(class)											\
void Show (																		\
	const Model::class &object													\
){																				\
	gui.add (*new Model::SimpleSummary (object.Summary()));						\
}

//****************************************************************************//
//      Show a simple GUI window with summary information and a custom title  //
//****************************************************************************//
# define	SHOW_SIMPLE_SUMMARY2(class)											\
void Show (																		\
	const string &name,															\
	const Model::class &range													\
){																				\
	gui.add (*new Model::SimpleSummary (name, range.Summary()));				\
}

//****************************************************************************//
//      Show a GUI window with the distribution information                   //
//****************************************************************************//
# define	SHOW_DISTRIBUTION_SUMMARY1(class)									\
void Show (																		\
	const Model::class &object													\
){																				\
	gui.add (*new Model::DistributionWindow (object.Summary()));				\
}

//****************************************************************************//
//      Show a GUI window with the distribution information and a custom title//
//****************************************************************************//
# define	SHOW_DISTRIBUTION_SUMMARY2(class)									\
void Show (																		\
	const string &name,															\
	const Model::class &object													\
){																				\
	gui.add (*new Model::DistributionWindow (name, object.Summary()));			\
}

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

//============================================================================//
//      Range                                                                 //
//============================================================================//
SHOW_SIMPLE_SUMMARY1 (Range)
SHOW_SIMPLE_SUMMARY2 (Range)

//============================================================================//
//      Confidence Interval                                                   //
//============================================================================//
SHOW_SIMPLE_SUMMARY1 (ConfidenceInterval)
SHOW_SIMPLE_SUMMARY2 (ConfidenceInterval)

//============================================================================//
//      Discrete distributions                                                //
//============================================================================//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Discrete uniform distribution                                         //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
SHOW_DISTRIBUTION_SUMMARY1 (DiscreteUniform)
SHOW_DISTRIBUTION_SUMMARY2 (DiscreteUniform)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Bernoulli distribution                                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
SHOW_DISTRIBUTION_SUMMARY1 (Bernoulli)
SHOW_DISTRIBUTION_SUMMARY2 (Bernoulli)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Geometric distribution                                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
SHOW_DISTRIBUTION_SUMMARY1 (Geometric)
SHOW_DISTRIBUTION_SUMMARY2 (Geometric)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Binomial distribution                                                 //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
SHOW_DISTRIBUTION_SUMMARY1 (Binomial)
SHOW_DISTRIBUTION_SUMMARY2 (Binomial)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Negative Binomial distribution                                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
SHOW_DISTRIBUTION_SUMMARY1 (NegativeBinomial)
SHOW_DISTRIBUTION_SUMMARY2 (NegativeBinomial)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Poisson distribution                                                  //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
SHOW_DISTRIBUTION_SUMMARY1 (Poisson)
SHOW_DISTRIBUTION_SUMMARY2 (Poisson)

//============================================================================//
//      Continuous distributions                                              //
//============================================================================//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Kolmogorov distribution                                               //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
SHOW_DISTRIBUTION_SUMMARY1 (Kolmogorov)
SHOW_DISTRIBUTION_SUMMARY2 (Kolmogorov)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Continuous uniform distribution                                       //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
SHOW_DISTRIBUTION_SUMMARY1 (ContinuousUniform)
SHOW_DISTRIBUTION_SUMMARY2 (ContinuousUniform)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Rayleigh distribution                                                 //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
SHOW_DISTRIBUTION_SUMMARY1 (Rayleigh)
SHOW_DISTRIBUTION_SUMMARY2 (Rayleigh)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Exponential distribution                                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
SHOW_DISTRIBUTION_SUMMARY1 (Exponential)
SHOW_DISTRIBUTION_SUMMARY2 (Exponential)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Erlang distribution                                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
SHOW_DISTRIBUTION_SUMMARY1 (Erlang)
SHOW_DISTRIBUTION_SUMMARY2 (Erlang)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Chi-Squared distribution                                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
SHOW_DISTRIBUTION_SUMMARY1 (ChiSquared)
SHOW_DISTRIBUTION_SUMMARY2 (ChiSquared)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Gamma distribution                                                    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
SHOW_DISTRIBUTION_SUMMARY1 (Gamma)
SHOW_DISTRIBUTION_SUMMARY2 (Gamma)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Pareto distribution                                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
SHOW_DISTRIBUTION_SUMMARY1 (Pareto)
SHOW_DISTRIBUTION_SUMMARY2 (Pareto)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Beta distribution                                                     //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
SHOW_DISTRIBUTION_SUMMARY1 (Beta)
SHOW_DISTRIBUTION_SUMMARY2 (Beta)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      F distribution                                                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
SHOW_DISTRIBUTION_SUMMARY1 (F)
SHOW_DISTRIBUTION_SUMMARY2 (F)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      StandardT distribution                                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
SHOW_DISTRIBUTION_SUMMARY1 (StandardT)
SHOW_DISTRIBUTION_SUMMARY2 (StandardT)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Logistic distribution                                                 //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
SHOW_DISTRIBUTION_SUMMARY1 (Logistic)
SHOW_DISTRIBUTION_SUMMARY2 (Logistic)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Normal distribution                                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
SHOW_DISTRIBUTION_SUMMARY1 (Normal)
SHOW_DISTRIBUTION_SUMMARY2 (Normal)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Laplace distribution                                                  //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
SHOW_DISTRIBUTION_SUMMARY1 (Laplace)
SHOW_DISTRIBUTION_SUMMARY2 (Laplace)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      AsymmetricLaplace distribution                                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
SHOW_DISTRIBUTION_SUMMARY1 (AsymmetricLaplace)
SHOW_DISTRIBUTION_SUMMARY2 (AsymmetricLaplace)

//============================================================================//
//      Statistical population                                                //
//============================================================================//
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

//============================================================================//
//      Statistical sample                                                    //
//============================================================================//
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
