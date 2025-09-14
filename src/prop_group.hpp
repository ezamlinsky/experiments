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
using property = pair <string, double>;
using properties = deque <property>;

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
//      Default constructor                                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	PropGroup (void) = default;

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
		const string &name,		// Object property name
		double value			// Value of the object property
	){
		// Prepend the property to the list
		props.push_front (property (name, value));
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Append a property to the list                                         //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void Append (
		const string &name,		// Object property name
		double value			// Value of the object property
	){
		// Append the property to the list
		props.push_back (property (name, value));
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

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const property &prop)
{
	// Print the property
	stream << "    ";
	stream.width (36);
	stream << left << prop.first << "= "<< prop.second;
	return stream;
}
ostream& operator << (ostream &stream, const properties &props)
{
	// Print all the properties
	for (const auto &opt : props)
		stream << opt << endl;
	return stream;
}
ostream& operator << (ostream &stream, const PropGroup &object)
{
	// Print the underlined group name if set
	const string &name = object.Name();
	if (name.length()) {
		stream << endl << name << ":" << endl;
		stream << string (name.size() + 1, '~') << endl;
	}

	// Print all the properties in the group
	stream << object.Properties();
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
