#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>

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

