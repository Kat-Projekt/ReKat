#pragma once
#include "../global.h"
using namespace ReKat::grapik::Input;

class Player : public Behaviour {
private:
    float speed = 4.0f;
    float camera_speed = 4.0f;
    bool attack = false;
    bool camera_centered = true;
    int gobleino = 1;
    glm::vec2 m_ = {0,0};
public:
    void Start ( ) {
        Manager::Object_Get("Spada")->Set_Active(attack);
        Manager::Active_Scene->cam.Move( Manager::Object_Get("Player")->Get_pos( ) );
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

    void Update ( ) {
        // controls
        glm::vec2 dpos = {0,0};
        if ( Key_Pressed( "A" ) ) { dpos += glm::vec2{-1,0}; }
        if ( Key_Pressed( "W" ) ) { dpos += glm::vec2{0,-1}; }
        if ( Key_Pressed( "D" ) ) { dpos += glm::vec2{1,0};  }
        if ( Key_Pressed( "S" ) ) { dpos += glm::vec2{0,1};  }
        Manager::Object_Get("Player")->DMove ( Normalize(dpos) * speed );

        if ( Key_Down ( "Mouse1" ) ) {
            attack = !attack;
            std::cout << ( attack ? "attack" : "no");
            Manager::Object_Get("Spada")->Set_Active(attack);
        }

        if ( Key_Down ( "C" ) ) { camera_centered = !camera_centered; }
        if ( Key_Down ( "Mouse3" ) ) { camera_centered = false; }

        if ( camera_centered ) {
            glm::vec2 pos = lerp ( Manager::Object_Get("Player")->Get_pos( ), Manager::Active_Scene->cam.Get_Pos(), Timer::delta_time * camera_speed );
            Manager::Active_Scene->cam.DMove( pos );
        } 

        if ( Key_Pressed ( "Mouse3" ) ) {
            mouse_pos;
            Manager::Active_Scene->cam.DMove ( (m_ - mouse_pos) * Manager::Active_Scene->cam.Get_Scale() );
        }
        m_ = mouse_pos;

        // if ( Key_Down ( "P" ) ) { Manager::Object_Load("gobelino_" + std::to_string(gobleino++), "gobelino", {100,100},{100,100} )->Add_component<gobelino>()->name = "gobelino_"+ std::to_string(gobleino);  }
    }
};