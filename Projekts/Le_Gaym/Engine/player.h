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
		
		if ( Key_Pressed( "A" ) && Key_Pressed( "W" ) && Key_Pressed( "D" ) && Key_Pressed( "S" ) ) {
			Manager::Object_Get("Player")->Set_Active(false);
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