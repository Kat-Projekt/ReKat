#include "value.h"
#include <functional>

typedef std::function < Reflection::Value ( const Reflection::Values & ) > _generic_function_pointer;
typedef std::unordered_map < std::string, _generic_function_pointer > _methods_map;

#define METHODS(...) _methods_map methods = { __VA_ARGS__ };		\
Reflection::Value _Perform						\
( const std::string& name, const Reflection::Values& values )		\
override 								\
{									\
	ASSERT ( const auto& p = methods.find ( name );			\
			p == methods.end ( ),				\
		 "Missing method '" + name + "'" );			\
	/* Gets function pointer -> calls ( values ) */			\
	return methods [ name ] ( values );				\
}									\
std::vector < std::string > _Reflected_Methods ( ) const override	\
{									\
	std::vector < std::string > metho_names;			\
	metho_names.reserve ( methods.size ( ) );			\
									\
	for ( const auto& metho : methods )				\
	{ metho_names.push_back ( metho.first ); }			\
									\
	return metho_names;						\
}



// Wrappers
#define _INTERNAL_VOID_ARGUMENTS(function,...) { #function,		\
	_generic_function_pointer (					\
	[this]( const Reflection::Values & args ) -> Reflection::Value	\
	{								\
		function ( ENCODE ( __VA_ARGS__ ) );			\
		return Reflection::Value {} = std::monostate{}; 	\
	} ) }

#define _INTERNAL_VOID_NO_ARGUMENTS(function) { #function,		\
	_generic_function_pointer (					\
	[this]( const Reflection::Values & ) -> Reflection::Value	\
	{								\
		function ( );						\
		return Reflection::Value {} = std::monostate{};		\
	} ) }

#define _INTERNAL_VALUE_ARGUMENTS(function,...) { #function,		\
	_generic_function_pointer (					\
	[this]( const Reflection::Values & args ) -> Reflection::Value	\
	{								\
		auto result = function ( ENCODE ( __VA_ARGS__ ) );	\
		return Reflection::Value {} = result;	 		\
	} ) }

#define _INTERNAL_VALUE_NO_ARGUMENTS(function) { #function,		\
	_generic_function_pointer (					\
	[this]( const Reflection::Values & ) -> Reflection::Value	\
	{								\
		auto result = function ( );				\
		return Reflection::Value {} = result;			\
	} ) }

// Checks if there is a comma in the __VA_ARGS__:
// - if there is DEFAULT is resolved
// - if there is not NO_DEFAULT is resolved
#define _INTERNAL_ARGUMENTS_COMMA(_0,_1,_2,...) _2
#define _INTERNAL_ARGUMENTS_COMMA_WRAPPER(...) _INTERNAL_ARGUMENTS_COMMA ( __VA_ARGS__, DEFAULT, NO_DEFAULT, VARIADIC_PLACE_HOLDER )

// This is an helper that is called like
//
// 	_INTERNAL_ARGUMENTS_PARENTESIS_PROBE x
//
// Then:
// - if x is (...) resolves to: ,
// - if x is value does not resolve and remains: _INTERNAL_ARGUMENTS_PARENTESIS_PROBE x
#define _INTERNAL_ARGUMENTS_PARENTESIS_PROBE_COMMA(...) ,
#define _INTERNAL_ARGUMENTS_PARENTESIS_PROBE(...) _INTERNAL_ARGUMENTS_PARENTESIS_PROBE_COMMA ( __VA_ARGS__ )

// Calls the comma check with 1 or 2 arguments, passed by: has parentesis
#define _INTERNAL_ARGUMENTS_PARENTESIS_CHECK(...) _INTERNAL_ARGUMENTS_COMMA_WRAPPER ( __VA_ARGS__ )
// Checks if the x arguments is encapsulated in (...)
// Resolves to: DEFAULT
// - if (...) is provvided
// Resolves to: NO_DEFAULT
// - if x is provvided
#define _INTERNAL_ARGUMENTS_HAS_PARENTESIS(x)			\
	_INTERNAL_ARGUMENTS_PARENTESIS_CHECK (			\
		_INTERNAL_ARGUMENTS_PARENTESIS_PROBE x )

// Decision check resolves to the requested macro like:
// if condition is DEFAULT:
// 
// 	_INTERNAL_PROCESS_ARGUMENTS_IF_WRAPPER ( condition )
//
// Epands to
// - _INTERNAL_PROCESS_DEFAULT    if condition == DEFAULT
// - _INTERNAL_PROCESS_NO_DEFAULT if condition == NO_DEFAULT
//
// Then you call it
//
#define _INTERNAL_PROCESS_ARGUMENTS_IF(condition) _INTERNAL_PROCESS_##condition 
#define _INTERNAL_PROCESS_ARGUMENTS_IF_WRAPPER(condition) _INTERNAL_PROCESS_ARGUMENTS_IF(condition)

// processes the ( x, value ) case
#define _INTERNAL_PROCESS_DEFAULT_EXPAND(name,value) #name, value
#define _INTERNAL_PROCESS_DEFAULT(x) _INTERNAL_PROCESS_DEFAULT_EXPAND x

// processed the x case
#define _INTERNAL_PROCESS_NO_DEFAULT(x) #x

// used for processing DEFAULT argumens and non DEFAULTED arguments
// Resolves to: "arg", value 
// - if DEFAULT ( arg, value ) is provvided
// Resolves to: "arg" 
// - if arg is provvided
#define _INTERNAL_PROCESS_ARGUMENTS(x)				\
	_INTERNAL_PROCESS_ARGUMENTS_IF_WRAPPER			\
	(							\
	 	_INTERNAL_ARGUMENTS_HAS_PARENTESIS ( x )	\
	)							\
	( x )

// Numbered encoding argument cases ( 1-7 )
#define _INTERNAL_ENCODE_1(x)		args( _INTERNAL_PROCESS_ARGUMENTS(x) )
#define _INTERNAL_ENCODE_2(x,...)	args( _INTERNAL_PROCESS_ARGUMENTS(x) ), _INTERNAL_ENCODE_1 ( __VA_ARGS__ )
#define _INTERNAL_ENCODE_3(x,...)	args( _INTERNAL_PROCESS_ARGUMENTS(x) ), _INTERNAL_ENCODE_2 ( __VA_ARGS__ )
#define _INTERNAL_ENCODE_4(x,...)	args( _INTERNAL_PROCESS_ARGUMENTS(x) ), _INTERNAL_ENCODE_3 ( __VA_ARGS__ )
#define _INTERNAL_ENCODE_5(x,...)	args( _INTERNAL_PROCESS_ARGUMENTS(x) ), _INTERNAL_ENCODE_4 ( __VA_ARGS__ )
#define _INTERNAL_ENCODE_6(x,...)	args( _INTERNAL_PROCESS_ARGUMENTS(x) ), _INTERNAL_ENCODE_5 ( __VA_ARGS__ )
#define _INTERNAL_ENCODE_7(x,...)	args( _INTERNAL_PROCESS_ARGUMENTS(x) ), _INTERNAL_ENCODE_6 ( __VA_ARGS__ )

// Counts the number of __VA_ARGS__
#define _INTERNAL_COUNT_ARGS( _1, _2, _3, _4, _5, _6, _7, N, ... ) N
#define _INTERNAL_COUNT_ARGS_WRAPPER(...) _INTERNAL_COUNT_ARGS ( __VA_ARGS__, 7, 6, 5, 4, 3, 2, 1, 0 )

// Routes the parameter expansions to the correct ENCODE_N
#define _INTERNAL_ENCODE_EXPAND_WRAPPER(N,...) _INTERNAL_ENCODE_EXPAND ( N, __VA_ARGS__ )
#define _INTERNAL_ENCODE_EXPAND(N,...) _INTERNAL_ENCODE_##N ( __VA_ARGS__ )

// Main entry point: dispatches to the correct _ENCODE_N macro
#define ENCODE(...) _INTERNAL_ENCODE_EXPAND_WRAPPER ( _INTERNAL_COUNT_ARGS_WRAPPER ( __VA_ARGS__ ), __VA_ARGS__ )

// Numbered encoding argument cases ( 1-7 )
#define _INTERNAL_FUNCTION_ENCODER_1(type,function)	_INTERNAL_##type##_NO_ARGUMENTS ( function )
#define _INTERNAL_FUNCTION_ENCODER_2(type,function,...)	_INTERNAL_##type##_ARGUMENTS ( function, __VA_ARGS__ )
#define _INTERNAL_FUNCTION_ENCODER_3(type,function,...)	_INTERNAL_##type##_ARGUMENTS ( function, __VA_ARGS__ )
#define _INTERNAL_FUNCTION_ENCODER_4(type,function,...)	_INTERNAL_##type##_ARGUMENTS ( function, __VA_ARGS__ )
#define _INTERNAL_FUNCTION_ENCODER_5(type,function,...)	_INTERNAL_##type##_ARGUMENTS ( function, __VA_ARGS__ )
#define _INTERNAL_FUNCTION_ENCODER_6(type,function,...)	_INTERNAL_##type##_ARGUMENTS ( function, __VA_ARGS__ )
#define _INTERNAL_FUNCTION_ENCODER_7(type,function,...)	_INTERNAL_##type##_ARGUMENTS ( function, __VA_ARGS__ )

// calls the specific _INTERNAL_XX_ARGUMENTS
#define _INTERNAL_DISPATCHER_WRAPPER(type_line,count,...) _INTERNAL_DISPATCHER ( type_line, count, __VA_ARGS__ )
#define _INTERNAL_DISPATCHER(type_line,N,...) _INTERNAL_FUNCTION_ENCODER_##N ( type_line, __VA_ARGS__ )

/**********************************************************
 * \brief Macro for setting a default argument
 *
 * When reflecting a function you can set default arguments
 * so when not provided your default will be used.
 * Examples are under VALUE_FUNCTION and VOID_FUNCTION
 *********************************************************/
#define DEFAULT(x,value) (x,value)

/**************************************************************************
 * \brief Exposes to the reflection system a function that returns a value
 *
 * This macro must be called inside a METHODS macro under a Behaviour
 * 
 * Example:
 * - Asume that in the class A : public Behavior are defined the following:
 *   - std::string Format ( )
 *   - int Format ( int value )
 *
 * In the reflection this are expressed like:
 *
 * 	METHODS (
 *		VALUE_FUNCTION ( Format ),
 *		VALUE_FUNCTION ( Format, value ),
 *		...
 *	)
 *
 * If you want to use default parameters you can via the DEFAULT macro
 *
 * 	METHODS (
 *		VALUE_FUNCTION ( Format ),
 *		VALUE_FUNCTION ( Format, DEFAULT ( value, 10 ) )
 * 	)
 *
 * Example:
 * - You have to reflect the following function:
 *   - float Func ( int arg1, std::string arg2, double arg3, int * ptr )
 *   - You know that arg1 can be defaulted as 1 and ptr as nullptr.
 *
 * In the reflection this is exposed like
 *
 * 	METHODS (
 *		VALUE_FUNCTION ( Func,	DEFAULT ( arg1, 1 ), arg2,
 *					arg3, DEFAULT ( ptr, nulltr ) ),
 *		...
 * 	)
 *
 * Note that the default arguments are not all at the end.
 * The parameter order is more important the the default order.
 * Because when you call the function you use:
 *
 * 	b.Perform ( "Func", arg(arg3) = 1.0, arg(arg2) = "ciao" );
 * 
 * This is then converted to -> b.Func ( 10, "ciao", 1.0, nullptr );
 * The order is reconstructed by the reflection system.
 * To specify the default arguments you use: the same sintax:
 *
 * 	b.Perform ( "Func",	arg(arg1) = 3, 
 * 				arg(arg3) = 1.0,
 * 				arg(ptr)  = 0x123
 * 				arg(arg2) = "ciao" );
 *
 * Note that if you do not specify the non defaulted argument
 * an error is thrown:
 *
 * 	b.Perform ( "Func", arg(arg1) = 2 );
 *			  ^^ missing arguments arg2 and arg3
 * 
 *************************************************************************/
#define VALUE_FUNCTION(...) _INTERNAL_DISPATCHER_WRAPPER (	\
	VALUE,							\
	_INTERNAL_COUNT_ARGS_WRAPPER ( __VA_ARGS__ ),		\
	__VA_ARGS__						\
)
/*******************************************************************
 * \brief Works exacly like VALUE_FUNCTION but "discards" the result
 *
 * This is expected to be used in 2 scenarios:
 * - a void function ( ... )
 * - a function where the return value is expected to be ignored.
 *
 * For examples look under the VALUE_FUNCTION docs.
 ******************************************************************/
#define VOID_FUNCTION(...) _INTERNAL_DISPATCHER_WRAPPER (	\
	VOID,							\
	_INTERNAL_COUNT_ARGS_WRAPPER ( __VA_ARGS__ ),		\
	__VA_ARGS__						\
)

