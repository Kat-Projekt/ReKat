#pragma once
#include "collision_narrower.hpp"

class Brute_Force : public Collision_Narrower {
public:
	List < collision_check > Get_Collisions_To_Check
	( ) {
		List < collision_check > cheks;
		DEBUG ( 4, "BRUTE FORCING", _colliders );

		for ( auto collider1 : _colliders )
		{
			// have at least one movable collider
			if ( collider1->Is_Static ( ) ) { continue; }
			for ( auto collider2 : _colliders ) {

				if ( collider1 == collider2 ) { continue; }
				cheks.append ( {collider2,collider1} );
			}
		}

		return cheks;
	}
};