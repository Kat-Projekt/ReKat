#include <string>
#include <variant>

template < typename T, typename Variant >
struct variant_contains;

template < typename T, class... Ts>
struct variant_contains < T, std::variant < Ts... > >
: std::disjunction < std::is_constructible < Ts, T > ... >
{ };


class EditorType {
public:
	typedef std::variant <
		std::monostate,
		bool,
		int,
		float,
		std::string
	> variant_type;
private:
	inline static constexpr const char * names [5] = {"mono","bool","int","float","string"};
	variant_type _value;
public:
	EditorType ( ) = default;
	
	template < typename T >
	EditorType ( T&& value )
	: _value ( std::forward < T > ( value ) )
	{ }

	EditorType ( const char * value )
	: _value ( std::string ( value ))
	{ }

	const char * Name ( ) const
	{ return names [ _value.index ( ) ]; }

	template < typename T >
	const T& Get ( ) const
	{ return std::get < T > ( _value ); }
};

template < typename T >
struct variant_contains < T, EditorType >
    : variant_contains < T, EditorType::variant_type >
{ };
