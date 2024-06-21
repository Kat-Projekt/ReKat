#ifndef _Master_Mind
#define _Master_Mind

#include "Gobelino.h"

class Master_Mind : public Behaviour {
	float delay = 4;
	float spawn;
public:
	Objekt * player;
	Camera * cam;
    virtual void Start ( ) 
	{ spawn = Timer::current_time + delay; }
    virtual void Update ( ) {
		if ( Timer::current_time < spawn ) { return; }
		auto enemy = new Objekt ( "gobelino" );
		enemy->Add_Component < Gobelino > ( );
		enemy->Add_Component < Sprite > ( );
		enemy->Add_Component < Rigidbody > ( );
		enemy->Add_Component < Sfere_Collider > ( ) -> Set_Size ( 60 );
		enemy->Get_Component < Sprite > ( ) -> Set 
		( Manager::Texture_Get ( "gobelino" ), Manager::Shader_Get ( "sprite" ), cam );
		enemy->Get_Component < Gobelino > ( ) -> target = player;
		enemy->Get_Component < Gobelino > ( ) -> cam = cam;

		obj->Add_Child ( enemy );
		spawn += delay;
	}
	virtual void Fixed_Update ( ) { }
	virtual void UI_Update ( ) { }

    virtual void Collision ( Objekt* _obj ) { _obj; }
    virtual void Collision_Trigger ( Objekt* _obj ) { _obj; }
};

#endif