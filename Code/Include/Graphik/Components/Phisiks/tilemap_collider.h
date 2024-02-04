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
            while ( getline ( sData_stream, intermediate, ',' ) ) { W++; D.push_back( std::stoi(intermediate) > 0 ); }
        }

		HH = H * obj->Get_Size().y;
		WW = W * obj->Get_Size().x;
		std::cout << D << '\n' << HH << '\t' << WW;
	}

	void Set ( std::string path ) { _path = path; }

	Collision_Result Check_Collision ( Box_Collider * B ) {
		vec2 rec_scale;
		vec2 half_size = B->Get_Size ( ) * 0.5f;
		vec2 pos = B->obj->Get_Pos ( );
		vec2 X_limit = { pos.x - half_size.x, pos.x + half_size.x };
		vec2 Y_limit = { pos.y - half_size.y, pos.y + half_size.y };

		// rasterize the collider
		vec2 _pos = obj->Get_Pos();
		vec2 _size = obj->Get_Size();
		float _Min_X = ( (X_limit.x - _pos.x) / _size.x + 0.5f ), _Max_X = ( (X_limit.y - _pos.x) / _size.x + 0.5f );
		float _Min_Y = ( (_pos.y - Y_limit.y) / _size.y + 0.5f ), _Max_Y = ( (_pos.y - Y_limit.x) / _size.y + 0.5f );

		// Min_ < Max_
		int Min_X = ( _Min_X >= 0 ? (int)_Min_X : (int) (_Min_X-1.0f));
		int Max_X = ( _Max_X >= 0 ? (int)_Max_X : (int) (_Max_X-1.0f));
		int Min_Y = ( _Min_Y >= 0 ? (int)_Min_Y : (int) (_Min_Y-1.0f));
		int Max_Y = ( _Max_Y >= 0 ? (int)_Max_Y : (int) (_Max_Y-1.0f));

		// work on rasterized collider
		// // everyting out
		if ( ( Max_X < 0 && Min_X < 0 ) || ( Max_Y < 0 && Min_Y < 0 ) ) { return { false, {0,0,0} }; }
		if ( ( Max_X >= W && Min_X >= W ) || ( Max_Y >= H && Min_Y >= H ) ) { return { false, {0,0,0} }; }

		// clamp
		Min_X < 0 ? Min_X = 0 : 0;
		Min_Y < 0 ? Min_Y = 0 : 0;
		Max_X >= W ? Max_X = W - 1 : 0;
		Max_Y >= H ? Max_Y = H - 1 : 0;

		// tile to check
		vec3 out_vector = {0,0,0};
		for (size_t x = Min_X; x <= Max_X; x++) {
			for (size_t y = Min_Y; y <= Max_Y; y++) {
				if ( D[x + y * W] != 1 ) { continue; }
				vec2 tile_pos = obj->Get_Pos() + vec3{ obj->Get_Size().x * x, -obj->Get_Size().y * y, 0 };
				vec2 P_Delta = tile_pos - (vec2)B->obj->Get_Pos();
				vec2 Delta = abs(P_Delta) - (vec2)(obj->Get_Size() + B->Get_Size()) * 0.5f;

				// collision not appeing
				if ( Delta.x > 0 || Delta.y > 0 ) { continue; }

				// trigger collision ( no nead for fastest exit point )
				if ( B->Is_Trigger() ) { return { true,{0,0,0} }; }

				// determine exit point
				if (abs(Delta.x) > abs(Delta.y)) {// Delta.y is Low
					float Sing = (P_Delta.y >= 0 ? -1 : 1);
					float move_vector = Delta.y * Sing;
					if ( abs ( out_vector.y ) < abs ( move_vector ) ) { out_vector.y = move_vector; }
				}
				else { // Delta.x is Max
					float Sing = (P_Delta.x >= 0 ? -1 : 1);
					float move_vector = Delta.x * Sing;
					if ( abs ( out_vector.x ) < abs ( move_vector ) ) { out_vector.x = move_vector; }
				}
			}
		}

		return { out_vector != vec3{0,0,0}, out_vector };
	}
	Collision_Result Check_Collision ( Sfere_Collider * S ) {
		return { false, {0,0,0} };
	}
};

template < typename C >
Collision_Result Check_Collision ( Tilemap_Collider *T, C B ) 
{ return T->Check_Collision ( B ); }
#endif