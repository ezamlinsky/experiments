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
# include	<deque>

// Use shortenings
using namespace std;
using properties = deque <pair <string, double>>;

//****************************************************************************//
//      Class "PropGroup"                                                     //
//****************************************************************************//
class PropGroup
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	string group_name;			// Name of the properties group
	properties props;			// List of the properties

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	PropGroup (
		const string &name		// Name of the properties group
	) :	group_name (name)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Prepend a property to the list                                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void Prepend (
		const string &property,	// Object property name
		double value			// Value of the object property
	){
		// Append the property to the list
		props.push_front (pair <string, double> (property, value));
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Append a property to the list                                         //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void Append (
		const string &property,	// Object property name
		double value			// Value of the object property
	){
		// Append the property to the list
		props.push_back (pair <string, double> (property, value));
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Set name of the properties group                                      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void Name (
		const string &name		// New name of the properties group
	){
		group_name = name;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Get name of the properties group                                      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const string& Name (void) const {
		return group_name;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      List of the properties                                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const properties& Properties (void) const {
		return props;
	}
};
using groups = deque <PropGroup>;

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
