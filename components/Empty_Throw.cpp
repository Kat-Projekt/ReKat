#include <engine.hpp>

class Empty_Throw : public Behaviour {
public:
	Empty_Throw ( )
	{
		DEBUG ( DebugLevel::FATAL, "This shouds throw" );
	}

	METADATA ( Empty, "empty component", 4,0,0,false );
};
