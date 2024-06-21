#ifndef _Dart
#define _Dart

#include "Spell.h"

class Dart : public Spell {
	float speed = 500.0f;
	float life = 1.0f;
public:
    virtual void Start ( ) {
		obj->Add_Component < Box_Collider > ( )->Set_Trigger(true);
		obj->Add_Component < Rigidbody > ( );
		obj->Add_Component < Sprite > ( )->
		Set ( Manager::Texture_Get( "dart" ), Manager::Shader_Get( "sprite" ), cam, {2,1} );

		vec3 start_pos = obj->Get_Pos();
		vec3 dir = normalize ( target - start_pos );
		float a = angle ( {dir.x, dir.y} );
		obj->Set_2D_Rot ( a );
		obj->Get_Component < Rigidbody > ( )->velocity = dir * speed;

		life += Timer::current_time;
	}

	void Update ( ) 
	{ if ( Timer::current_time > life ) { obj->Set_Active ( false ); } }
};

#endif