#pragma once
#include "global.hpp"
using namespace ReKat::grapik::Input;

class Player : public Behaviour {
private:
    float speed = 4.0f;
    bool attack = false;
    int gobleino = 1;
public:
    void Start ( ) {
        Manager::Object_Get("Spada")->Set_Active(attack);
    }

    void Update ( ) {
        // controls 
        if ( Key_Pressed( "A" ) ) { Manager::Object_Get("Player")->DMove ( {-1 * speed,0} ); }
        if ( Key_Pressed( "W" ) ) { Manager::Object_Get("Player")->DMove ( {0,-1 * speed} ); }
        if ( Key_Pressed( "D" ) ) { Manager::Object_Get("Player")->DMove ( {1 * speed,0} );  }
        if ( Key_Pressed( "S" ) ) { Manager::Object_Get("Player")->DMove ( {0,1 * speed} );  }

        if ( Key_Down ( "Mouse1" ) ) {
            attack = !attack;
            std::cout << ( attack ? "attack" : "no");
            Manager::Object_Get("Spada")->Set_Active(attack);
        }

        // if ( Key_Down ( "P" ) ) { Manager::Object_Load("gobelino_" + std::to_string(gobleino++), "gobelino", {100,100},{100,100} )->Add_component<gobelino>()->name = "gobelino_"+ std::to_string(gobleino);  }
    }
};