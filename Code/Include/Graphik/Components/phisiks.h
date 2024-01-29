#ifndef PHISIKS_H
#define PHISIKS_H

#include "../objekt.h"
#include "../graphik.hpp"
#include "collision.h"

// take care of the collisions and phisik update
class Phisiks : public Behaviour {
private:
	// phisik update
	float _t;
	float _delta = 1.0f/50.0f;
public:
	void Start ( ) 
	{ _t = Timer::Get_Time ( ) + _delta; }

	void Update ( ) {
		if ( _t < Timer::Get_Time ( ) ) 
		{ _t = Timer::Get_Time() + _delta;  } _Fixed_Update ( );
	}

	// every fixed update check for collisions and update phisik
	void _Fixed_Update ( ) {
		Timer::Fixed_Update ( );
		// calling for childrens
		obj->Fixed_Update ( );

		// broad collision fase
		// // get all of the colliders
		List < Box_Collider* > BCs = obj->Get_component_recursive < Box_Collider > ( );
		List < Sfere_Collider* > SCs = obj->Get_component_recursive < Sfere_Collider > ( );
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

				// std::cout << "collision: " << _cp->obj->Get_Name ( ) << " & " << _cs->obj->Get_Name ( ) << '\n';
				if ( ! _cs->obj->Get_Active ( ) ) { continue; }
				if ( _cp->Is_Static ( ) && _cs->Is_Static ( ) ) { continue; };
				auto C = Check_Collision ( _cp,  _cs );
				// std::cout << C << '\n';
                // std::cout << "check " << _cp->name << " & " << _cs->name << " = " << ( C==glm::vec2{0,0} ? "si" : "no" ) << '\n';

				// inside collision
				if ( C.triggered ) {
					if ( ! _cp->Is_Trigger( ) && ! _cs->Is_Trigger( ) ) {
						// std::cout << " both solid\n";
						// reaction
						if ( ! _cp->Is_Static ( ) ) { // first dinamic
							if ( ! _cs->Is_Static ( ) ) { // second dinamic
								float M1 = _cp->obj->Get_component < Rigidbody > ()->mass;
								float M2 = _cs->obj->Get_component < Rigidbody > ()->mass;

								_cp->obj->Inc_Pos ( C.exit_direction * 0.5f );
								_cs->obj->Inc_Pos ( - C.exit_direction * 0.5f );
							}
							_cp->obj->Inc_Pos ( C.exit_direction );
						}
						if ( ! _cs->Is_Static ( ) ) { _cs->obj->Inc_Pos ( -C.exit_direction ); }

						_cp->obj->Andle_Collsions ( _cs->obj, false );
						_cs->obj->Andle_Collsions ( _cp->obj, false );
					} else {
						// std::cout << " both trigger\n";
						_cp->obj->Andle_Collsions ( _cs->obj, true );
						_cs->obj->Andle_Collsions ( _cp->obj, true );
					}
				}
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

				// std::cout << "collision: " << _cp->obj->Get_Name ( ) << " & " << _cs->obj->Get_Name ( ) << '\n';
				if ( ! _cs->obj->Get_Active ( ) ) { continue; }
				if ( _cp->Is_Static ( ) && _cs->Is_Static ( ) ) { continue; };
				auto C = Check_Collision ( _cp,  _cs );
				//std::cout << C << '\n';
                // std::cout << "check " << _cp->name << " & " << _cs->name << " = " << ( C==glm::vec2{0,0} ? "si" : "no" ) << '\n';

				// inside collision
				if ( C.triggered ) {
					if ( ! _cp->Is_Trigger( ) && ! _cs->Is_Trigger( ) ) {
						//std::cout << " both solid\n";
						// reaction
						if ( ! _cp->Is_Static ( ) ) { // first dinamic
							if ( ! _cs->Is_Static ( ) ) { // second dinamic
								float M1 = _cp->obj->Get_component < Rigidbody > ()->mass;
								float M2 = _cs->obj->Get_component < Rigidbody > ()->mass;

								float M = 1 / ( M1 + M2 );

								_cp->obj->Inc_Pos ( C.exit_direction * M1 * M );
								_cs->obj->Inc_Pos ( - C.exit_direction * M2 * M );
							}
							_cp->obj->Inc_Pos ( C.exit_direction );
						}
						if ( ! _cs->Is_Static ( ) ) { _cs->obj->Inc_Pos ( -C.exit_direction ); }

						_cp->obj->Andle_Collsions ( _cs->obj, false );
						_cs->obj->Andle_Collsions ( _cp->obj, false );
					} else {
						//std::cout << " both trigger\n";
						_cp->obj->Andle_Collsions ( _cs->obj, true );
						_cs->obj->Andle_Collsions ( _cp->obj, true );
					}
				}
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
				if ( _cp->Is_Static ( ) && _cs->Is_Static ( ) ) { continue; };
				auto C = Check_Collision ( _cp,  _cs );
				//std::cout << C << '\n';
                // std::cout << "check " << _cp->name << " & " << _cs->name << " = " << ( C==glm::vec2{0,0} ? "si" : "no" ) << '\n';

				// inside collision
				if ( C.triggered ) {
					if ( ! _cp->Is_Trigger( ) && ! _cs->Is_Trigger( ) ) {
						//std::cout << " both solid\n";
						// reaction
						if ( ! _cp->Is_Static ( ) ) { // first dinamic
							if ( ! _cs->Is_Static ( ) ) { // second dinamic
								float M1 = _cp->obj->Get_component < Rigidbody > ()->mass;
								float M2 = _cs->obj->Get_component < Rigidbody > ()->mass;

								float M = 1 / ( M1 + M2 );

								_cp->obj->Inc_Pos ( - C.exit_direction * M1 * M );
								_cs->obj->Inc_Pos ( C.exit_direction * M2 * M );
							}
							_cp->obj->Inc_Pos ( - C.exit_direction );
						}
						if ( ! _cs->Is_Static ( ) ) { _cs->obj->Inc_Pos ( C.exit_direction ); }

						_cp->obj->Andle_Collsions ( _cs->obj, false );
						_cs->obj->Andle_Collsions ( _cp->obj, false );
					} else {
						//std::cout << " both trigger\n";
						_cp->obj->Andle_Collsions ( _cs->obj, true );
						_cs->obj->Andle_Collsions ( _cp->obj, true );
					}
				}
            }
        }
	
		//std::cout << "Box-Sfere computed\n";
	}
};

#endif
