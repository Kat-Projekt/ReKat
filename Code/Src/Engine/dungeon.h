#pragma once
#include "dungeon_special.h"

class Mappa {
public:
	struct vec2 { int x; int y; };

	int seed;
	vec2 size;
	void fill ( );
	std::vector < int > rooms;

	void fill_room ( vec2 pos, int val ) {
		rooms [ pos.x * size.y + pos.y] = val;
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

int abs ( int a )
{ return ( a > 0 ? a : -a );}
int sing ( int a )
{ return ( a < 0 ? -1 : 1 );}

void Mappa::fill ( ) {
	//std::cout << "stating to fill\n\n";
	srand(this->seed);

	// genera caverna
	// prendi dei punti
	// crea delle linee tra due punti consecutivi e traccia la linea nella matrice
	// allarga le carverne
	// se una casella ha vicino n scrivi n+1
	// in base alla distanza aggiungi layers
	std::vector < vec2 > points = {{0,0}, {3,7}, {7,3}};
	/*for (size_t i = 0; i < 3; i++) {
		points.push_back ( { rand()%(size.x), rand()%(size.y) } );
		 code 
	}*/

	for ( size_t i = 0; i < points.size() - 1; i++ ) {
  		int X = abs(points [i+1].x - points [i].x), X_s = sing (points [i+1].x - points [i].x);
  		int Y = abs(points [i+1].y - points [i].y), Y_s = sing (points [i+1].y - points [i].y);
		if ( X > Y ) {
			float M = (float)(points[i+1].y - points[i].y) / (float)(points[i+1].y - points[i].y);// delta y / delta x
			for ( int x = 0; x < X; x++) {
				// draw the line
				std::cout << "e ";
				fill_room ( { points[i].x + x * X_s, points[i].y }, 1);
				/* code */
			}
		} else {
			for ( int y = 0; y < Y; y++) {
				// draw the line
				std::cout << "e ";
				fill_room ( { points[i].x, points[i].y + y * Y_s }, 1);
				/* code */
			}
		}
  
		
		
		/* code */
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
        mappa = Mappa ( 12345, {10,10} );
        std::cout << mappa;
		for (size_t i = 0; i < 10; i++) {
			for (size_t y = 0; y < 10; y++) {
				glm::vec4 color = {i/10.0,y/10.0,y/10.0,1};
				if ( mappa.rooms[i*mappa.size.y+y] == 0 ) { color = glm::vec4(1,1,1,1); }
				std::string n = "Tile:" + std::to_string(i) + ":" + std::to_string(y);
				Manager::Object_Load( n, "empty_sprite",{100*y,100*i},{100,100})->Set_Color(color);
				Manager::Object_Get(name)->Add_Sub_Object(n ,Manager::Object_Get(n));
			}
		}
    }
};