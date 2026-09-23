#include "value.h"
#include <unordered_map>
#include <string>

typedef std::function < void ( const Reflection::Values& ) > setter; 
typedef std::function < Reflection::Value ( ) > getter;
typedef std::unordered_map < std::string, std::pair < setter, getter > > _parameters_map;

#define PARAMETERS(...) _parameters_map parameters = { __VA_ARGS__ };	\
void _Configure ( const Reflection::Values& values )			\
{									\
	for ( const auto& param : values )				\
	{								\
		/* set that value */					\
		parameters [ param.Name ( ) ].first ( param );		\
	}								\
}									\
Reflection::Value _Query ( const std::string& parameter )		\
{									\
	/* calls the getter for parameter*/				\
	return parameters [ parameter ].second ( );			\
}

		
#define PARAMETER(name)						\
{ #name,							\
{								\
	[this]( const Reflection::Values& args ) {		\
		this -> name = args(#name);			\
	},							\
	[this]( ) -> Reflection::Value {			\
		return Reflection::Value {} = this->name;	\
	}							\
} }

#define PROPERTY(name,setter,getter)				\
{ #name,							\
{								\
	[this]( const Reflection::Values& args ) {		\
		setter ( args(#name) );				\
	},							\
	[this]() -> Reflection::Value {				\
		return Reflection::Value{} = getter ( );	\
	}							\
} }

