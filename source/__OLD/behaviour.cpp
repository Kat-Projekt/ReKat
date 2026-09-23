#include "_objekt_implentation_details/behaviour.h"

void Behaviour::_Start ( )
{
	if ( _active && !_started )
	{ Start ( ); }

	_started = true;
}

void Behaviour::_Early_Update ( )
{
	if ( _active )
	{ Early_Update ( ); }
}

void Behaviour::_Update ( )
{
	if ( _active )
	{ Update ( ); }
}

void Behaviour::_Late_Update ( )
{
	if ( _active )
	{ Late_Update ( ); }
}

void Behaviour::_Fixed_Update ( )
{
	if ( _active )
	{ Fixed_Update ( ); }
}

Behaviour * Behaviour::Set ( const ComponentArguments& args )
{
	auto pars = Get_Parameters ( );
	for ( auto par : pars ) 
	{ par.Set ( this, args ); }
}

const ComponentArguments Behaviour Get ( ) const
{
	ComponentArguments getted_args;
	auto pars = Get_Parameters ( );
	for ( auto par : pars ) 
	{ par.Get ( this, getted_args ) }

	return getted_args;
}