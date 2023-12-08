#pragma once
#include "../global.hpp"

static int gobelini_counter = 0;

static glm::vec2 speed = {1,1};
static int anim_dutation = 300;
static uint64_t ms;
class gobelino {
private:
    int entity_id;
    /* data */
public:
    // spawn a gobelino
    gobelino ( glm::vec2 pos, glm::vec2 size ) {
        entity_id = gobelini_counter++;
       // std::cout << "inizialing gobleino_" << entity_id << '\n';
        Manager::Object_Load ( "gobelino_" + entity_id, "gobelino", pos, size );
        Manager::Object_Get ( "gobelino_" + entity_id )->Set_Start_call(Start);
        Manager::Object_Get ( "gobelino_" + entity_id )->Set_Update_call(Update);
    }

    static void Start ( std::string name ) {
        ms = Get_Time();
    }

    static void Update ( std::string name ) { // follow player
        glm::vec2 vector = Manager::Object_Get("Player")->Get_pos() - Manager::Object_Get(name)->Get_pos();
        if ( vector == glm::vec2{0,0} ) { return; }
        // normalize
        float magnetude = sqrt(vector.x*vector.x + vector.y*vector.y);
        vector = { vector.x / magnetude * speed.x, vector.y / magnetude * speed.y };

        if ( ms + anim_dutation < Get_Time ( ) ) 
        { Manager::Object_Get ( name )->DChange_frame(1); ms = Get_Time(); } 

        Manager::Object_Get ( name )->DMove( vector ); 
    }

    ~gobelino ( ) { }
};
