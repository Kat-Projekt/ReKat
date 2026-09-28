#include <core/reflection/metadata.h>

bool Reflection::Is_Version_Major ( const Reflection::Metadata& lower, const Reflection::Metadata& upper )
{
	if ( upper.major > lower.major )
	{ return true; }
	else if ( upper.minor > lower.minor )
	{ return true; }
	else if ( upper.patch > lower.patch )
	{ return true; }
	else
	{ return false; }
}

std::string Reflection::Version_Number ( const Reflection::Metadata& meta )
{	
	std::string r = ""; // version 
	r += std::to_string ( meta.major );
	r += ".";
	r += std::to_string ( meta.minor );
	r += ".";
	r += std::to_string ( meta.patch );
	r += ( meta.stable ? "[v]" : "[x]" );
	return r;

}

std::ostream& Reflection::operator << ( std::ostream& os, const Reflection::Metadata& meta )
{
	os << meta.name;
	os << " ";
	os << Version_Number ( meta );
	os << " ( ";
	os << meta.description;
	os << " )";
	return os;
}
