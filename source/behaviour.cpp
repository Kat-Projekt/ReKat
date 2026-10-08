#include <core/behaviour.h>

// publically callable update functions (Aktor->...) + _active / start gates
void Behaviour::_Start ( )
{
	std::lock_guard < std::mutex > execution_lock ( executing );
	if ( _active.load ( ) && !_started.load ( ) )
	{ 
		Start ( );
		_started.store ( true );

	} else { }
}

void Behaviour::_Internal_Caller ( void ( Behaviour::* function ) ( void ) )
{
	std::lock_guard < std::mutex > execution_lock ( executing );
	if ( _active.load ( ) && _started.load ( ) )
	{
		( this->*function ) ( );

	} else { }
}

// Collision / Trigger routing + _start / _active gates
void Behaviour::_Collsion_Router ( Aktor * _obj, int mode, bool trigger )
{
	std::lock_guard < std::mutex > execution_lock ( executing );
	if ( _active && _started )
	{	
		if ( trigger )
		{
			switch ( mode ) {
				case 0: Collision_Trigger_Enter ( _obj ); break;
				case 1: Collision_Trigger ( _obj ); break;
				case 2: Collision_Trigger_Exit ( _obj ); break;
				default: break;
			}
		} else {
			switch ( mode ) {
				case 0: Collision_Enter ( _obj ); break;
				case 1: Collision ( _obj ); break;
				case 2: Collision_Exit ( _obj ); break;
				default: break;
			}
		}

	} else { }
}

