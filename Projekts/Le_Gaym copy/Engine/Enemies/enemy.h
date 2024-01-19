#pragma once
#include "../global.h"
#include "exact_folower.h"
#include "gobelino.h"
using namespace ReKat::grapik::Input;

class Enemy : public Behaviour {
private:
	float time;
	float between = 1;
	int count = 0;
public:
    void Start ( ) { time = 0; }

    void Update ( ) {
		time += Timer::delta_time;
		if ( time > between ) { 
			time = 0; count++;
			//std::cout << "spawn gobelino " << count << '\n';
			Manager::Object_Load ( "Gob" + std::to_string (count), "gobelino", {0,0}, {100,100} )->Add_component<Gobelino>();
			Manager::Add_Collider ("Gob" + std::to_string (count)) ->Set_size ( { 100,100 } );
			Manager::Object_Get ("Gob" + std::to_string (count))->Start();
			Manager::Object_Get ( name )->Add_Sub_Object ( "Gob" + std::to_string (count), Manager::Object_Get("Gob" + std::to_string (count)) );
			//std::cout << "end gobelino " << count << '\n';
		}
	}
};