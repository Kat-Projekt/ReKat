#include <engine.hpp>

class Empty_Throw : public Behaviour {
public:
	Empty_Throw ( )
	{
		DEBUG ( DebugLevel::FATAL, "This shouds throw" );
	}

	METADATA ( Empty, "empty component that throws on cosntruction that throws on cosntruction", 4,0,0,false )
};
