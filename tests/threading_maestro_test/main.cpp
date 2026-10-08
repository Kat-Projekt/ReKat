#include "../helper/test"

#include <core/maestro.h>

SCENARIO ( "a Maestro can allocate Behaviours on multiple threads" )
{
	GIVEN ( "an Iniziazized Maestro" )
	{
		Maestro m;
	}
}

SCENARIO ( "the Start function on a Behaviour can only be Called Once" )
{

}

SCENARIO ( "the Update functions ( Fixed, Late, ...) on a Behaviour can be Called once per Frame" )
{

}

SCENARIO ( "the Configuration can happen only outside Updates" )
{

}

SCENARIO ( "the Query can be executed always" )
{

}

SCENARIO ( "the Perform is executed only outside Update and Configuration" )
{

}

