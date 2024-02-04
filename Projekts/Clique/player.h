#include "com.h"
using namespace ReKat::grapik::Input;

class Gobelino : public Behaviour {
private:
	float life = 20;
    float speed = 100.0f;
public:
	Objekt *pointer;
    void Start ( ) { }

    void Update ( ) {
        glm::vec3 vector = pointer->Get_Pos() - obj->Get_Pos();
        
        //Manager::Object_Get ( name )->Change_frame( Manager::Animator_Get( "walk" )->Get_Animation_Frame("walk",Timer::Get_Time()));
        
        if ( vector == glm::vec3{0,0,0} ) { return; }
        // normalize
		vector = glm::normalize ( vector );
        obj->Inc_Pos( vector * speed * Timer::delta_time );
		// std::cout << "pos" << Manager::Object_Get ( name )->Get_pos().x << " x " << Manager::Object_Get ( name )->Get_pos().y << '\n';
	}

	void Collision_Trigger ( Objekt * e ) { obj->Set_Active(false); }
};

class Player : public Behaviour {
private:
    float speed = 200.0f;
    float camera_speed = 4.0f;
    bool camera_centered = true;
    int gobleino = 1;
    glm::vec2 m_ = {0,0};
	float progression = 10;
	float attack_duration = 0.15f;
	float attack_angle;
	float attack_spread = 1.5f;
	int Killed_gobelins = 0;
public:

	void Kill ( ) { Killed_gobelins ++; }

    void Update ( ) {
        // controls
        glm::vec2 dpos = {0,0};
        if ( Key_Pressed( "A" ) ) { dpos += glm::vec2{-1,0}; }
        if ( Key_Pressed( "W" ) ) { dpos += glm::vec2{0,1};  }
        if ( Key_Pressed( "D" ) ) { dpos += glm::vec2{1,0};  }
        if ( Key_Pressed( "S" ) ) { dpos += glm::vec2{0,-1}; }
        obj->Get_component < Rigidbody > ()->velocity = ( vec3(Normalize(dpos) * speed, 0) );
		
		if ( Key_Down( "A" ) && Key_Down( "W" ) && Key_Down( "D" ) && Key_Down( "S" ) ) {
			Objekt *K = new Objekt("Kat");
			// K->Add_component < Sfere_Collider > ( );
			K->Add_component < Sprite > ( )->Set ( Manager::Texture_Get( "sprite" ), Manager::Shader_Get( "sprite" ), camera.Get_component < Camera > ( ), {2,1} );
			K->Add_component < Gobelino > ( )->pointer = obj;
			K->Add_component < Sfere_Collider > ( )->Set_Size (50);
			K->Add_component < Rigidbody >();
			std::cout << "Created obj\n";
			K->Start ( );
			Scene_Manager::_current_scene->Add_Child( K );
			std::cout << "Added obj\n";
		}

        /*if ( Key_Down ( "Mouse1" ) && progression >= attack_duration ) {
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
		}*/

		/*if ( Key_Down ("P") ) {
			std::cout << "Play\n";
			ReKat::synth::Play ( "Player", "Player" );
		}*/

        // if ( Key_Down ( "P" ) ) { Manager::Object_Load("gobelino_" + std::to_string(gobleino++), "gobelino", {100,100},{100,100} )->Add_component<gobelino>()->name = "gobelino_"+ std::to_string(gobleino);  }
    }
};