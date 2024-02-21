#ifndef _Wall
#define _Wall

#include "Spell.h"

class Single_Wall : public Behaviour {
	float _end;
	float _emilife;
public:
	void Update ( ) {
		if ( Timer::Get_Time () > _end ) {
			obj->Get_Component < Sprite > () -> Set_Active ( true );
			obj->Get_Component < Box_Collider > () -> Set_Size ( 100 ) -> Set_Active ( true );
		}

		if ( Timer::Get_Time () > _emilife ) { obj->Set_Active ( false ); }
	}

	void Set ( float end, float emilife ) 
	{ _end = end; _emilife = emilife; }
};

class Wall : public Spell {
public:
	int number = 5;
	float time_to_wait = 0.2;
	float wall_life = 1;

	void Spawn ( std::string name, vec3 pos, int frame, float end ) {
		Objekt* Wall_o = new Objekt ( name, pos, {100,100,100} );
		Wall_o->Add_Component < Single_Wall > ( ) 
			-> Set ( end, end + wall_life );
		Wall_o->Add_Component < Box_Collider > ( ) 
			-> Set_Size ( obj->Get_Size() ) -> Set_Active ( false );
		Wall_o->Add_Component < Sprite > ( ) 
			-> Set ( Manager::Texture_Get ( "wall" ), Manager::Shader_Get ( "sprite" ), cam, {2,1}, frame )
			-> Set_Active ( false );
		obj->Add_Child ( Wall_o );
	}

    void Start ( ) {
		// rasterize position
		vec3 new_target;
		new_target.x = ( target.x > 0 ? (int) target.x : (int)target.x + 1 );
		new_target.y = ( target.y > 0 ? (int) target.y : (int)target.y + 1 );
		new_target.z = ( target.z > 0 ? (int) target.z : (int)target.z + 1 );
		obj->Set_Pos ( new_target );
		// spawn central
		float start = Timer::Get_Time ( );
		Spawn ( "Wall:C", {0,0,0}, 0, start );
		// creating the walls
		for ( size_t i = 1; i < number; i++ ) {
			Spawn ( "Wall:L" + std::to_string (i), vec3 {100,0,0} * (float)i, 1, start + time_to_wait * i );
			Spawn ( "Wall:R" + std::to_string (i), vec3 {-100,0,0} * (float)i, 1, start + time_to_wait * i );
		}
	}
	
    void Collision_Trigger ( Objekt* _obj ) { _obj; }
    void Collision ( Objekt* _obj ) { _obj; }
};

#endif