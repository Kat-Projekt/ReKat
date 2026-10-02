#pragma once

#include "editor_types.h"
#include "../diagnostic/assert.h"
#include <unordered_map>
#include <iostream>

namespace Reflection
{
	/***************************************************************************
	 * \brief This class contains a variable and its name
	 *
	 * To access the variable contents you can type cast the Value
	 * To create a Value you give it a name and a value like:
	 *
	 * ```cpp
	 * auto v = arg(name);			// using the macro
	 * auto v = Reflection::Value("name");	// using the simbol
	 * v = 10;				// sets the value of "name" to 10
	 * auto name = v.Name ( );		// gets the name
	 * int value = v;			// implicitally converts v to int
	 * vec3 errr = v;	// since the value of v is int this throws an error
	 * ```
	 *************************************************************************/
	class Value;
	/**********************************************************************
	 * \brief This class is a containter for Value
	 *
	 * You can iterate Values stored and set/get Value
	 *
	 * ```cpp
	 * Reflection::Values args;
	 * args, arg(arg1) = 10, arg(arg2) = 0.1f, arg(ciao) = "ciao";
	 *
	 * // Iterate the Values
	 * for ( const auto& arg : args )
	 * 	std::cout << arg.Name ( );
	 *
	 * int arg1  = args ("arg1");		// gets the value of arg1
	 * auto arg1 = args ("arg1", 10);	// this has a defualt value
	 * 		// so if arg1 is not set or corrupted 10 will be used
	 * ```
	 *******************************************************************/
	class Values;
}


class Reflection::Value
{
private:
	const char * _name;
	EditorType _value;
public:
	Value ( )
	: _name ( "" ), _value ( std::monostate { } ) { }

	Value
	( const char* name )
	: _name ( name ) { }

	template < typename T >
	Value& operator =
	( T value )
	{
		using C = std::decay_t < T >;
		static_assert ( variant_contains < C, EditorType >::value );
		_value = value;
		return *this;
	}

	template < typename T >
	operator T ( ) const
	{
		try {
			return _value.Get < T > ( );
		} catch
			( const std::bad_variant_access& )
		{
			DEBUG ( DebugLevel::ERROR, "Type missmatch '", _value.Name ( ), "' for '", _name, "'" );
			throw;
		}
	}

	[[nodiscard]] const char * Name ( ) const 
	{ return _name; }

	[[nodiscard]] const char * Type ( ) const 
	{ return _value.Name ( ); }
};

class Reflection::Values
{
private:
	std::unordered_map < std::string, Reflection::Value > _args;
public:
	Values
	( const Value& start )
	{ _args[ start.Name ( ) ] = std::move ( start ); }

	Values
	( ) { }

	auto begin ( ) const { return _args.begin ( ); }
	auto end ( ) const { return _args.end ( ); }


	Values& operator ,
	( Reflection::Value&& con ) // use move refence
	{
		auto v = _args.find ( con.Name ( ) );
		ASSERT ( v != _args.end ( ), std::string ( "Duplicate value: " ) + con.Name ( ) );
		
		_args [ con.Name ( ) ] = std::move ( con );
		
		return *this;
	}


	const Value& operator ( ) 
	( const std::string& name ) const
	{
		auto v = _args.find ( name );
		ASSERT ( v == _args.end ( ), "Missing value: " + name );
		
		return v->second;
	}

	template < typename T >
	T operator ( )
	( const std::string& name, const T& default_value ) const
	{
		auto v = _args.find ( name );
		if ( v == _args.end ( ) )
		{
			return default_value;
		} else {
			return static_cast < T > ( v->second );
		}
	}
};

// helper macro for defining Values
#define arg(name) Reflection::Value(#name)

