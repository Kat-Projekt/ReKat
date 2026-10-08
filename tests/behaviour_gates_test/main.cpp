#include "../helper/test"

#include <core/behaviour.h>

class Behaviour_Test : public Behaviour
{
public:
	int update_calls = 0;
	int start_calls = 0;
	int collision_calls = 0;
	int reflected_function = 0;
	mutable int cache = 0;
private:
	void Start ( ) override { start_calls ++; }
	void Update ( ) override { update_calls ++; }
	void Fixed_Update ( ) override { update_calls ++; Set_Active ( false ); }

	void Collision_Enter ( Aktor * ) override { collision_calls ++; }

	void function ( ) { reflected_function ++; };

	int query ( ) const
	{ cache ++; return cache; }

	void setter ( int ) { cache ++; }

	METHODS (
		VOID_FUNCTION ( function )
	)

	PARAMETERS (
		PROPERTY ( test, setter, query )
	)
};

SCENARIO ( "Behaviour call order is correct", "[behaviour]" )
{
	GIVEN ( "a Behaviour" )
	{
		Behaviour_Test b;

		WHEN ( "Updating whitout start" )
		{
			b._Update ( );
			b._Update ( );

			THEN ( "no update calls made" )
			{
				REQUIRE ( b.update_calls == 0 );
			}
		}

		WHEN ( "Updating after starting" )
		{
			b._Start ( );
			b._Update ( );

			THEN ( "update occured correcttly" )
			{
				REQUIRE ( b.start_calls == 1 );
				REQUIRE ( b.update_calls == 1 );
			}
		}

		WHEN ( "Starting multiple times" )
		{
			b._Start ( );
			b._Start ( );

			THEN ( "Started only once" )
			{
				REQUIRE ( b.start_calls == 1 );
			}
		}

		WHEN ( "Colliding before start" )
		{
			b._Collsion_Router ( nullptr, 0, false );
			b._Collsion_Router ( nullptr, 0, false );


			THEN ( "Collisions after start" )
			{
				REQUIRE ( b.collision_calls == 0 );
			}
		}
	
		WHEN ( "Colliding after start" )
		{
			b._Start ( );
			b._Collsion_Router ( nullptr, 0, false );
			b._Collsion_Router ( nullptr, 0, false );


			THEN ( "Collisions after start" )
			{
				REQUIRE ( b.collision_calls == 2 );
			}
		}
	}
}

SCENARIO ( "Call concurrency for a behaviour", "[behaviour][threads]"  )
{
	GIVEN ( "A behaviour" )
	{	
		Behaviour_Test b;
		static const int times = 1000;

		THEN ( "Correctly setted up" )
		{
			REQUIRE ( b.Reflected_Methods ( ) == std::vector < std::string > {"function"} );
			REQUIRE ( b.Reflected_Parameters ( ) == std::vector < std::string > {"test"} );
		}

		WHEN ( "Updating whitout start" )
		{
			Call_Multiple ( [&] () {b._Update ( ); }, times );

			THEN ( "no update calls made" )
			{
				REQUIRE ( b.update_calls == 0 );
			}
		}

		WHEN ( "Updating after starting" )
		{
			b._Start ( );
			Call_Multiple ( [&] () {b._Update ( ); }, times );

			THEN ( "update occured correcttly" )
			{
				REQUIRE ( b.start_calls == 1 );
				REQUIRE ( b.update_calls == times );
			}
		}

		WHEN ( "Updating after starting then deactivatign the component" )
		{
			b._Start ( );
			Call_Multiple ( [&] () {b._Fixed_Update ( ); }, times );

			THEN ( "update occured correcttly" )
			{
				REQUIRE ( b.start_calls == 1 );
				REQUIRE ( b.update_calls == 1 );
			}
		}


		WHEN ( "Starting multiple times" )
		{
			Call_Multiple ( [&] () {b._Start ( ); }, times );

			THEN ( "Started only once" )
			{
				REQUIRE ( b.start_calls == 1 );
			}
		}

		WHEN ( "Colliding before start" )
		{
			Call_Multiple ( [&] () { b._Collsion_Router ( nullptr, 0, false ); }, times );

			THEN ( "Collisions after start" )
			{
				REQUIRE ( b.collision_calls == 0 );
			}
		}
	
		WHEN ( "Colliding after start" )
		{
			b._Start ( );
			Call_Multiple ( [&] () { b._Collsion_Router ( nullptr, 0, false ); }, times );

			THEN ( "Collisions after start" )
			{
				REQUIRE ( b.collision_calls == times );
			}
		}

		WHEN ( "Performing before start" )
		{
			Call_Multiple ( [&] () { b.Perform ( "function" ); }, times );

			THEN ( "Collisions after start" )
			{
				REQUIRE ( b.reflected_function == 0 );
			}
		}
	
		WHEN ( "Performing after start" )
		{
			b._Start ( );
			
			Call_Multiple ( [&] () { b.Perform ( "function" ); }, times );

			THEN ( "Collisions after start" )
			{
				REQUIRE ( b.reflected_function == times );
			}
		}

		WHEN ( "concurrently modifing chache via query" )
		{
			Call_Multiple ( [&] () { b.Query ( "test" ); }, times );

			THEN ( "cached is been incrementend times times" )
			{
				REQUIRE ( b.cache == times );
			}

		}

		WHEN ( "concurrently setting component" )
		{
			Call_Multiple ( [&] () { b.Configure ( arg(test) = 1 ); }, times );

			THEN ( "cached is been incrementend times times" )
			{
				REQUIRE ( b.cache == times );
			}

		}

	}
}
