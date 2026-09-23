#include "value.h"
#include "method.h"
#include "parameter.h"

class Behaviour
{
	virtual Reflection::Value _Perform 
	( const std::string&, const Reflection::Values& )
	{
		std::cout << "no methods reflected\n";
		return Reflection::Value();
	}

	virtual void _Configure
	( const Reflection::Values& )
	{ std::cout << "no parameters reflected\n"; }

	virtual Reflection::Value _Query
	( const std::string& )
	{
		std::cout << "no parameters to query\n";
		return Reflection::Value();
	}
	
public:
	// template wrappers
	template < typename ... Args >
	Reflection::Value Perform ( const std::string& name, Args&...args )
	{
		/* create empty state */
		Reflection::Values values;
		/* folds parameters */
		( values.operator, ( std::forward <Args> (args) ), ... );

		return _Perform ( name, values );
	}
	
	template < typename ... Args > 
	void Configure ( Args& ... paras )
	{
		/* create empty state */
		Reflection::Values values;
		/* folds parameters */
		( values.operator, ( std::forward <Args> (paras) ), ... );

		_Configure ( values );
	}

	Reflection::Value Query
	( const std::string& parameter )
	{ return _Query ( parameter ); }
};
