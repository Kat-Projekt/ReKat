#include <test>

#include <core/aktor.h>

SCENARIO ( "An actor can manage components" )
{
	GIVEN ( "A Global Maestro and an Aktor" )
	{
		Maestro m;
		m.Set_Global ( );
		int r = m.Register_Directory ( "." );

		Aktor a;

		THEN ( "Maestro Loaded Correctly" )
		{
			REQUIRE ( r == 5 ); 
		}

		WHEN ( "Aktor Adding Behaviours" )
		{
			a.Add_Behaviour ( "AddTest" );
			a.Add_Behaviour ( "Empty", false, 3, 0, 0 );

			THEN ( "Aktor Has Behaviours" )
			{
				REQUIRE_NOTHROW ( a.Get_Behaviour ( "AddTest" ) );
				REQUIRE_NOTHROW ( a.Get_Behaviour ( "Empty", false, 3 ) );
			}
		}

		WHEN ( "Aktor can Remove Behaviours" )
		{
			a.Add_Behaviour ( "RemTest" );
			a.Rem_Behaviour ( "RemTest" );

			THEN ( "Aktor Han zero Behaviours" )
			{
				REQUIRE_THROWS ( a.Get_Behaviour ( "RemTest" ) );
			}
		}

		WHEN ( "Aktor can Get Behaviours" )
		{
			a.Add_Behaviour ( "Empty" );
			
			THEN ( "Aktor Gets components" )
			{
				REQUIRE ( a.Get_Behaviour ( "Empty" ) != nullptr );
				REQUIRE_THROWS ( a.Get_Behaviour ( "AddTest" ) );
				REQUIRE_THROWS ( a.Get_Behaviour ( "Empty", false, 12,3,5 ) );
			}
		}
	}
}

SCENARIO ( "An actor garantes Behaviour uniqueness" )
{
	GIVEN ( "A Global Maestro and an Aktor" )
	{
		Maestro m;
		m.Set_Global ( );
		int r = m.Register_Directory ( "." );

		Aktor a;

		THEN ( "Maestro Loaded Correctly" )
		{
			REQUIRE ( r == 5 );	
		}
	
		WHEN ( "Adding a component" )
		{
			auto test_E = a.Add_Behaviour ( "Empty", false, 1 );
			
			THEN ( "Adding a component of the same type" )
			{
				REQUIRE ( a.Add_Behaviour ( "Empty", false, 1 ) == test_E );
				REQUIRE_NOTHROW ( a.Add_Behaviour ( "Empty", true, 2 ) );

				REQUIRE ( a.Get_Behaviour ( "Empty" )->Metadata ( ).major == 2 );
			}
		}
	}
}

