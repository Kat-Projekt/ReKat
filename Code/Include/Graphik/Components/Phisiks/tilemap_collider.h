#ifndef TILEMAP_COLLIDER_H
#define TILEMAP_COLLIDER_H

#include "primitive_colliders.h"
#include <vector>
#include <fstream>
#include <sstream>

class Tilemap_Collider : public Collider {
private:
	std::string _path;
	int W = 0, H = 0;
	float WW = 0, HH = 0;
	std::vector < int > D;

public:
	void Start ( ) {
        std::vector < std::string > Data;
        std::ifstream Data_stream;
        // ensure ifstream objects can throw exceptions:
        Data_stream.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try {
            Data_stream.open ( _path );
            std::string intermediate;
            std::stringstream sData_stream;
            sData_stream << Data_stream.rdbuf();
            Data_stream.close ();
            // Tokenizing 
            H = 0;
            while ( getline ( sData_stream, intermediate, '\n' ) ) { H++; Data.push_back(intermediate); }
        }
        catch(const std::exception& e) { std::cerr << e.what() << '\n'; }
        
		D.reserve ( Data.size( ) );
        for ( std::string s : Data ) {
            std::string intermediate;
            std::stringstream sData_stream;
            sData_stream << s;
            // Tokenizing 
            W = 0;
            while ( getline ( sData_stream, intermediate, ',' ) ) { W++; D.push_back( std::stoi(intermediate) >= 0 ); }
        }

		HH = H * obj->Get_Size().y;
		WW = W * obj->Get_Size().x;
		std::cout << D << '\n' << HH << '\t' << WW;
	}

	void Set ( std::string path ) { _path = path; }
	vec3 Get_Space ( vec3 p ) {
		p -= obj->Get_Pos();
		vec3 s = obj->Get_Size();
		return {int(p.x / s.x), int(p.y / s.y), int(p.z / s.z)};
	}
	vec3 Ret_Space ( vec3 p ) {
		vec3 s = obj->Get_Size();
		p += obj->Get_Pos ( );
		p += vec3 { p.x * s.x, p.y * s.y, p.z * s.z };
		return p;
	}
	bool In_Space ( vec3 p ) {
		if ( ( p.x >= 0 && p.y >= 0 ) && ( p.x < WW && p.y < HH ) ) { return true; }
		return false;
	}
	/*List < Box_Collider > Get_Colliders ( vec3 Top_corner, vec3 Bot_corner ) {
		if ( ! ( In_Space ( Top_corner ) && In_Space ( Bot_corner ) ) ) { return List < Box_Collider > ( ); }
		// get clamped space Top_corner
		Top_corner = Get_Space ( Top_corner );
		// get clamped space Bot_corner
		Bot_corner = Get_Space ( Bot_corner );
		if ( Top_corner == Bot_corner ) {
			Box_Collider B;
		}
	}*/

	Collision_Result Check_Collision ( Box_Collider * B ) {
		std::cout << "- check collision " << obj->Get_Name ( ) << " & " << B->obj->Get_Name ( ) << '\n';
		// gets the position of B on the grid ( if on bound prosed )
    	vec3 Half_Size = B->Get_Size ( ) * 0.5f;
    	vec3 Center = B->obj->Get_Pos ( );

		vec3 Su_Destra = Center + Half_Size;
		vec3 Giu_Sinistra = Center - Half_Size;

		std::cout << "- - " << Center << '\n';

		if ( ! ( In_Space ( Su_Destra ) && In_Space ( Giu_Sinistra ) ) ) { return {false, {0,0,0}}; }
		std::cout << "- Obj in space\n";

		// check for Giu_Sinistra
		vec3 Giu_Sinistra_C = Get_Space ( Giu_Sinistra );
		std::cout << Giu_Sinistra_C << '\n';
		if ( D [ Giu_Sinistra_C.x + Giu_Sinistra_C.y * W ] ) {
			vec3 move_vector;
			vec3 Delta = Ret_Space ( Giu_Sinistra );

			// determine exit point
			if ( abs ( Delta.x ) > abs ( Delta.y ) ) {
				move_vector = { 0, Delta.y, 0 };
			} else {
				move_vector = { Delta.x, 0, 0 };
			}

			return {true,move_vector};
		}
		
		// check for Su_Destra
		vec3 Su_Destra_C = Get_Space ( Su_Destra );
		std::cout << Su_Destra_C << '\n';
		if ( D [ Su_Destra_C.x + Su_Destra_C.y * W ] ) { return { true, ( Giu_Sinistra - Ret_Space ( Giu_Sinistra_C) ) }; }
	}
	Collision_Result Check_Collision ( Sfere_Collider * S ) {
		return { false, {0,0,0} };
	}
};

template < typename C >
Collision_Result Check_Collision ( Tilemap_Collider *T, C B ) 
{ return T->Check_Collision ( B ); }
#endif