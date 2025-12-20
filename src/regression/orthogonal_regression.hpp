/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                POLYNOMIAL REGRESSION BY ORTHOGONAL FUNCTIONS                 #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"orthogonal_functions.hpp"
# include	"../object_summary.hpp"

//****************************************************************************//
//      Class "OrthogonalRegression"                                          //
//****************************************************************************//
class OrthogonalRegression
{
//============================================================================//
//      Members                                                               //
//============================================================================//
protected:
	const OrthogonalFunctions *funcs;	// Orthogonal functions for the expansion
	mvector values;						// Original response values
	mvector residuals;					// Regression residuals
	mvector approx;						// Regression approximation
	mvector coeffs;						// Regression coefficients
	mvector norms;						// Norms of orthogonal functions
	double total_ss;					// Total sum of squares (TSS)
	double residual_ss;					// Residual sum of squares (RSS)

//============================================================================//
//      Private methods                                                       //
//============================================================================//
private:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Calculate the deviation from the approximation line                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const mvector Deviation (
		double scale					// Scale coefficient for the regression deviation
	) const {
		return mvector (sqrt (MSE_RSS()) * scale, funcs -> Size());
	}

//============================================================================//
//      Protected methods                                                     //
//============================================================================//
protected:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Sort the data set (connected X and Y values) by X values              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	static double* Sort (
		double x[],						// Predictors (independent variables)
		double y[],						// Response (dependent variables)
		size_t size						// Size of the dataset
	){
		// Prepare the connected variables for sorting
		vector <pair <double, double> > pairs;
		for (size_t i = 0; i < size; i++)
			pairs.push_back (pair <double, double> (x[i], y[i]));

		// Sort them in ascending order
		std::sort (pairs.begin(), pairs.end());

		// Put them back in the original arrays, but sorted
		for (size_t i = 0; i < size; i++) {
			x[i] = pairs[i].first;
			y[i] = pairs[i].second;
		}

		// Return sorted predictors back
		return x;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	OrthogonalRegression (
		double data[],						// Response values
		const OrthogonalFunctions *funcs	// Orthogonal functions for the expansion
	) :	funcs (funcs),
		values (data, funcs -> Size()),
		residuals (data, funcs -> Size()),
		approx (0.0, funcs -> Size()),
		coeffs (0.0, funcs -> Count()),
		norms (0.0, funcs -> Count())
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Destructor                                                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual ~OrthogonalRegression (void) {
		delete funcs;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Approximate the dataset with weighted orthogonal functions            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void Approximate (void) {

		// Get all of the orthogonal functions in the set
		const vector <mvector> &functions = funcs -> Functions();

		// Find the first regression coefficient and its variance
		double norm2 = functions[0].NormSqr();
		double coeff = functions[0].DotProduct (residuals) / norm2;
		coeffs[0] = coeff;
		norms[0] = sqrt (norm2);

		// Find residuals and regression approximation
		const mvector &func0 = functions[0];
		residuals.Sub (func0, coeff);
		approx.Add (func0, coeff);

		// Calculate total sum of squares (TSS)
		total_ss = residuals.NormSqr();

		// Use all the available orthogonal functions for an orthogonal expansion
		const size_t count = funcs -> Count();
		for (size_t j = 1; j < count; j++) {

			// Find each regression coefficient and its variance
			norm2 = functions[j].NormSqr();
			coeff = functions[j].DotProduct (residuals) / norm2;
			coeffs[j] = coeff;
			norms[j] = sqrt (norm2);

			// Find residuals and regression approximation
			const mvector &func = functions[j];
			residuals.Sub (func, coeff);
			approx.Add (func, coeff);
		}

		// Calculate residual sum of squares (RSS)
		residual_ss = residuals.NormSqr();
	}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Range of predictors (independent variables)                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const Model::Range& Domain (void) const {
		return funcs -> Domain();
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Predictors (independent variables)                                    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	vector <double> X (void) const {
		return funcs -> Values();
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Response (dependent variables)                                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	vector <double> Y (void) const {
		return values;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Regression approximation                                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	vector <double> Approx (void) const {
		return approx;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Regression residuals                                                  //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	vector <double> Residuals (void) const {
		return residuals;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Deviation from the regression approximation line (min values)         //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	vector <double> ApproxMin (
		double stdev					// Scale coefficient for standard deviation
	) const {
		return approx - Deviation (stdev);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Deviation from the regression approximation line (max values)         //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	vector <double> ApproxMax (
		double stdev					// Scale coefficient for standard deviation
	) const {
		return approx + Deviation (stdev);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Regression coefficients                                               //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const vector <double> Coeffs (void) const {
		return coeffs;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Standard errors of the regression coefficients                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const vector <double> StdErrCoeffs (void) const {
		mvector errors (sqrt (MSE_RSS()), funcs -> Count());
		return errors / norms;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Regression value for the target argument                              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual double Regression (
		double x						// Value to calculate the regression for
	) const {
		return funcs -> FuncValues (x).DotProduct (coeffs);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Total sum of squares (TSS)                                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double TSS (void) const {
		return total_ss;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Residual sum of squares (RSS)                                         //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double RSS (void) const {
		return residual_ss;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Explained sum of squares (ESS)                                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double ESS (void) const {
		return total_ss - residual_ss;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Mean square error (MSE) of total sum of squares (TSS)                 //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double MSE_TSS (void) const {
		const size_t degrees_freedom = funcs -> Size() - 1;
		return TSS() / degrees_freedom;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Mean square error (MSE) of residual sum of squares (RSS)              //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double MSE_RSS (void) const {
		const size_t degrees_freedom = funcs -> Size() - funcs -> Count();
		return RSS() / degrees_freedom;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Mean square error (MSE) of explained sum of squares (ESS)             //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double MSE_ESS (void) const {
		const size_t degrees_freedom = funcs -> Count() - 1;
		return ESS() / degrees_freedom;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Fraction of variance explained                                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double ExplainedVarFrac (void) const {
		return ESS() / TSS();
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Fraction of variance unexplained                                      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double UnexplainedVarFrac (void) const {
		return RSS() / TSS();
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Coefficient of determination                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double Determination (void) const {
		return ExplainedVarFrac();
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Coefficient of correlation                                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double Correlation (void) const {
		return sqrt (ExplainedVarFrac());
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Standard error of the coefficient of correlation                      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	double StdErrCorrelation (void) const {
		return UnexplainedVarFrac() / sqrt (funcs -> Size() - funcs -> Count());
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Data size                                                             //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	size_t Size (void) const {
		return funcs -> Size();
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Regression degree                                                     //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	size_t Degree (void) const {
		return funcs -> Degree();
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Count of the orthogonal functions used                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	size_t Functions (void) const {
		return funcs -> Count();
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Summary of the object                                                 //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	ObjectSummary Summary (
		const string &name				// Object name
	) const {

		// Create the summary storage
		ObjectSummary summary (name);

		// Regression info
		PropGroup info ("Regression info");
		info.Append ("Data size", Size());
		info.Append ("Regression degree", Degree());
		info.Append ("Orthogonal functions", Functions());
		info.Append ("Coefficient of determination", Determination());
		info.Append ("Coefficient of correlation", Correlation());
		info.Append ("Standard error of the correlation", StdErrCorrelation());
		summary.Append (info);

		// Model quality
		PropGroup quality ("Model quality");
		quality.Append ("Fraction of variance explained", ExplainedVarFrac());
		quality.Append ("Fraction of variance unexplained", UnexplainedVarFrac());
		summary.Append (quality);

		// Sums of squares
		PropGroup sums ("Sums of squares");
		sums.Append ("Total sum of squares (TSS)", TSS());
		sums.Append ("Explained sum of squares (ESS)", ESS());
		sums.Append ("Residual sum of squares (RSS)", RSS());
		sums.Append ("Mean square error (MSE) of TSS", MSE_TSS());
		sums.Append ("Mean square error (MSE) of ESS", MSE_ESS());
		sums.Append ("Mean square error (MSE) of RSS", MSE_RSS());
		summary.Append (sums);

		// Regression coefficients
		PropGroup coefficients ("Regression coefficients");
		const auto &coeffs = Coeffs();
		for (size_t i = 0; i < coeffs.size(); i++) {
			const string opt_name = string ("A[" + to_string (i) + "]");
			coefficients.Append (opt_name, coeffs[i]);
		}
		summary.Append (coefficients);

		// Standard errors of the regression coefficients
		PropGroup coefficient_errors ("Standard errors of the coefficients");
		const auto &errors = StdErrCoeffs();
		for (size_t i = 0; i < errors.size(); i++) {
			const string opt_name = string ("SE[" + to_string (i) + "]");
			coefficient_errors.Append (opt_name, errors[i]);
		}
		summary.Append (coefficient_errors);

		// Return the summary
		return summary;
	}
};
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
