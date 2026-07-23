#include "objekt/behaviour.hpp"

Behaviour::Behaviour ( ) { }

Behaviour::~Behaviour ( ) 
{ Delete ( ); }

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
	DEBUG ( 6, "tring to update component: ", typeid (*this).name( ) );
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

void Behaviour::Start ( ) { }
void Behaviour::Early_Update ( ) { }
void Behaviour::Update ( ) { }
void Behaviour::Late_Update ( ) { }
void Behaviour::Fixed_Update ( ) { }

void Behaviour::Collision
( Objekt* _obj )
{ ( void ) _obj; }
void Behaviour::Collision_Exit
( Objekt* _obj )
{ ( void ) _obj; }
void Behaviour::Collision_Enter
( Objekt* _obj )
{ ( void ) _obj; }
void Behaviour::Collision_Trigger
( Objekt* _obj )
{ ( void ) _obj; }
void Behaviour::Collision_Trigger_Exit
( Objekt* _obj )
{ ( void ) _obj; }
void Behaviour::Collision_Trigger_Enter
( Objekt* _obj )
{ ( void ) _obj; }

void Behaviour::Set_Active ( bool active )
{
	_active = active;

	if ( !_started )
	{ Start ( ); }
}

bool Behaviour::Get_Active ( )
{ return _active; }

void Behaviour::Delete ( )
{ }

/* Behaviour& Behaviour::Set
( const std::vector < std::string > &Args )
{
	( void ) Args;
	return *this;
} */

const Behaviour::Component_Metadata& Behaviour::Get_Info
( ) const
{
	return Informations;
}

Behaviour::Component_Metadata::Component_Metadata
(
	const char* _name,
	float _version,
	const char* _description,
	std::vector < const char* > _arguments
)
: name(_name), version(_version), description(_description), arguments(_arguments)
{ }

Behaviour::Component_Metadata::Component_Metadata
( ) { }

const char* Behaviour::Get_Type ( )
{ return Informations.name; }