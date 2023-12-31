#pragma once
#include "dungeon_special.h"

inline int abs ( int a )
{ return ( a > 0 ? a : -a );}
inline int sing ( int a )
{ return ( a < 0 ? -1 : 1 );}

class Mappa {
public:
	struct vec2 { int x; int y; };

	int seed;
	vec2 size;
	void fill ( );
	std::vector < int > rooms;

	void fill_room ( vec2 pos, int val ) { 
		rooms [ clamp(pos.x, 0, size.x-1) * size.y + clamp(pos.y, 0, size.y-1)] = val; 
	}
	int get_room  ( vec2 pos ) { 
		return rooms [ clamp(pos.x, 0, size.x-1) * size.y + clamp(pos.y, 0, size.y-1)]; 
	}

	Mappa ( ) { }
	Mappa ( int seed, vec2 size ) {
		rooms.resize(size.x*size.y);
		this->seed = seed;
		this->size = size;
		this->fill ( );
	}
	~Mappa ( ) { rooms.clear(); }
};


void Mappa::fill ( ) {
	//std::cout << "stating to fill\n\n";
	srand(this->seed);

	// genera caverna
	// prendi dei punti
	// crea delle linee tra due punti consecutivi e traccia la linea nella matrice
	// allarga le carverne
	// se una casella ha vicino n scrivi n+1
	// in base alla distanza aggiungi layers
	std::vector < vec2 > points = { {7,3}, {13,17}, {27,23}, {33,37}, {47,13} };
	/*for (size_t i = 0; i < 3; i++) {
		points.push_back ( { rand()%(size.x), rand()%(size.y) } );
		 code 
	}*/

	for ( size_t i = 0; i < points.size() - 1; i++ ) {
  		int X = abs(points [i+1].x - points [i].x), X_s = sing (points [i+1].x - points [i].x);
  		int Y = abs(points [i+1].y - points [i].y), Y_s = sing (points [i+1].y - points [i].y);
		if ( X > Y ) {
			float M = (float)(Y) / (float)(X);
			for ( int x = 0; x < X; x++) 
			{ fill_room ( { points[i].x + x * X_s, points[i].y + (int)(M * x) }, 1); }
		} else {
			float M = (float)(X) / (float)(Y);
			for ( int y = 0; y < Y; y++) 
			{ fill_room ( { points[i].x + (int)(M * y), points[i].y + y * Y_s }, 1); }
		}
	}

	// enlarge dungeon
	for (size_t iteration = 1; iteration < 4; iteration++) {
		for ( int x = 0; x < size.x; x++ ) {
			for (int y = 0; y < size.y; y++) {
				if ( get_room( { x-1, y} ) == iteration || get_room( { x+1, y} ) == iteration || 
					 get_room( { x, y-1} ) == iteration || get_room( { x, y+1} ) == iteration ) 
				{ if ( get_room ( { x, y } ) == 0) { fill_room ( {x,y}, iteration + 1 ); } }
			}
		}
	}
}

std::ostream& operator << (std::ostream& os, const Mappa& map) {
	os << "Map generated with seed: " << map.seed;
	os << "\nThis map has: " << map.size.x *map.size.y << " rooms\n";
	
	for (size_t x = 0; x < map.size.x; x++) {
		for (size_t y = 0; y < map.size.y; y++) {
			std::cout << map.rooms[x*map.size.y+y];
		}
		std::cout << '\n';
	}
	
    return os;
}

class dungeon : public Behaviour {
private:
    Mappa mappa;
public:
    void Start ( ) {
        mappa = Mappa ( 12345, {50,50} );
        std::cout << mappa;
		for (size_t i = 0; i < 50; i++) {
			for (size_t y = 0; y < 50; y++) {
				glm::vec4 color = {mappa.rooms[i*mappa.size.y+y]/4,0,0,1};
				if ( mappa.rooms[i*mappa.size.y+y] == 0 ) { continue; }
				std::string n = "Tile:" + std::to_string(i) + ":" + std::to_string(y);
				Manager::Object_Load( n, "empty_sprite",{100*y,100*i},{100,100})->Set_Color(color);
				if ( mappa.rooms[i*mappa.size.y+y] == 4 )
				{ Manager::Add_Collider ( n )->Set_size({100,100})->Set_movable(false); std::cout << "col"; }
				Manager::Object_Get(name)->Add_Sub_Object(n ,Manager::Object_Get(n));
			}
		}
    }
};