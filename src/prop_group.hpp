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
using properties = vector <pair <string, double>>;

//****************************************************************************//
//      Class "PropGroup"                                                     //
//****************************************************************************//
class PropGroup
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	const string name;				// Name of the properties group
	properties props;				// List of the properties

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	PropGroup (
		const string &name			// Name of the properties group
	) :	name (name)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Append a property to the list                                         //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void Append (
		const string &property,		// Object property name
		double value				// Value of the object property
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
	const properties& Properties (void) const {
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
	const auto &props = object.Properties();
	for (const auto &opt : props) {
		stream << "    ";
		stream.width (36);
		stream << left << opt.first << "= "<< opt.second << endl;
	}
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
