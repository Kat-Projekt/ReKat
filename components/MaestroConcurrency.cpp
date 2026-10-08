#include <core/behaviour.h>

class MaestroConcurrency : public Behaviour
{
	int Return1 ( )
	{ return 1; }

public:
	METADATA ( MaestroConcurrency, "exposes the Return1 function", 1, 0, 0 )
	METHODS (
		VALUE_FUNCTION ( Return1 )
	)
};

