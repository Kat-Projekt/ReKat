#ifndef _TheWeel
#define _TheWeel

#include "Bean.h"
#include "Dart.h"
#include "Piss.h"
#include "Wall.h"

class TheWeel : public Behaviour {
	int _selected_speel = 0;
	Objekt* Weel;
	Objekt* SpellsContainer;
	std::vector < std::string > spell_list = {"Lasur" , "Wall", "Beans"};
	int spell_count = spell_list.size();
	float spell_angle = 2 * 3.1415 / spell_count ;
public:
	Camera* camera;
    virtual void Start ( ) {
		Weel = new Objekt ( "spells", {0,0,0.6} );
		SpellsContainer = new Objekt ( "SpellsContainer" );
		obj->Add_Child ( Weel );
		obj->Get_Father ( ) ->Add_Child ( SpellsContainer );

		for ( size_t i = 0; i < spell_count; i++ ) {
			auto S = new Objekt ( spell_list[i], vec3 { sin( spell_angle * (i + 0.5) ), cos( spell_angle * (i + 0.5) ), 0 } * 100.0f, {100,100,100} );
			S->Add_Component < Sprite > ( )
			-> Set ( Manager::Texture_Get( "weel" ), Manager::Shader_Get( "sprite" ), camera, {2,4} ) 
			-> frame = i*2 + 1;

			S->Start ( );
			Weel->Add_Child ( S );
		}
		Weel->Set_Active(false);
	}
    
	template < class T > 
	void Spawn_Speel ( ) {
		vec3 target = camera->obj->Get_Pos ( ) + vec3{mouse_pos * camera->Scale, 0};
		Objekt* speel = new Objekt ( "spell", obj->Get_Pos(), {60,60,0}) ;
		speel->Add_Component < T > ( )->target = target;
		speel->Get_Component < T > ( )->cam = camera;
		SpellsContainer->Add_Child ( speel );
	}
	virtual void Update ( ) {
		// spell whell
		if ( Key_Down ( "Alt" ) ) 
		{ Weel->Set_Active(true); }
		if ( Key_Pressed ( "Alt" ) ) {
			int current_spell = ( angle ( mouse_pos ) / spell_angle + 1 );
			current_spell %= spell_count;
			current_spell = spell_count - current_spell;
			current_spell %= spell_count;
			for ( size_t i = 0; i < spell_count; i++ ) {
				if ( i == current_spell ) { Weel->Get_Children ( spell_list[i] )->Get_Component < Sprite > ()->frame = i * 2; }
				else {  Weel->Get_Children ( spell_list[i] )->Get_Component < Sprite > ()->frame = i * 2 + 1; }
			}
		}
		if ( Key_Up ( "Alt" ) ) {
			Weel->Set_Active(false);
			float a = angle ( mouse_pos );
			std::cout << "angle: " << a << '\n';
			a /= spell_angle;
			_selected_speel = a + 1;
			if ( _selected_speel != 0 )
			{ _selected_speel = spell_list.size() - _selected_speel; }
			std::cout << "c: " << _selected_speel;
		}

		// spell casting
		if ( Key_Down ( "Mouse1" ) ) {
			switch ( _selected_speel ) {
			case 0: std::cout << "casting spell: " << spell_list[0] << '\n'; 
				Spawn_Speel < Dart > ( ); break;
			case 1: std::cout << "casting spell: " << spell_list[1] << '\n';
				Spawn_Speel < Wall > ( ); break;
			case 2: std::cout << "casting spell: " << spell_list[2] << '\n';
				Spawn_Speel < Bean > ( ); break;
			}
		}
	}
	virtual void Fixed_Update ( ) { }
	virtual void UI_Update ( ) { }

    virtual void Collision ( Objekt* _obj ) { _obj; }
    virtual void Collision_Trigger ( Objekt* _obj ) { _obj; }
};

#endif