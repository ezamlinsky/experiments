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
# include	"raw.hpp"
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
//      Expose "KolmogorovScoreTable" class to Python                         //
//============================================================================//
	class_ <KolmogorovScoreTable> ("KolmogorovScoreTable",
		"Show the score of the one-sample Kolmogorov-Smirnov test for different distribution models",
		init <> ())
		.def (self_ns::str (self_ns::self));

//============================================================================//
//      Expose "PearsonScoreTable" class to Python                            //
//============================================================================//
	class_ <PearsonScoreTable> ("PearsonScoreTable",
		"Show the score of the Pearson's chi-squared test for different distribution models",
		init <> ())
		.def (self_ns::str (self_ns::self));

//============================================================================//
//      Expose "Bins" class to Python                                         //
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
//      Expose "RawCDF" class to Python                                       //
//============================================================================//
	class_ <RawCDF> ("RawCDF",
		"Calculate a raw cdf function",
		init <> ())

		// Constructor from a theoretical model
		.def (init <const Model::BaseDiscrete&> (args ("model"),
			"Calculate theoretical raw CDF values for a discrete model"))
		.def (init <const Model::BaseContinuous&> (args ("model"),
			"Calculate theoretical raw CDF values for a continuous model"))

		// Constructors from empirical data
		.def (init <const pylist&> (args ("data"),
			"Calculate raw CDF values from empirical data"))
		.def (init <const vector <double>&> (args ("data"),
			"Calculate raw CDF values from empirical data"))
		.def (init <const Observations&> (args ("data"),
			"Calculate raw CDF values from empirical data"))

		// Methods
		.def ("Domain",			&RawCDF::Domain,	return_internal_reference <> (),
			"Domain of the raw CDF values")
		.def ("Values",			&RawCDF::Values,	return_internal_reference <> (),
			"Unique values in the dataset")
		.def ("CDF",			&RawCDF::CDF,		return_internal_reference <> (),
			"Values of the CDF function for the dataset")
		.def ("GetCDF",			&RawCDF::GetCDF, args ("x"),
			"Find a value of the CDF function for an arbitrary Х")
		.def (self_ns::str (self_ns::self))

		// Properties
		.add_property ("Size",	&RawCDF::Size,
			"Count of collected CDF values");

//============================================================================//
//      Expose "Distribution" class to Python                                 //
//============================================================================//
	class_ <Distribution> ("Distribution",
		"Calculate pdf and cdf functions for a distribution",
		init <> ())

		// Constructor from a theoretical model
		.def (init <const Model::BaseDiscrete&> (args ("model"),
			"Calculate theoretical PDF and CDF values for a discrete model"))
		.def (init <const Model::BaseContinuous&> (args ("model"),
			"Calculate theoretical PDF and CDF values for a continuous model"))

		// Constructors from empirical data
		.def (init <const pylist&> (args ("data"),
			"Calculate a discrete distribution from empirical data"))
		.def (init <const vector <double>&> (args ("data"),
			"Calculate a discrete distribution from empirical data"))
		.def (init <const Observations&> (args ("data"),
			"Calculate a discrete distribution from empirical data"))
		.def (init <const pylist&, size_t> (args ("data", "bins"),
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
void (CDF::*ReferenceSample1)(const pylist &py_list)		= &CDF::ReferenceSample;
void (CDF::*ReferenceSample2)(const vector <double> &data)	= &CDF::ReferenceSample;
void (CDF::*ReferenceSample3)(const Observations &data)		= &CDF::ReferenceSample;
	class_ <CDF> ("CDF",
		"Compare two cdf functions with one another",
		init <const pylist&> (args ("data"),
			"Init the sample from empirical data"))
		.def (init <const vector <double>&> (args ("data"),
			"Init the sample from empirical data"))
		.def (init <const Observations&> (args ("data"),
			"Init the sample from empirical data"))

		// Constructors from empirical data and a theoretical model
		.def (init <const pylist&, const Model::BaseDiscrete&>
			(args ("data", "model"),
			"Init CDF functions from empirical data and a discrete theoretical model"))
		.def (init <const vector <double>&, const Model::BaseDiscrete&>
			(args ("data", "model"),
			"Init CDF functions from empirical data and a discrete theoretical model"))
		.def (init <const Observations&, const Model::BaseDiscrete&>
			(args ("data", "model"),
			"Init CDF functions from empirical data and a discrete theoretical model"))
		.def (init <const pylist&, const Model::BaseContinuous&>
			(args ("data", "model"),
			"Init CDF functions from empirical data and a continuous theoretical model"))
		.def (init <const vector <double>&, const Model::BaseContinuous&>
			(args ("data", "model"),
			"Init CDF functions from empirical data and a continuous theoretical model"))
		.def (init <const Observations&, const Model::BaseContinuous&>
			(args ("data", "model"),
			"Init CDF functions from empirical data and a continuous theoretical model"))

		// Constructors from empirical data only (a sample and a reference)
		.def (init <const pylist&, const pylist&>
			(args ("sample", "reference"),
			"Init CDF functions from empirical data only (a sample and a reference)"))
		.def (init <const vector <double>&, const vector <double>&>
			(args ("sample", "reference"),
			"Init CDF functions from empirical data only (a sample and a reference)"))
		.def (init <const Observations&, const Observations&>
			(args ("sample", "reference"),
			"Init CDF functions from empirical data only (a sample and a reference)"))

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
			"Return sample CDF function")
		.def ("Reference",					&CDF::Reference,
			return_value_policy <copy_const_reference> (),
			"Return reference CDF function")
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
void (DistComparator::*ReferenceModel1)(const Model::BaseDiscrete &model)	= &DistComparator::ReferenceModel;
void (DistComparator::*ReferenceModel2)(const Model::BaseContinuous &model)	= &DistComparator::ReferenceModel;
	class_ <DistComparator> ("DistComparator",
		"Compare two distributions functions with one another",
		init <const pylist&> (args ("data"),
			"Init the sample as a discrete distribution from empirical data"))
		.def (init <const vector <double>&> (args ("data"),
			"Init the sample as a discrete distribution from empirical data"))
		.def (init <const Observations&> (args ("data"),
			"Init the sample as a discrete distribution from empirical data"))
		.def (init <const pylist&, size_t> (args ("data", "bins"),
			"Init the sample as a continuous distribution from empirical data"))
		.def (init <const vector <double>&, size_t> (args ("data", "bins"),
			"Init the sample as a continuous distribution from empirical data"))
		.def (init <const Observations&, size_t> (args ("data", "bins"),
			"Init the sample as a continuous distribution from empirical data"))

		// Constructors from empirical data and a theoretical model
		.def (init <const pylist&, const Model::BaseDiscrete&>
			(args ("data", "model"),
			"Init discrete distributions from empirical data and a theoretical model"))
		.def (init <const vector <double>&, const Model::BaseDiscrete&>
			(args ("data", "model"),
			"Init discrete distributions from empirical data and a theoretical model"))
		.def (init <const Observations&, const Model::BaseDiscrete&>
			(args ("data", "model"),
			"Init discrete distributions from empirical data and a theoretical model"))
		.def (init <const pylist&, const Model::BaseContinuous&, size_t>
			(args ("data", "model", "bins"),
			"Init continuous distributions from empirical data and a theoretical model"))
		.def (init <const vector <double>&, const Model::BaseContinuous&, size_t>
			(args ("data", "model", "bins"),
			"Init continuous distributions from empirical data and a theoretical model"))
		.def (init <const Observations&, const Model::BaseContinuous&, size_t>
			(args ("data", "model", "bins"),
			"Init continuous distributions from empirical data and a theoretical model"))

		// Methods
		.def ("ReferenceModel",				ReferenceModel1,	args ("model"),
			"Load a discrete distribution model as a reference for the distribution test")
		.def ("ReferenceModel",				ReferenceModel2,	args ("model"),
			"Load a continuous distribution model as a reference for the distribution test")
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
