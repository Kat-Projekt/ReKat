#ifndef _Weapon
#define _Weapon

#include <Graphik/objekt.h>
#include <Graphik/graphik.hpp>
#include <Graphik/Components/_catalog.hpp>
using namespace ReKat::grapik::Input;

class Weapon : public Behaviour {
	float _attack_progression = 1;
	float _start_angle;
protected:
	float damage = 1;
	float attack_spread = 1.5;
	float attack_duration = 0.20;
	int durability = 100;
public:
	virtual void Start ( ) 
	{ attack_duration = 1 / attack_duration; }

	virtual void Start_Attack ( float angle ) {
		if ( durability < 1 ) { return; }
		_start_angle = angle + attack_spread * 0.5f;
		if ( _attack_progression >= 1 ) 
		{ _attack_progression = 0; }
		obj->Set_Active ( true );
	}

	virtual void Fixed_Update ( ) {
		_attack_progression += Timer::fixed_delta_time * attack_duration;;
		// end of attack disable weapon
		if ( _attack_progression >= 1 ) { obj->Set_Active ( false ); return; }

		obj->Set_2D_Rot ( _start_angle - ( attack_spread * _attack_progression ) );
	}

    virtual void Collision ( Objekt* _obj ) {
		if ( _obj->Get_Name ( ) == "Player" || _obj->Get_Name ( ) == "speel" ) { return; }
		_attack_progression = 1;
		durability --;
	}
    virtual void Collision_Trigger ( Objekt* _obj ) {
		if ( _obj->Get_Name ( ) == "Player" || _obj->Get_Name ( ) == "speel" ) { return; }
		durability --; _attack_progression = 1; 
	}
};

#endif