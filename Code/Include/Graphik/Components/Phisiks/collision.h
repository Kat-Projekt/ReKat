#ifndef COLLISION_H
#define COLLISION_H

#include "primitive_colliders.h"
#include "tilemap_collider.h"

template < class C1, class C2 >
void Resolve_Collision ( C1 c1, C2 c2 ) {
	if ( c1->Is_Static ( ) && c2->Is_Static ( ) ) { return; };
	auto C = Check_Collision ( c1, c2 );
	
	// inside collision
	if ( C.triggered ) {
		if ( ! c1->Is_Trigger( ) && ! c2->Is_Trigger( ) ) { // reaction
			if ( ! c1->Is_Static ( ) ) { // first dinamic
				if ( ! c2->Is_Static ( ) ) { // both dinamic
					float M1 = c1->obj->Get_Component < Rigidbody > ()->mass;
					float M2 = c2->obj->Get_Component < Rigidbody > ()->mass;
					float M = M1 + M2;
					M1 = M1 / M;
					M2 = M2 / M;

					c1->obj->Inc_Pos ( C.exit_direction * ( 1 - M1 ) );
					c2->obj->Inc_Pos ( - C.exit_direction * ( 1 - M2 ) );

					// vincolar reaction
					if ( C.exit_direction == vec3{0,0,0} ) { return; }
					vec3 normalize_exit = normalize(C.exit_direction);

					c1->obj->Get_Component < Rigidbody >()->Vincolar_Reaction( normalize_exit );
					c2->obj->Get_Component < Rigidbody >()->Vincolar_Reaction( -normalize_exit );
					return;
				}
				c1->obj->Inc_Pos ( C.exit_direction );
				if ( C.exit_direction == vec3{0,0,0} ) { return; }
				vec3 normalize_exit = normalize(C.exit_direction);
				c1->obj->Get_Component < Rigidbody >()->Vincolar_Reaction(normalize_exit);
			}
			if ( ! c2->Is_Static ( ) ){ // second dinamic 
				c2->obj->Inc_Pos ( -C.exit_direction );
				if ( C.exit_direction == vec3{0,0,0} ) { return; }
				vec3 normalize_exit = normalize(C.exit_direction);
				c2->obj->Get_Component < Rigidbody >()->Vincolar_Reaction(-normalize_exit);
			}

			c1->obj->Andle_Collsions ( c2->obj, false );
			c2->obj->Andle_Collsions ( c1->obj, false );
		} else {
			c1->obj->Andle_Collsions ( c2->obj, true );
			c2->obj->Andle_Collsions ( c1->obj, true );
		}
	}
}

void Resolve_Collisions ( Objekt * obj ) {
	// broad collision fase
	// // get all of the colliders
	List < Box_Collider* > BCs = obj->Get_Component_Recursive < Box_Collider > ( );
	List < Sfere_Collider* > SCs = obj->Get_Component_Recursive < Sfere_Collider > ( );
	List < Tilemap_Collider* > TCs = obj->Get_Component_Recursive < Tilemap_Collider > ( );
	// // get their type : STATIC / DINAMIC
	
	// // create a spacial hash map
	// // find the collision to calculate

	// narrow collision fase
	// // calculatate the collisions

	// collisin resolution
	// // find the best resolution for the collisions

	// Brute force
	// // run Box - Box
	for ( size_t primo = 0; primo + 1 < BCs.size(); primo ++ ) {
		Box_Collider * _cp = BCs[primo];
		if ( ! _cp->obj->Get_Active ( ) ) { continue; }
		// ciclo per le coppie non viste e diverse dall'identità
		for ( size_t secondo = 1 + primo; secondo < BCs.size(); secondo ++ ) {
			Box_Collider * _cs = BCs[secondo];
			if ( ! _cs->obj->Get_Active ( ) ) { continue; }

			Resolve_Collision ( _cp, _cs );
		}
	}

	//std::cout << "Box-Box computed\n";

	// // run Sfere - Sfere
	//std::cout << SCs.size() << '\n';
	for ( size_t primo = 0; primo + 1< SCs.size(); primo ++ ) {
		//std::cout << "coked\n";
		Sfere_Collider * _cp = SCs[primo];
		if ( ! _cp->obj->Get_Active ( ) ) { continue; }
		// ciclo per le coppie non viste e diverse dall'identità
		for ( size_t secondo = 1 + primo; secondo < SCs.size(); secondo ++ ) {
			Sfere_Collider * _cs = SCs[secondo];
			if ( ! _cs->obj->Get_Active ( ) ) { continue; }

			Resolve_Collision ( _cp, _cs );
		}
	}

	//std::cout << "Sfere-Sfere computed\n";

	// // run Box - Sfere
	for ( size_t primo = 0; primo < BCs.size(); primo ++ ) {
		Box_Collider * _cp = BCs[primo];
		if ( ! _cp->obj->Get_Active ( ) ) { continue; }
		// ciclo per le coppie non viste e diverse dall'identità
		for ( size_t secondo = 0; secondo < SCs.size(); secondo ++ ) {
			Sfere_Collider * _cs = SCs[secondo];

			//std::cout << "collision: " << _cp->obj->Get_Name ( ) << " & " << _cs->obj->Get_Name ( ) << '\n';
			if ( ! _cs->obj->Get_Active ( ) ) { continue; }
			
			Resolve_Collision ( _cp, _cs );
		}
	}

	//std::cout << "Box-Sfere computed\n";

	for ( size_t primo = 0; primo < TCs.size(); primo ++ ) {
		Tilemap_Collider * _cp = TCs[primo];
		if ( ! _cp->obj->Get_Active ( ) ) { continue; }
		// ciclo per le coppie non viste e diverse dall'identità
		for ( size_t secondo = 0; secondo < BCs.size(); secondo ++ ) {
			Box_Collider * _cs = BCs[secondo];
			//std::cout << "collision: " << _cp->obj->Get_Name ( ) << " & " << _cs->obj->Get_Name ( ) << '\n';
			if ( ! _cs->obj->Get_Active ( ) ) { continue; }
			
			Resolve_Collision ( _cp, _cs );
			//std::cout << "- resolved " << _cp->obj->Get_Name ( ) << " & " << _cs->obj->Get_Name ( ) << "\n\n";
		}
	}

}

#endif