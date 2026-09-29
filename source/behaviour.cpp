#include <core/behaviour.h>

// publically callable update functions (Aktor->...) + _active / start gates
void Behaviour::_Start ( )
{
	if ( _active && ! _started )
	{
		Start ( );
		_started = true;
	} else { }
}
void Behaviour::_Early_Update ( )
{
	if ( _active && _started )
	{
		Early_Update ( );
	} else { }
}
void Behaviour::_Update ( )
{
	if ( _active && _started )
	{
		Update ( );
	} else { }
}
void Behaviour::_Late_Update ( )
{
	if ( _active && _started )
	{
		Late_Update ( );
	} else { }
}
void Behaviour::_Fixed_Update ( )
{
	if ( _active && _started )
	{
		Fixed_Update ( );
	} else { }
}

// Collision / Trigger routing + _start / _active gates
void Behaviour::_Collsion_Enter ( Aktor * _obj, bool trigger )
{
	if ( _active && _started )
	{
		if ( trigger )
		{ Collision_Trigger_Enter ( _obj ); }
		else 
		{ Collision_Enter ( _obj ); }
	} else { }
}
void Behaviour::_Collsion ( Aktor * _obj, bool trigger )
{
	if ( _active && _started )
	{
		if ( trigger )
		{ Collision_Trigger ( _obj ); }
		else 
		{ Collision ( _obj ); }
	} else { }
}

void Behaviour::_Collsion_Exit ( Aktor * _obj, bool trigger )
{
	if ( _active && _started )
	{
		if ( trigger )
		{ Collision_Trigger_Exit ( _obj ); }
		else 
		{ Collision_Exit ( _obj ); }
	} else { }
}


