/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#               SHOW THE GUI WINDOW WITH THE OBJECT INFORMATION                #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"discrete/uniform.hpp"
# include	"discrete/bernoulli.hpp"
# include	"discrete/geometric.hpp"
# include	"discrete/binomial.hpp"
# include	"discrete/negative_binomial.hpp"
# include	"discrete/poisson.hpp"
# include	"continuous/kolmogorov.hpp"
# include	"continuous/uniform.hpp"
# include	"continuous/standard_t.hpp"
# include	"continuous/fisher_snedecor.hpp"
# include	"continuous/beta.hpp"
# include	"continuous/erlang.hpp"
# include	"continuous/chi_squared.hpp"
# include	"continuous/gamma.hpp"
# include	"continuous/pareto.hpp"
# include	"continuous/exponential.hpp"
# include	"continuous/rayleigh.hpp"
# include	"continuous/logistic.hpp"
# include	"continuous/normal.hpp"
# include	"continuous/laplace.hpp"
# include	"continuous/asymmetric_laplace.hpp"
# include	"../prop_notebook.hpp"
# include	"../summary_window.hpp"

//****************************************************************************//
//      Name space "Model"                                                    //
//****************************************************************************//
namespace Model
{
//****************************************************************************//
//      Class "DistributionWindow"                                            //
//****************************************************************************//
class DistributionWindow : public SummaryWindow
{
//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors                                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	DistributionWindow (
		const ObjectSummary &summary	// Summary info to display
	) :	SummaryWindow (
		summary.Name(),
		make_managed <PropNotebook> (
			summary.Header(),
			summary.Groups(),
			summary.Precision()
		)
	)
	{
		// Show the window
		show_all();
	}

	DistributionWindow (
		const ustring &name,			// Window name
		const ObjectSummary &summary	// Summary info to display
	) :	SummaryWindow (
		name,
		summary.Name(),
		make_managed <PropNotebook> (
			summary.Header(),
			summary.Groups(),
			summary.Precision()
		)
	)
	{
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
