#ifndef AUDIO_LISTENER_H
#define AUDIO_LISTENER_H

#include "../../objekt.hpp"
#include "../synth.hpp"

class Audio_Listener : public Behaviour {
public:
    // create context
	vec3 old_p;
	void Start ( ) { 
		old_p = obj->Get_Pos ( );
		DEBUG ( 4, "Audio Listener Component Started" );
	}

    void Update ( ) {
        vec3 pos = obj->Get_Pos ();
		float p [3]= { pos.x, pos.y, pos.z };
		ReKat::synth::Reposition ( p );
		pos = old_p - pos;
		float p2[3] = { pos.x, pos.y, pos.z };
		ReKat::synth::Set_Velocity ( p2 );
		DEBUG ( 5, "Updated Audio Listener Position");
    }
};

#endif