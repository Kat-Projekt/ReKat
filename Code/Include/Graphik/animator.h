#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "sprite.h"
#include <chrono>
#include <vector>
#include <unordered_map>

// time manager
namespace Timer {
    auto start = std::chrono::system_clock::now();
    float delta_time = 0;
    float old_time = 0;
    float Get_Time () 
    { return (float)( std::chrono::system_clock::now ( ) - start ).count( ) / 10000000.0; }
    void Update_Delta_time ( ) {
        delta_time = Get_Time () - old_time;
        old_time = Get_Time ();
    }

} // namespace Timer


class Animation {
private:
    // stores frame at specified timestemp
    std::vector < std::pair < int, float > > frames;
    float totat_durutation = 0;
public:
    Animation ( ) { }
    ~Animation ( ) { }

    int Get_Frame ( float time ) {
        time = std::fmod( time, totat_durutation );
        for ( auto frame : frames ) 
        { if ( frame.second > time ) { return frame.first; } }
    }
    void Add_Frame ( int frame, float duration ) {
        totat_durutation += duration;
        frames.push_back ( {frame, totat_durutation} ); 
    }
    void Add_Frames ( std::vector < std::pair < int, float > > _frames ) {
        for ( auto f : _frames ) 
        { Add_Frame (f.first,f.second); }
    }
};


class Animator { 
private:
    std::unordered_map < std::string, Animation > Animations;
public:
    Animator ( ) { }
    Animation* Create_Animation ( std::string name ) {
        Animations.insert ( {name, Animation()} );
        return & Animations[name];         
    }
    Animation* Get_Animation ( std::string name ) 
    { return & Animations[name]; }
    int Get_Animation_Frame ( std::string name, float time ) 
    { return Animations[name].Get_Frame(time); }
    
    ~Animator ( ) { }
};

#endif