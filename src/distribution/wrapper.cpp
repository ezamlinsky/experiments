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
# include	<boost/python/suite/indexing/vector_indexing_suite.hpp>
# include	"bins.hpp"
# include	"distribution.hpp"
# include	"cdf.hpp"
# include	"comparator.hpp"

//****************************************************************************//
//      Python module initialization functions                                //
//****************************************************************************//
BOOST_PYTHON_MODULE (distribution) {

	// Use shortenings
	using namespace boost::python;

//============================================================================//
//      Expose "KolmogorovScore" to Python                                    //
//============================================================================//
	class_ <KolmogorovScore> ("KolmogorovScore")
		.def_readonly ("name", &KolmogorovScore::name)
		.def_readonly ("score", &KolmogorovScore::score);

//============================================================================//
//      Expose "PearsonScore" to Python                                       //
//============================================================================//
	class_ <PearsonScore> ("PearsonScore")
		.def_readonly ("name", &PearsonScore::name)
		.def_readonly ("score", &PearsonScore::score);

//============================================================================//
//      Expose vector <KolmogorovScoreVector> to Python                       //
//============================================================================//
	class_ <vector <KolmogorovScore>> ("KolmogorovScoreVector")
		.def (vector_indexing_suite <vector <KolmogorovScore> > ())
		.def ("__str__", kolmogorov_score_to_string);

//============================================================================//
//      Expose vector <PearsonScoreVector> to Python                          //
//============================================================================//
	class_ <vector <PearsonScore>> ("PearsonScoreVector")
		.def (vector_indexing_suite <vector <PearsonScore> > ())
		.def ("__str__", pearson_score_to_string);

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
			"Terrell-Scott rule")
		.def (self_ns::str (self_ns::self));

//============================================================================//
//      Expose "Distribution" class to Python                                 //
//============================================================================//
	class_ <Distribution> ("Distribution",
		"Calculate pdf and cdf for a discrete distribution",
		init <> ())

		// Constructor from a theoretical model
		.def (init <const BaseModel&, const vector <double>&>
			(args ("model", "values"),
			"Calculate a discrete distribution for a theoretical model"))
		.def (init <const BaseModel&, const Range&, size_t>
			(args ("model", "range", "bins"),
			"Calculate a continuous distribution for a theoretical model"))

		// Constructors from empirical data
		.def (init <const list&> (args ("data"),
			"Calculate a discrete distribution from empirical data"))
		.def (init <const vector <double>&> (args ("data"),
			"Calculate a discrete distribution from empirical data"))
		.def (init <const Observations&> (args ("data"),
			"Calculate a discrete distribution from empirical data"))
		.def (init <const list&, size_t> (args ("data", "bins"),
			"Calculate a continuous distribution from empirical data"))
		.def (init <const vector <double>&, size_t> (args ("data", "bins"),
			"Calculate a continuous distribution from empirical data"))
		.def (init <const Observations&, size_t> (args ("data", "bins"),
			"Calculate a continuous distribution from empirical data"))

		// Methods
		.def ("Domain",			&Distribution::Domain,	return_internal_reference <> (),
			"Domain of the distribution")
		.def ("Values",			&Distribution::Values,	return_internal_reference <> (),
			"Unique values in the dataset")
		.def ("CDF",			&Distribution::CDF,		return_internal_reference <> (),
			"Values of the CDF function for the dataset")
		.def ("PDF",			&Distribution::PDF,		return_internal_reference <> (),
			"Values of the PDF function for the dataset")
		.def ("SmoothedPDF",	&Distribution::SmoothedPDF,	args ("points"),
			"Smoothed values of the CDF function for the dataset")
		.def ("GetCDF",			&Distribution::GetCDF, args ("x"),
			"Find a value of the CDF function for an arbitrary Х")
		.def ("GetPDF",			&Distribution::GetPDF, args ("x"),
			"Find a value of the PDF function for an arbitrary Х")
		.def (self_ns::str (self_ns::self))

		// Properties
		.add_property ("Bins",	&Distribution::Bins,
			"Bins count the distribution function is split into");

//============================================================================//
//      Expose "CDF" class to Python                                          //
//============================================================================//
void (CDF::*ReferenceSample1)(const list &py_list)			= &CDF::ReferenceSample;
void (CDF::*ReferenceSample2)(const vector <double> &data)	= &CDF::ReferenceSample;
void (CDF::*ReferenceSample3)(const Observations &data)		= &CDF::ReferenceSample;
	class_ <CDF> ("CDF",
		"Compare two cdf functions with one another",
		init <const list&> (args ("data"),
			"Init the sample from empirical data"))
		.def (init <const vector <double>&> (args ("data"),
			"Init the sample from empirical data"))
		.def (init <const Observations&> (args ("data"),
			"Init the sample from empirical data"))

		// Constructors from empirical data and a theoretical model
		.def (init <const list&, const BaseModel&>
			(args ("data", "model"),
			"Init distributions from empirical data and a theoretical model"))
		.def (init <const vector <double>&, const BaseModel&>
			(args ("data", "model"),
			"Init distributions from empirical data and a theoretical model"))
		.def (init <const Observations&, const BaseModel&>
			(args ("data", "model"),
			"Init distributions from empirical data and a theoretical model"))

		// Constructors from empirical data only (a sample and a reference)
		.def (init <const list&, const list&>
			(args ("sample", "reference"),
			"Init distributions from empirical data only (a sample and a reference)"))
		.def (init <const vector <double>&, const vector <double>&>
			(args ("sample", "reference"),
			"Init distributions from empirical data only (a sample and a reference)"))
		.def (init <const Observations&, const Observations&>
			(args ("sample", "reference"),
			"Init distributions from empirical data only (a sample and a reference)"))

		// Methods
		.def ("ReferenceSample",			ReferenceSample1,		args ("data"),
			"Load another sample as a reference for the distribution test")
		.def ("ReferenceSample",			ReferenceSample2,		args ("data"),
			"Load another sample as a reference for the distribution test")
		.def ("ReferenceSample",			ReferenceSample3,		args ("data"),
			"Load another sample as a reference for the distribution test")
		.def ("ReferenceModel",				&CDF::ReferenceModel,	args ("model"),
			"Load a distribution model as a reference for the distribution test")
		.def ("Sample",						&CDF::Sample,
			return_value_policy <copy_const_reference> (),
			"Return sample distribution")
		.def ("Reference",					&CDF::Reference,
			return_value_policy <copy_const_reference> (),
			"Return reference distribution")
		.def ("KolmogorovConfidenceLevel",	&CDF::KolmogorovConfidenceLevel,
			"Confidence level of the one-sample Kolmogorov-Smirnov test")
		.def ("KolmogorovSmirnovTest",		&CDF::KolmogorovSmirnovTest,
			"Perform the one-sample or two-sample Kolmogorov-Smirnov test")
		.def ("ScoreTable",					&CDF::ScoreTable,
			"Score table (confidence level) for different distribution models")
		.def (self_ns::str (self_ns::self))

		// Static methods
        .staticmethod ("ScoreTable");

//============================================================================//
//      Expose "DistComparator" class to Python                               //
//============================================================================//
	class_ <DistComparator> ("DistComparator",
		"Compare two distributions functions with one another",
		init <const list&> (args ("data"),
			"Init the sample as a discrete distribution from empirical data"))
		.def (init <const vector <double>&> (args ("data"),
			"Init the sample as a discrete distribution from empirical data"))
		.def (init <const Observations&> (args ("data"),
			"Init the sample as a discrete distribution from empirical data"))
		.def (init <const list&, size_t> (args ("data", "bins"),
			"Init the sample as a continuous distribution from empirical data"))
		.def (init <const vector <double>&, size_t> (args ("data", "bins"),
			"Init the sample as a continuous distribution from empirical data"))
		.def (init <const Observations&, size_t> (args ("data", "bins"),
			"Init the sample as a continuous distribution from empirical data"))

		// Constructors from empirical data and a theoretical model
		.def (init <const list&, const BaseModel&>
			(args ("data", "model"),
			"Init discrete distributions from empirical data and a theoretical model"))
		.def (init <const vector <double>&, const BaseModel&>
			(args ("data", "model"),
			"Init discrete distributions from empirical data and a theoretical model"))
		.def (init <const Observations&, const BaseModel&>
			(args ("data", "model"),
			"Init discrete distributions from empirical data and a theoretical model"))
		.def (init <const list&, const BaseModel&, size_t>
			(args ("data", "model", "bins"),
			"Init continuous distributions from empirical data and a theoretical model"))
		.def (init <const vector <double>&, const BaseModel&, size_t>
			(args ("data", "model", "bins"),
			"Init continuous distributions from empirical data and a theoretical model"))
		.def (init <const Observations&, const BaseModel&, size_t>
			(args ("data", "model", "bins"),
			"Init continuous distributions from empirical data and a theoretical model"))

		// Methods
		.def ("ReferenceModel",				&DistComparator::ReferenceModel,	args ("model"),
			"Load a distribution model as a reference for the distribution test")
		.def ("Sample",						&DistComparator::Sample,
			return_value_policy <copy_const_reference> (),
			"Return sample distribution")
		.def ("Reference",					&DistComparator::Reference,
			return_value_policy <copy_const_reference> (),
			"Return reference distribution")
		.def ("PearsonConfidenceLevel",		&DistComparator::PearsonConfidenceLevel,
			"Confidence level of Pearson's chi-squared test")
		.def ("PearsonChiSquaredTest",		&DistComparator::PearsonChiSquaredTest,
			"Perform the Pearson's chi-squared test")
		.def ("ScoreTable",					&DistComparator::ScoreTable,
			"Score table (confidence level) for different distribution models")
		.def (self_ns::str (self_ns::self))

		// Static methods
        .staticmethod ("ScoreTable");
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
