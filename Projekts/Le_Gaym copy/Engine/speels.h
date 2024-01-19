#pragma once
#include "../global.h"
using namespace ReKat::grapik::Input;

class Beans : public Behaviour {
	public:
	vec2 target_pos;
	
	void Update ( ) {
		Manager::Object_Get(name)->Change_frame ( 0 );
		Manager::Object_Get ( name )->Move ( lerp ( Manager::Object_Get ( name )->Get_pos(), target_pos, Timer::delta_time *5 ) );

		Manager::Object_Get(name)->Set_Scale ( 0.5 );
		auto D = Dist ( Manager::Object_Get(name)->Get_pos(), target_pos );
		if ( D < 256 ) {
			Manager::Object_Get(name)->Set_Scale ( 1 - D / 512 );
		}

		if ( D < 100 ) { Manager::Object_Get(name)->Change_frame ( 1 ); }
		if ( D < 10 ) { Manager::Object_Get(name)->Change_frame ( 2 ); }
		if ( D < 3 ) { Manager::Object_Get(name)->Change_frame ( 3 ); }
	}
};

class Lasurs : public Behaviour {
	public:
	vec2 target_pos;
	vec2 dir = {0,0};
	vec2 start_pos = {0,0};
	float progress = 0;
	float speed = 3;
	float fase = 3 * 3.1415;
	float amplitude = 20;

	void Start ( ) {
		start_pos = Manager::Object_Get (name)->Get_pos();
		float a = angle ( target_pos - start_pos );
		dir = normalize ( target_pos - start_pos );
		Manager::Object_Get ( name )->Rotate ( a );
	}

	void Update ( ) {
		progress += Timer::delta_time * speed;

		Manager::Object_Get ( name )->Move ( lerp ( start_pos, target_pos, progress ) + 
											 vec2 { cos ( fase * progress ) * dir.y,
											        sin ( fase * progress ) * dir.x } * amplitude);

		if ( progress > 0.8 ) { Manager::Object_Get ( name )->Change_frame (1); }
		else { Manager::Object_Get ( name )->Change_frame (0); }
		if ( progress > 1 ) { Manager::Object_Delete ( name ); }
	}
};

class Wall : public Behaviour {
	public:
	void Start ( ) {
		if ( number <= 0 ) { return; }
		number--;
		// first wall
		if ( direction == vec2{0,0} ) {
			// left wall
			vec2 Wall_pos = Manager::Object_Get(name)->Get_pos ( ) + vec2{100,0};
			
			Object* Wall_o = Manager::Object_Load ( name + ":" + std::to_string(number) + "L", "Wall", Wall_pos, {100,100});
			auto W = Wall_o->Add_component < Wall > ();
			W->number = number;
			W->direction = {100,0};
			Wall_o->Add_component < Collider > ();
			Manager::Active_Scene->Add_Object ( Wall_o );
			
			// rigth wall
			Wall_pos = Manager::Object_Get(name)->Get_pos ( ) + vec2{-100,0};
			Wall_o = Manager::Object_Load ( name + ":" + std::to_string(number) + "R", "Wall", Wall_pos, {100,100});
			W = Wall_o->Add_component < Wall > ();
			W->number = number;
			W->direction = {-100,0};
			Wall_o->Add_component < Collider > ();
			Manager::Active_Scene->Add_Object ( Wall_o );

			return;
		}
		
		vec2 Wall_pos = Manager::Object_Get(name)->Get_pos ( ) + direction;
			
		Object* Wall_o = Manager::Object_Load ( name + ":" + std::to_string(number) + (direction.x > 0 ? "L" : "R"), "Wall", Wall_pos, {100,100});
		auto W = Wall_o->Add_component < Wall > ();
		W->number = number;
		W->direction = direction;
		Wall_o->Add_component < Collider > ();
		Manager::Active_Scene->Add_Object ( Wall_o );			
	}
	void Update ( ) { }

	
	int number;
	vec2 direction = {0,0};
};

class Spells : public Behaviour {
	int speel;
	int Beanss = 0;
	int Lasurss = 0;
public:
	
	std::vector < std::string > spell_list = {"Lasur", "Beans" , "Bones", "Satan piss", "Wall"};
	int spell_count = spell_list.size();
	float spell_angle = 2 * 3.1415 / spell_count ;
    void Start ( ) {
		for ( size_t i = 0; i < spell_count; i++ ) {
			Manager::UI_Object_Get ( name )->Add_Sub_Object ( spell_list[i], 
			Manager::UI_Object_Load ( spell_list[i], "", "Spells", "", vec2{ sin( spell_angle * (i + 0.5) ), cos( spell_angle * (i + 0.5) ) } * 100.0f, {100,100}, nullptr, i*2 + 1 ) );
		}
		Manager::UI_Object_Get ( name )->Set_Active(false);
	}
    void Update ( ) {
		// spell whell
		if ( Key_Down ( "Alt" ) ) 
		{ Manager::UI_Object_Get ( name )->Set_Active(true); }
		if ( Key_Pressed ( "Alt" ) ) {
			int current_spell = ( angle ( mouse_pos ) / spell_angle + 1 );
			current_spell %= spell_count;
			current_spell = spell_count - current_spell;
			current_spell %= spell_count;
			for ( size_t i = 0; i < spell_count; i++ ) {
				if ( i == current_spell ) { Manager::UI_Object_Get ( spell_list[i] )->Change_frame ( i * 2 ); }
				else { Manager::UI_Object_Get ( spell_list[i] )->Change_frame ( i * 2 + 1 ); }
			}
			// std::cout << "c: " << current_spell;
		}
		if ( Key_Up ( "Alt" ) ) {
			Manager::UI_Object_Get ( name )->Set_Active(false);
			float a = angle ( mouse_pos );
			std::cout << "angle: " << a << '\n';
			a /= spell_angle;
			speel = a + 1;
		}

		if ( Key_Down ( "Mouse1" ) ) {
			switch ( speel ) {
			case 0: { // cast laser ray from player in mouse direction
				std::cout << "------------------------------ lasur\n";
				vec2 Lasur_target = Manager::Active_Scene->cam.Get_Pos() + mouse_pos * Manager::Active_Scene->cam.Get_Scale ( );
				std::cout << Lasur_target.x << " : " << Lasur_target.y;
				Object* Lasur = Manager::Object_Load ( "Lasur" + std::to_string (Lasurss++), "Lasur", Manager::Object_Get("Player")->Get_pos(), {60,60});
				Lasur->Add_component < Lasurs > ()->target_pos = Lasur_target;
				Lasur->Add_component < Collider > ()->Set_Obj(Lasur)->Set_trigger(true);
				Manager::Active_Scene->Add_Object ( Lasur );
				break; }
			
			case 1: { // spawn a can of beans
				std::cout << "oooooooooooooooooooooooooooo Beans\n";
				vec2 bean_pos = Manager::Active_Scene->cam.Get_Pos() + mouse_pos * Manager::Active_Scene->cam.Get_Scale ( );
				std::cout << bean_pos.x << " : " << bean_pos.y;
				Object* Bean = Manager::Object_Load ( "Bean" + std::to_string (Beanss++), "Bean", Manager::Object_Get("Player")->Get_pos(), {100,100});
				Bean->Add_component < Beans > ()->target_pos = bean_pos;
				Manager::Active_Scene->Add_Object ( Bean );
				break; }

			case 2: // piss gun ( like a water gun with piss)
				std::cout << " D == > ------------------------------ piss\n";
				break;
				
			case 3: // Bones rotate
				std::cout << " I----I I-------I  I---I  I-I I----------I I----I Bones\n";
				break;
			
			case 4: // kill stampante
				break;

			case 5: // the Wall
				// creates a single wall that spreads for a couple of units
				// x x x W x x x
				// x x W W W x x
				// x W W W W W x
				break;
			}
		}
	}
};