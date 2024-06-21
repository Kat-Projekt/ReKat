#ifndef _Enemy
#define _Enemy

#include "../Spells/TheWeel.h"
#include "../Weapons/Weaponer.h"

class Enemy : public Behaviour {
	float life = 3;
public:
    virtual void Start ( ) { }
    virtual void Update ( ) { }
	virtual void Fixed_Update ( ) { }
	virtual void UI_Update ( ) { }
	virtual void Death ( ) { obj->Set_Active ( false ); }

    virtual void Collision ( Objekt* _obj ) { 
		if ( _obj->Get_Name ( ) != "spell" && _obj->Get_Name ( ) != "weapon") { return; }
		life -= 1;
		if ( life <= 0 ) { obj->Set_Active ( false ); }
	}
    virtual void Collision_Trigger ( Objekt* _obj ) {
		if ( _obj->Get_Name ( ) != "spell" && _obj->Get_Name ( ) != "weapon") { return; }
		life -= 1;
		if ( life <= 0 ) { Death ( ); }
	}
};

#endif