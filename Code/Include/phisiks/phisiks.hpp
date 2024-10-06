#ifndef PHISIKS_H
#define PHISIKS_H

#include "../objekt.hpp"
#include "components/collision.h"

#include "timer.hpp"

// take care of the collisions and phisik update
class Phisiks : public Behaviour {
private:
	// phisik update
	float _t;
	float _delta = 1.0f/120.0f;
public:
	void Start ( ) 
	{ _t = Timer::Get_Time ( ) + _delta; }

	void Update ( ) {
		Timer::Update ( );
		if ( _t < Timer::Get_Time ( ) ) 
		{ _t = Timer::Get_Time() + _delta; _Fixed_Update ( );}
	}

	// every fixed update check for collisions and update phisik
	void _Fixed_Update ( ) {
		Timer::Fixed_Update ( );
		// calling for childrens
		obj->Fixed_Update ( );

		Resolve_Collisions ( obj );
	}
};

#endif
