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

// Default number of decimal digits to display for floating-point numbers
# define	DIGITS		16

// Use shortenings
using header = pair <string, string>;
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
	header header_str;			// Header line to show before object properties
	groups grps;				// Groups of object properties
	size_t digits;				// Number of decimal digits to display for numbers

//============================================================================//
//      Public methods                                                        //
//============================================================================//
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Default constructor                                                   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	ObjectSummary (void) : digits (DIGITS)
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Constructors                                                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	ObjectSummary (
		const string &name,		// Object name
		const string &pname,	// Header for property name
		const string &pvalue	// Header for property value
	) :	object_name (name),
		header_str (pname, pvalue),
		digits (DIGITS)
	{}

	ObjectSummary (
		const string &name		// Object name
	) :	ObjectSummary (name, "Property", "Value")
	{}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Prepend a group to the list of groups                                 //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void Prepend (
		const PropGroup &group	// The properties group to prepend
	){
		// Prepend the properties group to the list
		grps.push_front (group);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Append a group to the list of groups                                  //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void Append (
		const PropGroup &group	// The properties group to append
	){
		// Append the properties group to the list
		grps.push_back (group);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Set precision for numeric values                                      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	void Precision (
		size_t precision		// Precision value for the floating-point numbers
	){
		if (precision > DIGITS)
			throw invalid_argument (string ("ObjectSummary: Max precision value is ") + to_string (DIGITS));

		// Change the precision value
		digits = precision;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Get precision for numeric values                                      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	size_t Precision (void) const {
		return digits;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//      Set name of the object                                                //
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
//      Header line to show before object properties                          //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	const header& Header (void) const {
		return header_str;
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
	stream.precision (object.Precision());

	// Print the object name
	string name = object.Name();
	transform (name.begin(), name.end(), name.begin(), [](unsigned char c) { return toupper(c); });
	stream << "╔═══════════════════════════════════════";
	stream << "═══════════════════════════════════════╗" << endl;
	stream << "║   ";
	stream.width (75);
	stream << left << name + ":" << "║" <<  endl;
	stream << "╠══════════════════════════════════════╤";
	stream << "═══════════════════════════════════════╣" << endl;

	// Print the header string
	const header &table_header = object.Header();
	stream << "║ ";
	stream.width (37);
	stream << left << table_header.first;
	stream << "│ ";
	stream.width (38);
	stream << left << table_header.second << "║" << endl;
	stream << "╟──────────────────────────────────────┼";
	stream << "───────────────────────────────────────╢" << endl;

	// Print all the groups
	const groups &grps = object.Groups();
	const size_t size = grps.size();
	const size_t last = size - 1;
	for (size_t i = 0; i < size; i++) {

		// Print the group
		stream << grps[i];

		// Print the group separating line
		if (i == last) {
			stream << "╚══════════════════════════════════════╧";
			stream << "═══════════════════════════════════════╝" << endl;
		}
		else {
			stream << "╟──────────────────────────────────────┼";
			stream << "───────────────────────────────────────╢" << endl;
		}
	}

	// Restore the precision
	stream.precision (restore);
	return stream;
}
/*
################################################################################
#                                 END OF FILE                                  #
################################################################################
*/
