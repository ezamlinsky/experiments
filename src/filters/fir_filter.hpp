/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                        FINITE IMPULSE RESPONSE FILTER                        #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	"data_cleaner.hpp"
# include	"filter.hpp"
# include	"../templates/array.hpp"
# include	"../object_summary.hpp"

//****************************************************************************//
//      Class "FIR_Filter"                                                    //
//****************************************************************************//
class FIR_Filter : public Filter
{

// Use shortenings
using filt_func = vector <double> (*) (size_t);
using win_func = void (*) (double[], size_t);
using norm_func = void (*) (double[], size_t, double);

//============================================================================//
//      Protected methods                                                     //
//============================================================================//
protected:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Blackman window                                                       //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	static void Blackman (
		double impulse[],			// Filter impulse response
		size_t size					// Size of impulse response
	){
		// Blackman window properties
		const double alpha = 0.16;
		const double a0 = 0.5 * (1.0 - alpha);
		const double a1 = 0.5;
		const double a2 = 0.5 * alpha;

		// Apply the window function to the impulse response
		for (size_t i = 1; i < size; i++) {
			const double x = M_PI * i / size;
			impulse[i] *= a0 + a1 * cos (x) + a2 * cos (2.0 * x);
		}
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors                                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	FIR_Filter (
		filt_func filter,			// Function to compute the filter impulse response
		win_func window,			// Function to compute the filter window function
		norm_func norm,				// Function to normalize the filter coefficients (calibration)
		double pos_sign,			// Sign for the coefficients of "positive" frequencies
		double neg_sign,			// Sign for the coefficients of "negative" frequencies
		double scale,				// Scale factor for the filter norm
		size_t points				// Count of neighbor points to smooth by
	) :	Filter (points)
	{
		// Calculate the half of the filter impulse response, because of symmetry
		vector <double> half_impulse = filter (points);

		// Apply the window function to the impulse response
		window (half_impulse.data(), half_impulse.size());

		// Normalize the filter impulse response
		// In other words, we calibrate the filter after applying the window function
		norm (half_impulse.data(), half_impulse.size(), scale);

		// Reserve space for the full impulse response function
		const size_t size = half_impulse.size();
		vector <double> buffer (2 * size - 1, 0.0);

		// Reflect the filter impulse response to the left and right
		const size_t center_pos = size - 1;
		for (size_t i = 0; i < size; i++) {
			buffer [center_pos + i] = pos_sign * half_impulse [i];
			buffer [center_pos - i] = neg_sign * half_impulse [i];
		}

		// Set the full impulse response function of the filter
		impulse = buffer;
	}

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Apply the filter to the target time series                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	virtual vector <double> Apply (
		const vector <double> &data	// The time series to filter
	) const override final {

		// Clear the dataset from NaNs
		DataCleaner cleaner = DataCleaner (data);

		// Expand the clean dataset to the left and right
		const size_t impulse_size = impulse.size();
		const vector <double> &clean = cleaner.ExpandedData (impulse_size / 2, impulse_size / 2);

		// Filter response
		vector <double> response;

		// Compute the filter response
		const size_t size = data.size();
		for (size_t i = 0; i < size; i++) {

			// Find the filter response for the moving window
			const double conv = Array::SumMul (clean.data() + i, impulse.data(), impulse_size);
			response.push_back (conv);
		}

		// Restore NaN values in the impulse response
		return cleaner.Restore_NaNs (response);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Summary of the object                                                 //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	ObjectSummary Summary (
		const string &name			// Object name
	) const {

		// Create the summary storage
		ObjectSummary summary (name);

		// General info
		PropGroup info;
		info.Append ("Smoothing points", Points());
		info.Append ("Impulse response length", Size());
		summary.Append (info);

		// Return the summary
		return summary;
	}
};
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
