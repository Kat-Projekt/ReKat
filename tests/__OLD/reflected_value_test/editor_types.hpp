#include <string>
#include <variant>

class EditorType {
	inline static const std::string names [5] = {"mono","bool","int","float","string"};
	std::variant <
		std::monostate,
		bool,
		int,
		float,
		std::string
	> _value;
public:
	EditorType ( ) = default;
	
	template < typename T >
	EditorType ( T&& value )
	: _value ( std::move ( value ) )
	{ }

	EditorType ( const char* value )
	: _value ( std::string ( value ))
	{ }

	const std::string& Name ( ) const
	{ return names [ _value.index ( ) ]; }

	template < typename T >
	const T& Get ( ) const
	{ return std::get < T > ( _value ); }
};
