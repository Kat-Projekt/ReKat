#include "value.h"
#include <functional>

typedef std::function < Reflection::Value ( const Reflection::Values & ) > _generic_function_pointer;
typedef std::unordered_map < std::string, _generic_function_pointer > _methods_map;

#define METHODS(...) _methods_map methods = { __VA_ARGS__ };	\
Reflection::Value _Perform					\
( const std::string& name, const Reflection::Values& values )	\
override 							\
{								\
	ASSERT ( const auto& p = methods.find ( name );		\
			p == methods.end ( ),			\
		 "Missing method" + name );			\
	/* Gets function pointer -> calls ( values ) */		\
	return methods [ name ] ( values );			\
}

// Numbered Cases
#define ENCODE_1(x)	args(#x)
#define ENCODE_2(x,...)	args(#x), ENCODE_1 ( __VA_ARGS__ )
#define ENCODE_3(x,...)	args(#x), ENCODE_2 ( __VA_ARGS__ )
#define ENCODE_4(x,...)	args(#x), ENCODE_3 ( __VA_ARGS__ )
#define ENCODE_5(x,...)	args(#x), ENCODE_4 ( __VA_ARGS__ )
#define ENCODE_6(x,...)	args(#x), ENCODE_5 ( __VA_ARGS__ )
#define ENCODE_7(x,...)	args(#x), ENCODE_6 ( __VA_ARGS__ )

// Counts the number of __VA_ARGS__
#define COUNT_ARGS( _1, _2, _3, _4, _5, _6, _7, N, ... ) N
#define COUNT_ARGS_WRAPPER(...) COUNT_ARGS ( __VA_ARGS__, 7, 6, 5, 4, 3, 2, 1, 0 )

// Routes the call to the correct ENCODE_n
#define ENCODE_EXPAND_WRAPPER(N,...) ENCODE_EXPAND ( N, __VA_ARGS__ )
#define ENCODE_EXPAND(N,...) ENCODE_##N ( __VA_ARGS__ )

// Main entry point: dispatches to the correct _ENCODE_N macro
#define ENCODE(...) ENCODE_EXPAND_WRAPPER ( COUNT_ARGS_WRAPPER ( __VA_ARGS__ ), __VA_ARGS__ )

#define METHOD(function,...) { #function,				\
	_generic_function_pointer (					\
	[this]( const Reflection::Values & args ) -> Reflection::Value	\
	{								\
		auto result = function ( ENCODE ( __VA_ARGS__ ) );	\
		return Reflection::Value {} = result; 			\
	} ) }

#define NO_ARGUMENTS(function) { #function,				\
	_generic_function_pointer (					\
	[this]( const Reflection::Values & ) -> Reflection::Value	\
	{								\
		auto result = function ( );				\
		return Reflection::Value {} = result;			\
	} ) }
