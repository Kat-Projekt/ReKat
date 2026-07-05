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

void Behaviour::_Update ( )
{
	DEBUG ( 6, "tring to update component: ", typeid (*this).name( ) );
	if ( _active )
	{ Update ( ); }
}

void Behaviour::_Fixed_Update ( )
{
	if ( _active )
	{ Fixed_Update ( ); }
}

void Behaviour::Start ( ) { }
void Behaviour::Update ( ) { }
void Behaviour::Fixed_Update ( ) { }

void Behaviour::Collision ( std::shared_ptr < Objekt > _obj )
{ ( void ) _obj; }
void Behaviour::Collision_Exit ( std::shared_ptr < Objekt > _obj )
{ ( void ) _obj; }
void Behaviour::Collision_Enter ( std::shared_ptr < Objekt > _obj )
{ ( void ) _obj; }
void Behaviour::Collision_Trigger ( std::shared_ptr < Objekt > _obj )
{ ( void ) _obj; }
void Behaviour::Collision_Trigger_Exit ( std::shared_ptr < Objekt > _obj )
{ ( void ) _obj; }
void Behaviour::Collision_Trigger_Enter ( std::shared_ptr < Objekt > _obj )
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

std::shared_ptr <Behaviour> Behaviour::Set ( const std::vector < std::string > &Args )
{
	( void ) Args;
	return shared_from_this ( );
}

Behaviour::Component_Metadata Behaviour::Get_Info ( )
{
	return Informations;
}

Behaviour::Component_Metadata::Component_Metadata
( const char* _name, float _version, const char* _description )
: name(_name), version(_version), description(_description) { }

Behaviour::Component_Metadata::Component_Metadata
( ) { }