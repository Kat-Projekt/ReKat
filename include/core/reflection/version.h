#include <iostream>
namespace Reflection {
class Version
{
	u_int32_t _major = 0;
	u_int32_t _minor = 0;
	u_int32_t _patch = 0;
	bool _stable = false;

public:
	constexpr Version ( u_int32_t major = 0, u_int32_t minor = 0, u_int32_t patch = 0, bool stable = false )
	: _major ( major ), _minor ( minor ), _patch ( patch ), _stable ( stable )
	{ }

	constexpr bool operator > ( const Version& other ) const
	{
		if ( _major > other._major )
		{ return true; }
		else if ( _minor > other._minor )
		{ return true; }
		else if ( _patch > other._patch )
		{ return true; }
		else
		{ return false; }
	}

	constexpr bool operator == ( const Version& other ) const
	{
		return (
			_major == other._major &&
			_minor == other._minor &&
			_patch == other._patch
		);
	}

	constexpr bool operator < ( const Version& other ) const
	{ return other > *this; }
	constexpr bool operator != ( const Version& other ) const
	{ return ! ( *this == other ); }

	friend std::ostream& operator << ( std::ostream& os, const Version& v )
	{
		os << v._major;
		os << ".";
		os << v._minor;
		os << ".";
		os << v._patch;
		os << ( v._stable ? "[v]" : "[x]" );
		return os;
	}	
};
};
