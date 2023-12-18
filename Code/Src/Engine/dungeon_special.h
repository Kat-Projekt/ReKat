#pragma once
#include "../global.h"

#include <vector>
#include <iostream>
#include <stdlib.h>

// camps n tra a e b
int clamp ( int n, int a, int b ) {
	if ( n > b ) { n = b; }
	if ( n < a ) { n = a; }
	return n;
}

enum dir {
	NORD,
	SUD,
	EST,
	OVEST
};

dir invert ( dir direzione ) {
	if		( direzione == NORD ) { return   SUD; }
	else if ( direzione ==  SUD ) { return  NORD; }
	else if ( direzione ==  EST ) { return OVEST; }
	else 						  { return   EST; }
}

struct room {
	// borders
	int nord = -1;
	int sud = -1;
	int est = -1;
	int ovest = -1;

	// returns 4 if empty
	int count ( ) {
		int c = 0;
		if ( nord  == -1 ) { c++; }
		if ( sud   == -1 ) { c++; }
		if ( est   == -1 ) { c++; }
		if ( ovest == -1 ) { c++; }
		return c;
	}

	int get_dir ( dir direzione ) {
		if ( direzione == NORD ) { return nord; }
		else if ( direzione == SUD ) { return sud; }
		else if ( direzione == EST ) { return est; }
		else  { return ovest; }
	}

	bool is_free ( dir direzione ) {
		if ( direzione == NORD ) 
		{ if ( nord == -1 )  { return true; } else { return false; } }
		else if ( direzione == SUD ) 
		{ if ( sud == -1 )   { return true; } else { return false; } }
		else if ( direzione == EST ) 
		{ if ( est == -1 )   { return true; } else { return false; } }
		else  
		{ if ( ovest == -1 ) { return true; } else { return false; } }
	}

	dir get_next_dir ( int next ) {
		int dirs = 0;
		dir next_dir;
		room b = *this;

		// loop in the stanzas untili next pass
		while ( dirs != next + 1 ) {
			if ( b.nord  == -1 ) { next_dir  = NORD; b.nord  = 0; dirs++; continue; }
			if ( b.sud   == -1 ) { next_dir   = SUD; b.sud   = 0; dirs++; continue; }
			if ( b.est   == -1 ) { next_dir   = EST; b.est   = 0; dirs++; continue; }
			if ( b.ovest == -1 ) { next_dir = OVEST; b.ovest = 0; dirs++; continue; }
		}

		return next_dir;
	}

	void set_dir ( dir direzione, int index ) {
		if ( direzione == NORD )  { nord  = index; }
		if ( direzione == SUD )   { sud   = index; }
		if ( direzione == EST )   { est   = index; }
		if ( direzione == OVEST ) { ovest = index; }
	}
};

bool all_rooms_filled ( std::vector < room > & rooms ){
	// verifiy if evry room has at least 2 bridges
	for ( room r : rooms ) {
		if ( r.count() >= 3 ) { return false; } 
	}

	// check if ath least one room has 4 brides
	for ( room r : rooms ) {
		if ( r.count() == 0 ) { return true; } 
	}

	return false;
}

std::ostream& operator << (std::ostream& os, const std::vector < room > & map ) {

	for (size_t i = 0; i < map.size(); i++) {
		room r = map[i];
		os << "\n\nRoom n: " << i;

		if ( r.nord  != -1 ) { os << "\nNord:  " << r.nord ; }
		if ( r.sud   != -1 ) { os << "\nSud:   " << r.sud  ; }
		if ( r.est   != -1 ) { os << "\nEst:   " << r.est  ; }
		if ( r.ovest != -1 ) { os << "\nOvest: " << r.ovest; }
	}

    return os;
}



class Map {
public:
	int seed;
	int n_rooms;
	void fill ( ) {
		//std::cout << "stating to fill\n\n";
		srand(this->seed);
		int current_room = 0;

		while ( ! all_rooms_filled ( rooms ) ) {
			// scegli una direzione disponibile
			// // se la stanza non ha direzioni disponibili cambia
			if ( rooms[current_room].count() == 0 ) {
				current_room = rooms[current_room].get_dir( (dir)( rand()%4 ) );
				continue;
			}

			int next_dir_v = rand() % rooms[current_room].count();
			dir next_dir = invert( rooms[current_room].get_next_dir ( next_dir_v ) ); 
			// "prendi una stanza casuale\n";
			
			int next_room = rand() % n_rooms - 1;
			next_room >= current_room ? next_room ++ : 0;
			next_room = clamp( rand() % n_rooms - 1, 0, n_rooms -1);
			while ( !rooms[next_room].is_free ( next_dir ) ) {
				std::cout << "next_roon: " << next_room << " " << '\n' ;
				next_room = rand() % n_rooms - 1;
				next_room >= current_room ? next_room ++ : 0;
				next_room = clamp( rand() % n_rooms - 1, 0, n_rooms -1);
			}

			// linka le stanze
			rooms[next_room].set_dir(next_dir, current_room);
			rooms[current_room].set_dir(invert(next_dir), next_room);

			current_room = next_room;
			// std::cout << "next room: " << next_room << '\n';
			// std::cout << "room direction: " << next_dir << "\n\n";
		}
	}
	std::vector < room > rooms;

    Map ( ) { }
	Map ( int seed, int n_rooms = 4 ) {
		rooms.resize ( n_rooms );
		this->seed = seed;
		this->n_rooms = n_rooms;
		this->fill ( );
	}
	~Map ( ) { rooms.clear(); }
	
	friend std::ostream& operator << (std::ostream& os, const Map& map) {
		os << "Map generated with seed: " << map.seed;
		os << "\nThis map has: " << map.n_rooms << " rooms";
		os << map.rooms;
		return os;
	}
};



class dungeon_special : public Behaviour {
private:
    Map mappa;
public:
    void Start ( ) {
        mappa = Map ( 12345, 10 );
        std::cout << mappa;
		for (size_t i = 0; i < 10; i++) {
			for (size_t y = 0; y < 10; y++) {
				std::string n = "Tile:" + std::to_string(i) + ":" + std::to_string(y);
				Manager::Object_Load( n, "empty_sprite",{100*i,100*y},{100,100})->Set_Color({i/10.0,y/10.0,y/10.0,1});
				Manager::Object_Get(name)->Add_Sub_Object(n ,Manager::Object_Get(n));
			}
		}
    }
};