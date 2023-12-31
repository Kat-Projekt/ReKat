#pragma once
#include "../global.h"
using namespace ReKat::grapik::Input;

class Player : public Behaviour {
private:
    float speed = 200.0f;
    float camera_speed = 4.0f;
    bool camera_centered = true;
    int gobleino = 1;
    glm::vec2 m_ = {0,0};
	float progression = 10;
	float attack_duration = 0.15;
	float attack_angle;
	float attack_spread = 1.5;
	int Killed_gobelins = 0;
public:
    void Start ( ) {
        Manager::Object_Get("Spada")->Set_Active(false);
        Manager::Active_Scene->cam.Move( Manager::Object_Get("Player")->Get_pos( ) );
		Manager::UI_Object_Get("DEATH")->Set_Active(true);
    }

    // linear interpolation 0 <= t <= 1
    glm::vec2 lerp ( glm::vec2 a, glm::vec2 b, float t ) {
        return ( a-b ) * t;
    }
    glm::vec2 Normalize ( glm::vec2 a ) {
        float m = sqrt(a.x*a.x + a.y*a.y);
        if ( m == 0 ) { return {0,0}; }
        return a / m;
    }

	float angle ( glm::vec2 v ) {
		float angle = 0;
		float X = v.x / sqrt( v.x * v.x + v.y *v.y );
		if ( v.y > 0 ) { angle = 3.1415+std::asin ( X ); } 
		else { angle = -std::asin ( X ); }
		return 3.1415 -angle;
	}

	void Kill ( ) { Killed_gobelins ++; }

    void Update ( ) {
        // controls
        glm::vec2 dpos = {0,0};
        if ( Key_Pressed( "A" ) ) { dpos += glm::vec2{-1,0}; }
        if ( Key_Pressed( "W" ) ) { dpos += glm::vec2{0,1};  }
        if ( Key_Pressed( "D" ) ) { dpos += glm::vec2{1,0};  }
        if ( Key_Pressed( "S" ) ) { dpos += glm::vec2{0,-1}; }
        Manager::Object_Get("Player")->DMove ( Normalize(dpos) * speed * Timer::delta_time );
		Manager::UI_Object_Get("DEATH")->Set_text ( "killed gobelins: " + std::to_string ( Killed_gobelins ) );

		if ( Key_Pressed( "A" ) && Key_Pressed( "W" ) && Key_Pressed( "D" ) && Key_Pressed( "S" ) ) {
			Manager::Object_Get("Player")->Set_Active(false);
			Manager::UI_Object_Get("DEATH")->Set_Active(true);
		}

        if ( Key_Down ( "Mouse1" ) && progression >= attack_duration ) {
			progression = 0;
			attack_angle = angle( mouse_pos );
            Manager::Object_Get("Spada")->Set_Active(true);
        }
		if ( progression <= attack_duration ) {
			progression += Timer::delta_time;
			// spada rotation
			Manager::Object_Get("Player")->Get_Sub_Object("Spada")->Rotate( attack_angle - ( attack_spread * progression/attack_duration ) + attack_spread * 0.5f );
		} else { Manager::Object_Get("Spada")->Set_Active(false);  }

        if ( Key_Down ( "C" ) ) { camera_centered = !camera_centered; }
        if ( Key_Down ( "Mouse3" ) ) { camera_centered = false; }

        if ( camera_centered ) {
            glm::vec2 pos = lerp ( Manager::Object_Get("Player")->Get_pos( ), Manager::Active_Scene->cam.Get_Pos(), Timer::delta_time * camera_speed );
            Manager::Active_Scene->cam.DMove( {pos.x,pos.y} );
        } 

        if ( Key_Pressed ( "Mouse3" ) ) 
		{ Manager::Active_Scene->cam.DMove ( (m_ - mouse_pos) * Manager::Active_Scene->cam.Get_Scale() ); }
        m_ = mouse_pos;

		if ( Key_Down ( "E" ) ) {
			std::cout << "E";
			Manager::Object_Get("S")->Get_component<Dialog>()->Start();
		}

		if ( Key_Down ("P") ) {
			std::cout << "Play\n";
			ReKat::synth::Play ( "Player", "Player" );
		}

        // if ( Key_Down ( "P" ) ) { Manager::Object_Load("gobelino_" + std::to_string(gobleino++), "gobelino", {100,100},{100,100} )->Add_component<gobelino>()->name = "gobelino_"+ std::to_string(gobleino);  }
    }

	void Collision ( std::string obj ) {  }
	void CollisionTrigger ( std::string obj ) {  }
};