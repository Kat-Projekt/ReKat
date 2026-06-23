#include "Objekt/behaviour.hpp"

Behaviour::Behaviour ( ) { }

Behaviour::~Behaviour ( ) { }

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

void Behaviour::Collision ( Objekt * _obj )
{ ( void ) _obj; }
void Behaviour::Collision_Exit ( Objekt * _obj )
{ ( void ) _obj; }
void Behaviour::Collision_Enter ( Objekt * _obj )
{ ( void ) _obj; }
void Behaviour::Collision_Trigger ( Objekt * _obj )
{ ( void ) _obj; }
void Behaviour::Collision_Trigger_Exit ( Objekt * _obj )
{ ( void ) _obj; }
void Behaviour::Collision_Trigger_Enter ( Objekt * _obj )
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

Behaviour * Behaviour::Set ( ... )
{ return this; }