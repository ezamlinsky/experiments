/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#             GROUP PROPERTIES OF AN ABSTRACT OBJECT FOR A SUMMARY             #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	<iostream>
# include	<vector>

// Use shortenings
using namespace std;

//****************************************************************************//
//      Class "PropGroup"                                                     //
//****************************************************************************//
class PropGroup
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	const string name;						// Name of the properties group
	vector <pair <string, double>> props;	// List of the properties

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	PropGroup (
		const string &name					// Name of the properties group
	) :	name (name)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Add a property to the list                                            //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void Add (
		const string &property,				// Object property name
		double value						// Value of the object property
	){
		// Append the property to the list
		props.push_back (pair <string, double> (property, value));
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Name of the properties group                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const string& Name (void) const {
		return name;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      List of the properties                                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const vector <pair <string, double>>& properties (void) const {
		return props;
	}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const PropGroup &object)
{
	// Print the underlined group name
	const string &name = object.Name();
	stream << endl << name << ":" << endl;
	stream << string (name.size() + 1, '~') << endl;

	// Print all the properties in the group
	const auto &props = object.properties();
	for (const auto &opt : props) {
		const string opt_name = string ("    ") + opt.first;
		stream.width (40);
		stream << left << opt_name << "= "<< opt.second << endl;
	}
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
