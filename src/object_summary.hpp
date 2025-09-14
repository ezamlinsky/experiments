/*
################################################################################
# Encoding: UTF-8                                                  Tab size: 4 #
#                                                                              #
#                CONTAIN A FULL SUMMARY FOR AN ABSTRACT OBJECT                 #
#                                                                              #
# Ordnung muss sein!                             Copyleft (Ɔ) Eugene Zamlinsky #
################################################################################
*/
# pragma	once
# include	<cctype>
# include	<algorithm>
# include	"prop_group.hpp"

// Use shortenings
using groups = deque <PropGroup>;

//****************************************************************************//
//      Class "ObjectSummary"                                                 //
//****************************************************************************//
class ObjectSummary
{
//============================================================================//
//      Members                                                               //
//============================================================================//
private:
	string object_name;			// Object name
	properties props;			// List of object-specific properties
	groups grps;				// Groups of object properties

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructor                                                           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	ObjectSummary (
		const string &name		// Object name
	) :	object_name (name)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Append a property to the list of object-specific properties           //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void Append (
		const string &name,		// Object property name
		double value			// Value of the object property
	){
		// Append the property to the list
		props.push_back (property (name, value));
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Append a group to the list of groups                                  //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void Append (
		const PropGroup &group	// The properties group to append
	){
		// Append the property to the list
		grps.push_back (group);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Set name of the properties group                                      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void Name (
		const string &name		// New name of the object
	){
		object_name = name;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Get name of the object                                                //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const string& Name (void) const {
		return object_name;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Get count of object property groups                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	size_t Size (void) const {
		return grps.size();
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      List of object-specific properties                                    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const properties& Properties (void) const {
		return props;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      List of object property groups                                        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const groups& Groups (void) const {
		return grps;
	}
	groups& Groups (void) {
		return grps;
	}
};

//****************************************************************************//
//      Translate the object to a string                                      //
//****************************************************************************//
ostream& operator << (ostream &stream, const ObjectSummary &object)
{
	// Remember the precision to restore it later
	auto restore = stream.precision();
	stream.precision (16);

	// Print the underlined object name
	string name = object.Name();
	transform (name.begin(), name.end(), name.begin(), [](unsigned char c) { return toupper(c); });
	stream << endl << string (80, '=') << endl;
	stream << "    " << name << ":" << endl;
	stream << string (80, '=') << endl;

	// Print all the object-specific properties
	stream << object.Properties();

	// Do not print the group name if the object contains exactly one group inside
	if (object.Size() > 1)

		// Print all the groups
		for (const auto &group : object.Groups())
			stream << group;
	else

		// Print the only one group
		stream << object.Groups()[0].Properties();

	stream << string (80, '~') << endl;

	// Restore the precision
	stream.precision (restore);
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
