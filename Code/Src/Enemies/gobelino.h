#pragma once
#include "../global.hpp"

static int gobelini_counter = 0;

class gobelino : public Behaviour {
private:
    float speed = 1.0f;
public:
    std::string name;

    void Update ( ) { // follow player
        glm::vec2 vector = Manager::Object_Get("Player")->Get_pos() - Manager::Object_Get ( name )->Get_pos();
        
        Manager::Object_Get ( name )->Change_frame( Manager::Animator_Get( "walk" )->Get_Animation_Frame("walk",Timer::Get_Time()));
        
        if ( vector == glm::vec2{0,0} ) { return; }
        // normalize
        float magnetude = sqrt(vector.x*vector.x + vector.y*vector.y);
        vector = { vector.x / magnetude * speed, vector.y / magnetude * speed };
        Manager::Object_Get ( name )->DMove( vector );
    }
};
