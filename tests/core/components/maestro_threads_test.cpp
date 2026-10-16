#include "../helper/test"

#include <core/maestro.h>

SCENARIO ( "components can be loaded concurrently", "[maestro][threads]" )
{
	GIVEN ( "an empty Maestro" )
	{
		Maestro m;

		WHEN ( "loading components" )
		{
			std::atomic < int > started_threads;
			std::atomic < bool > start;

			std::vector < std::string > comp_names = {
				"Empty_v1.dylib",
				"Empty_v2.dylib",
				"Empty_v3.dylib",
				"Empty_Throw.dylib",
				"TestReflection.dylib",
				"MaestroConcurrency.dylib"
			};

			std::vector < std::thread > threads;
			threads.reserve ( comp_names.size ( ) );

			std::atomic < int > registered_compo = 0;

			for ( const auto& name : comp_names )
			{
				threads.emplace_back (
					[&] ( ) {
						started_threads += 1;
						// wait for start flagg
						while ( !start ) { std::this_thread::yield ( ); }
						int r = m.Register ( name );
						registered_compo += r;
					}
				);

			}

			while ( started_threads != comp_names.size ( ) )
			{ std::this_thread::yield ( ); }
			start = true;

			for ( auto& th : threads )
			{ th.join ( ); }

			THEN ( "all components registered" )
			{
				REQUIRE ( registered_compo == comp_names.size ( ) );
			}
		}
	}
}

SCENARIO ( "a Maestro can allocate Behaviours on multiple threads", "[maestro][threads]" )
{
	GIVEN ( "an Iniziazized Maestro" )
	{
		Maestro m;
		auto r = m.Register_Directory ( "." );
		static const int times = 1000;

		THEN ( "6 components registered" )
		{
			REQUIRE ( r == 6 );	
		}

		WHEN ( "loading component" )
		{
			std::atomic < int > count;
			Call_Multiple (
				[&] () {
					auto c = m.Construct ( "MaestroConcurrency" );
					// this assumes that the behaviour is loaded correctly
					int r = c->Prepare ( "Return1" );
					count += r;
				}, times );
			
			THEN ( "all componets are creaded correctly" )
			{
				REQUIRE ( count == times );
			}
		}
	}
}

