#include <catch2/catch_test_macros.hpp>
#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>

#include <iostream>

#include <core/maestro.h>

class SectionLogger : public Catch::EventListenerBase {
public:
	using Catch::EventListenerBase::EventListenerBase;

	void sectionStarting( Catch::SectionInfo const& sectionInfo) override 
	{ DEBUG ( DebugLevel::WARN, sectionInfo.name ); }
};

CATCH_REGISTER_LISTENER(SectionLogger)


SCENARIO ( "a maestro can register components", "[registration]" )
{
	GIVEN( "a maestro" )
	{
		Maestro m;
		int r = m.Register ( "Empty_v1.dylib" );

		THEN ( "the maestro has 1 registered component" )
		{
			REQUIRE ( r == 1 ); // registration success
			REQUIRE ( m.Get_Registered_Components ( ).size ( ) == 1 );
		}

		WHEN ( "a new component is registered" )
		{
			r = m.Register ( "Empty_v2.dylib" );
			THEN ( "the maestro has 2 registered components" )
			{
				REQUIRE ( r == 1 ); // registration success
				REQUIRE ( m.Get_Registered_Components ( ).size ( ) == 2 );
			}
		}

		WHEN ( "a component is re-registered" )
		{
			r = m.Register ( "Empty_v1.dylib" );
			THEN ( "the maestro has 1 registered components" )
			{
				REQUIRE ( r == 0 ); // registration failed
				REQUIRE ( m.Get_Registered_Components ( ).size ( ) == 1 );
			}
		}

		WHEN ( "loading the directory" )
		{
			r = m.Register_Directory ( "." );
			THEN ( "the maestro registered all components" )
			{
				REQUIRE ( r == 4 ); // registration of Emtpty_1 failed
				REQUIRE ( m.Get_Registered_Components ( ).size ( ) == 5 );
				DEBUG ( DebugLevel::NOTICE, m.Get_Registered_Components ( ) );
			}
		}
	}
}
