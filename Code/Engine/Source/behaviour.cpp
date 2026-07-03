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
{ delete this; }

Behaviour * Behaviour::Set ( const std::vector < std::string > &Args )
{
	( void ) Args;
	return  this;
}