#pragma once
#include "com.h"
using namespace ReKat::grapik::Input;

class Beans : public Behaviour {
	public:
	vec3 target_pos;
	
	void Update ( ) {
		obj->Get_component<Sprite>()->frame = 0;
		obj->Set_Pos ( lerp ( obj->Get_Pos(), target_pos, Timer::delta_time *5 ) );

		obj->Set_Size ( {50,50,50} );
		auto D = Dist ( obj->Get_Pos(), target_pos );
		if ( D < 256 ) {
			obj->Set_Size ( vec3 {100,100,100} * (1 - D / 512) );
		}

		if ( D < 100 ) { obj->Get_component<Sprite>()->frame = 1; }
		if ( D < 10 ) { obj->Get_component<Sprite>()->frame = 2; }
		if ( D < 3 ) { obj->Get_component<Sprite>()->frame = 3; }
	}
};

class Lasurs : public Behaviour {
	public:
	vec3 target_pos;
	vec3 dir = {0,0,0};
	vec3 start_pos = {0,0,0};
	float progress = 0;
	float speed = 3;
	float fase = 3 * 3.1415;
	float amplitude = 20;

	void Start ( ) {
		start_pos = obj->Get_Pos();
		dir = normalize ( target_pos - start_pos );
		float a = angle ( {dir.x, dir.y} );
		std::cout << "angle: " << a << '\n';
		obj->Set_2D_Rot ( a );
	}

	void Update ( ) {
		progress += Timer::delta_time * speed;

		obj->Set_Pos ( lerp ( start_pos, target_pos, progress ) + 
		vec3 { cos ( fase * progress ) * dir.y, sin ( fase * progress ) * dir.x, 0 } * amplitude);

		if ( progress > 0.8 ) { obj->Get_component<Sprite>()->frame = 1; }
		else { obj->Get_component<Sprite>()->frame = 0; }
		if ( progress > 1 ) { obj->Set_Active( false ); }
	}
};

class Wall : public Behaviour {
public:
	int number;
	vec3 direction = {0,0,0};
	float Time_to_wait = 0.2;
	float Begin;
	bool Spawned = false;

	void Spawn ( std::string name, vec3 pos, int frame, int number, vec3 _direction ) {
		std::cout << "spawing: " << name << '\n';
		Objekt* Wall_o = new Objekt ( name, pos, {100,100,100} );
		Wall_o->Add_component < Sprite > ()->
		Set ( Manager::Texture_Get ( "Wall" ), Manager::Shader_Get ( "sprite" ), camera.Get_component < Camera > ( ), {2,1} )->frame = frame;
		auto W = Wall_o->Add_component < Wall > ( );
		W->number = number;
		W->direction = _direction;
		Wall_o->Add_component < Box_Collider > ( )->Set_Size ( obj->Get_Size() );
		Wall_o->Start();
		Scene_Manager::_current_scene->Add_Child ( Wall_o );
	}

	void Start ( ) {
		Begin = Timer::Get_Time ( ) + Time_to_wait;
		if ( number <= 0 ) { return; }
		number--;
		// first wall
		if ( direction == vec3{0,0,0} ) {
			// center wall
			Spawn ( obj->Get_Name() + ":" + std::to_string(number) + "C", obj->Get_Pos( ), 0, 0, {0,0,0} );
			// left wall
			vec3 Wall_pos = obj->Get_Pos ( ) + vec3{100,0,0};
			Spawn ( obj->Get_Name() + ":" + std::to_string(number) + "L", Wall_pos, 1, number, {100,0,0} );			
			// rigth wall
			Wall_pos = obj->Get_Pos ( ) + vec3{-100,0,0};
			Spawn ( obj->Get_Name() + ":" + std::to_string(number) + "R", Wall_pos, 1, number, {-100,0,0} );
			number = 0; return;
		}
	}

	void Update ( ) {
		if ( number <= 0 ) { return; }
		if ( Spawned ) { return; }
		if ( Timer::Get_Time ( ) > Begin ) {
			number--;
			vec3 Wall_pos = obj->Get_Pos ( ) + direction;
			Spawn ( obj->Get_Name() + ":" + std::to_string(number) + (direction.x > 0 ? "L" : "R"), Wall_pos, 1, number, direction );
			Spawned = true;
		}
	}
};

class Spells : public Behaviour {
	int speel;
	int Beanss = 0;
	int Lasurss = 0;
	int Wallss = 0;

	// speel well
	Objekt* P;
public:
	Objekt* Player;
	
	std::vector < std::string > spell_list = {"Lasur", "Beans" , "Bones", "Satan piss", "Wall"};
	int spell_count = spell_list.size();
	float spell_angle = 2 * 3.1415 / spell_count ;
    void Start ( ) {
		P = new Objekt ( "spells" );
		obj->Add_Child ( P );
		for ( size_t i = 0; i < spell_count; i++ ) {
			auto S = new Objekt ( spell_list[i], vec3 { sin( spell_angle * (i + 0.5) ), cos( spell_angle * (i + 0.5) ), 0 } * 100.0f, {100,100,100} );
			S->Add_component < Sprite > ( )->frame = i*2 + 1;
			S->Get_component < Sprite > ( ) ->
			Set ( Manager::Texture_Get( "Spells" ), Manager::Shader_Get( "sprite" ), camera.Get_component < Camera > ( ), {2,4} );

			S->Start ( );
			P->Add_Child ( S );
		}
		P->Set_Active(false);
	}
    void Update ( ) {
		// spell whell
		if ( Key_Down ( "Alt" ) ) 
		{ P->Set_Active(true); }
		if ( Key_Pressed ( "Alt" ) ) {
			int current_spell = ( angle ( mouse_pos ) / spell_angle + 1 );
			current_spell %= spell_count;
			current_spell = spell_count - current_spell;
			current_spell %= spell_count;
			for ( size_t i = 0; i < spell_count; i++ ) {
				if ( i == current_spell ) { P->Get_Children ( spell_list[i] )->Get_component < Sprite > ()->frame = i * 2; }
				else {  P->Get_Children ( spell_list[i] )->Get_component < Sprite > ()->frame = i * 2 + 1; }
			}
			// std::cout << "c: " << current_spell;
		}
		if ( Key_Up ( "Alt" ) ) {
			P->Set_Active(false);
			float a = angle ( mouse_pos );
			std::cout << "angle: " << a << '\n';
			a /= spell_angle;
			speel = a + 1;
		}

		if ( Key_Down ( "Mouse1" ) ) {
			switch ( speel ) {
			case 0: { // cast laser ray from player in mouse direction
				std::cout << "------------------------------ lasur\n";
				vec3 Lasur_target = camera.Get_Pos ( ) + vec3{mouse_pos * camera.Get_component< Camera > ()->Scale, 0};
				// std::cout << Lasur_target.x << " : " << Lasur_target.y;
				Objekt* Lasur = new Objekt ( "Lasur_" + std::to_string (Lasurss++), Player->Get_Pos(), {60,60,0}) ;
				Lasur->Add_component < Lasurs > ( )->target_pos = Lasur_target;
				Lasur->Add_component < Sfere_Collider > ( )->Set_Trigger(true);
				Lasur->Add_component < Sprite > ( )->
				Set ( Manager::Texture_Get( "Lasur" ), Manager::Shader_Get( "sprite" ), camera.Get_component < Camera > ( ), {2,1} );
				Scene_Manager::_current_scene->Add_Child ( Lasur );
				Lasur->Start();
				break; }
			
			case 1: { // spawn a can of beans
				std::cout << "oooooooooooooooooooooooooooo Beans\n";
				vec3 bean_pos = camera.Get_Pos ( ) + vec3{mouse_pos * camera.Get_component< Camera > ()->Scale, 0};
				std::cout << bean_pos.x << " : " << bean_pos.y;
				Objekt* Bean = new Objekt ( "Bean_" + std::to_string (Beanss++), Player->Get_Pos(), {100,100,100});
				Bean->Add_component < Beans > ( )->target_pos = bean_pos;
				Bean->Add_component < Sprite > ( )->
				Set ( Manager::Texture_Get( "Bean" ), Manager::Shader_Get( "sprite" ), camera.Get_component < Camera > ( ), {2,2} );
				Bean->Start();
				Scene_Manager::_current_scene->Add_Child ( Bean );
				break; }

			case 2: // piss gun ( like a water gun with piss)
				std::cout << " D == > ------------------------------ piss\n";
				break;
				
			case 3: // Bones rotate
				std::cout << " I----I I-------I  I---I  I-I I----------I I----I Bones\n";
				break;
			
			case 5: // kill stampante
				break;

			case 4: // the Wall
				// creates a single wall that spreads for a couple of units
				// x x x W x x x
				// x x W W W x x
				// x W W W W W x
				std::cout << "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww WALL\n";
				vec3 wall_pos = camera.Get_Pos ( ) + vec3{mouse_pos * camera.Get_component< Camera > ()->Scale, 0};
				Objekt* Walls = new Objekt ( "Wall_" + std::to_string (Wallss++), wall_pos, {100,100,100});
				Walls->Add_component < Wall > ( )->number = 6;
				Walls->Start();
				Scene_Manager::_current_scene->Add_Child ( Walls );
				break;
			}
		}
	}
};