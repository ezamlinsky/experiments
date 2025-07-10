/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#               THE WRAPPER CODE TO EXPOSE C++ CLASSES TO PYTHON               #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# include	<boost/python.hpp>
# include	"bins.hpp"
# include	"cumulative_function.hpp"
# include	"cdf.hpp"
# include	"continuous_distribution.hpp"
# include	"distributions.hpp"

//****************************************************************************//
//      Python module initialization functions                                //
//****************************************************************************//
BOOST_PYTHON_MODULE (distribution) {

	// Use shortenings
	using namespace boost::python;

//============================================================================//
//      Expose Bins class to Python                                           //
//============================================================================//
	class_ <Bins> ("Bins",
		"Calculate the optimal number of bins for the observation",
		init <size_t> (args ("observations"),
			"The number of empirical observations"))
		.add_property ("Raw",			&Bins::Raw,
			"Raw number of observations")
		.add_property ("Default",		&Bins::Default,
			"Default (square root) choice")
		.add_property ("Sturges",		&Bins::Sturges,
			"Sturges's rule")
		.add_property ("Rice",			&Bins::Rice,
			"Rice rule")
		.add_property ("TerrellScott",	&Bins::TerrellScott,
			"Terrell-Scott rule");

//============================================================================//
//      Expose "CumulativeFunction" class to Python                           //
//============================================================================//
	class_ <CumulativeFunction> ("CumulativeFunction",
		"Calculate a CDF using empirical data or a theoretical model",
		init <> ())
		.add_property ("Size",	&CumulativeFunction::Size,
			"Count of CDF data points for which we have values")
		.def ("Domain",	&CumulativeFunction::Domain,
			return_internal_reference <> (),
			"Domain of the CDF")
		.def ("Values",	&CumulativeFunction::Values,
			return_internal_reference <> (),
			"Unique values in the dataset")
		.def ("CDF",	&CumulativeFunction::CDF,
			return_internal_reference <> (),
			"Values of the CDF function for the dataset")
		.def (self_ns::str (self_ns::self));

//============================================================================//
//      Expose "CDF" class to Python                                          //
//============================================================================//
void (CDF::*ReferenceSample1)(const list &py_list)			= &CDF::ReferenceSample;
void (CDF::*ReferenceSample2)(const vector <double> &data)	= &CDF::ReferenceSample;
	class_ <CDF> ("CDF",
		"Compare two CDF functions with one another",
		init <const list&> (args ("data"),
			"Calculate CDF for empirical data"))
		.def (init <const vector <double>&>
			(args ("data"),
			"Calculate CDF for empirical data"))
		.def (init <const list&, const BaseModel&>
			(args ("data", "model"),
			"Calculate CDFs for empirical data and a theoretical model"))
		.def (init <const vector <double>&, const BaseModel&>
			(args ("data", "model"),
			"Calculate CDFs for empirical data and a theoretical model"))
		.def (init <const list&, const list&>
			(args ("sample", "reference"),
			"Calculate CDFs for empirical data only (a sample and a reference)"))
		.def (init <const vector <double>&, const vector <double>&>
			(args ("sample", "reference"),
			"Calculate CDFs for empirical data only (a sample and a reference)"))
		.def ("ReferenceSample",		ReferenceSample1,		args ("data"),
			"Load another sample as a reference for the distribution test")
		.def ("ReferenceSample",		ReferenceSample2,		args ("data"),
			"Load another sample as a reference for the distribution test")
		.def ("ReferenceModel",			&CDF::ReferenceModel,	args ("model"),
			"Load a distribution model as a reference for the distribution test")
		.def ("Sample",					&CDF::Sample,
			return_value_policy <copy_const_reference> (),
			"Return sample CDF function")
		.def ("Reference",				&CDF::Reference,
			return_value_policy <copy_const_reference> (),
			"Return reference CDF function")
		.def ("KolmogorovSmirnovTest",	&CDF::KolmogorovSmirnovTest,
			"Do one-sample and two-sample Kolmogorov-Smirnov tests for two CDFs")
		.def (self_ns::str (self_ns::self));

//============================================================================//
//      Expose "ContinuousDistribution" class to Python                       //
//============================================================================//
	class_ <ContinuousDistribution> ("ContinuousDistribution",
		"Calculate pdf and cdf for a continuous distribution",
		init <> ())
		.def (init <const list&, size_t>
			(args ("data", "bins"),
			"Calculate a distribution for empirical data"))
		.def (init <const vector <double>&, size_t>
			(args ("data", "bins"),
			"Calculate a distribution for empirical data"))
		.def (init <const BaseModel&, const Range&, size_t>
			(args ("model", "range", "bins"),
			"Calculate a distribution for a theoretical model"))
		.add_property ("Size",	&ContinuousDistribution::Size,
			"Count of stored CDF data points")
		.add_property ("Bins",	&ContinuousDistribution::Bins,
			"Bins count the CDF function has been split for a histogram")
		.def ("Domain",	&ContinuousDistribution::Domain,
			return_internal_reference <> (),
			"Domain of the CDF")
		.def ("Values",	&ContinuousDistribution::Values,
			return_internal_reference <> (),
			"Unique values in the dataset")
		.def ("PDF",	&ContinuousDistribution::PDF,
			return_internal_reference <> (),
			"Values of the PDF function for the dataset")
		.def ("SmoothedPDF",	&ContinuousDistribution::SmoothedPDF,
			args ("points"),
			"Smoothed values of the CDF function for the dataset")
		.def ("CDF",	&ContinuousDistribution::CDF,
			return_internal_reference <> (),
			"Values of the CDF function for the dataset")
		.def (self_ns::str (self_ns::self));

//============================================================================//
//      Expose "Distributions" class to Python                                //
//============================================================================//
	class_ <Distributions> ("Distributions",
		"Aggregate multiple statistical distributions in one set",
		init <> ())
		.def (init <const ContinuousDistribution&>	(args ("dist"),
			"Appends a continuous distribution to the set"))
		.def (init <const list&, size_t>
			(args ("data", "bins"),
			"Calculate a continuous distribution for empirical data"))
		.def (init <const vector <double>&, size_t>
			(args ("data", "bins"),
			"Calculate a continuous distribution for empirical data"))
		.def (init <const BaseModel&, const Range&, size_t>
			(args ("model", "range", "bins"),
			"Calculate a continuous distribution for a theoretical model"))
		.def (init <const vector <double>&, const BaseModel&, size_t>
			(args ("data", "model", "bins"),
			"Calculate continuous distributions for empirical data and a model"))
		.def (init <const list&, const BaseModel&, size_t>
			(args ("data", "model", "bins"),
			"Calculate continuous distribution for empirical data and a model"))
		.def (init <const list&, const list&, size_t>
			(args ("sample", "reference", "bins"),
			"Calculate continuous distributions for empirical data only (a sample and a reference)"))
		.def (init <const vector <double>&, const vector <double>&, size_t>
			(args ("sample", "reference", "bins"),
			"Calculate continuous distributions for empirical data only (a sample and a reference)"))
		.add_property ("Size",	&Distributions::Size,
			"Count of stored distribution in the set")
		.def ("Add",			&Distributions::Add,
			"Add a continuous distribution to the set")
		.def ("Get",			&Distributions::Get,	args("index"),
			return_value_policy <copy_const_reference> (),
			"Get a continuous distribution by its index in the set")
		.def (self_ns::str (self_ns::self));
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
