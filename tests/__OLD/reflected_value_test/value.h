#pragma once

#include "editor_types.hpp"
#include <vector>
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
};


class Reflection::Value
{
private:
	std::string _name;
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
		_value = value;
		return *this;
	}

	template < typename T >
	operator T ( ) const
	{
		try
		{ return _value.Get < T > ( ); }
		catch
			( const std::bad_variant_access& e )
		{
			throw std::runtime_error (
			    "Cannot convert '" +
			    _value.Name ( ) +
			    "' to requested type"
			);
		}
	}

	[[nodiscard]] const std::string& Name ( ) const 
	{ return _name; }

	[[nodiscard]] const std::string& Type ( ) const 
	{ return _value.Name ( ); }
};

class Reflection::Values
{
private:
	std::vector < Reflection::Value > _args;
public:
	Values
	( const Value& start )
	{ _args.push_back ( start ); }

	Values
	( ) { }

	auto begin ( ) const { return _args.begin ( ); }
	auto end ( ) const { return _args.end ( ); }


	Values& operator ,
	( const Reflection::Value& con ) // use move refence
	{
		_args.push_back ( std::move ( con ) );
		return *this;
	}


	const Value& operator ()
	( const std::string& name ) const
	{
		static const Value empty ( "empty" );
		for ( const auto& v : _args )
		{
			if ( v.Name ( ) == name )
			{ return v; }
		}

		return empty;
	}

	template < typename T >
	T operator ()
	( const std::string& name, const T& default_value ) const
	{
		static const Value empty ( "empty" );
		for ( const auto& v : _args )
		{
			std::cout << "v " << v.Name () << " " << v.Type ( ) << std::endl;
			if ( v.Name ( ) == name )
			{ return static_cast < T > ( v ); }
		}
		
		return default_value;
	}
};

// helper macro for defining Values
#define arg(name) Reflection::Value(#name)

