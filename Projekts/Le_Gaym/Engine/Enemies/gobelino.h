#pragma once
#include "../global.h"
using namespace ReKat::grapik::Input;

class Gobelino : public Behaviour {
private:
	float life = 20;
    float speed = 150.0f;
public:
    void Start ( ) { }

    // linear interpolation 0 <= t <= 1
    glm::vec2 lerp ( glm::vec2 a, glm::vec2 b, float t ) {
        return ( a-b ) * t;
    }

    void Update ( ) {
        glm::vec2 vector = Manager::Object_Get("Player")->Get_pos() - Manager::Object_Get ( name )->Get_pos();
        
        //Manager::Object_Get ( name )->Change_frame( Manager::Animator_Get( "walk" )->Get_Animation_Frame("walk",Timer::Get_Time()));
        
        if ( vector == glm::vec2{0,0} ) { return; }
        // normalize
        float magnetude = sqrt(vector.x*vector.x + vector.y*vector.y);
        vector = { vector.x / magnetude, vector.y / magnetude };
        Manager::Object_Get ( name )->DMove( vector  * speed * Timer::delta_time );
		// std::cout << "pos" << Manager::Object_Get ( name )->Get_pos().x << " x " << Manager::Object_Get ( name )->Get_pos().y << '\n';
	}

	void Collision ( std::string obj ) { }
	void CollisionTrigger ( std::string obj ) { 
		if ( obj == "Spada") {  life--; }
		if ( life <= 0 ) { 
			Manager::Object_Get ( name )->Set_Active(false); 
			// Manager::Object_Get ( "Player")->Get_component < Player > ()->Kill ();
			// std::cout << "play result " << ReKat::synth::Play ( "Bals", "Bals" ) << '\n';
		}
        // std::cout << name << " collison with: " << obj << '\n';
	}
};

// per chi suona le campane (Hemingway)