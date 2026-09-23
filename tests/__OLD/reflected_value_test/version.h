#include <iostream>
namespace Reflection {
struct Version
{
	u_int32_t major = 0;
	u_int32_t minor = 0;
	u_int32_t patch = 0;
	bool stable = false;

	constexpr Version ( u_int32_t major = 0, u_int32_t minor = 0, u_int32_t patch = 0, bool stable = false )
	: major ( major ), minor ( minor ), patch ( patch ), stable ( stable )
	{ }

	constexpr bool operator > ( const Version& other ) const
	{
		if ( major > other.major )
		{ return true; }
		else if ( minor > other.minor )
		{ return true; }
		else if ( patch > other.patch )
		{ return true; }
		else
		{ return false; }
	}

	constexpr bool operator == ( const Version& other ) const
	{
		return (
			major == other.major &&
			minor == other.minor &&
			patch == other.patch
		);
	}

	constexpr bool operator < ( const Version& other ) const
	{ return other > *this; }
	constexpr bool operator != ( const Version& other ) const
	{ return ! ( *this == other ); }

	friend std::ostream& operator << ( std::ostream& os, const Version& v )
	{
		os << v.major;
		os << ".";
		os << v.minor;
		os << ".";
		os << v.patch;
		os << ( v.stable ? "[v]" : "[x]" );
		return os;
	}	
};
};
