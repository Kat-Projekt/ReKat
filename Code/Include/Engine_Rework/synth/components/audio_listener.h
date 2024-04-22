#ifndef AUDIO_LISTENER_H
#define AUDIO_LISTENER_H

#include "../../objekt.hpp"
#include "../synth.hpp"

class Audio_Listener : public Behaviour {
public:
    // create context
	void Start ( ) 
	{ }

    void Update ( ) {
        vec3 pos = obj->Get_Pos ();
		alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
		int error;
		AL_CHECK_ERROR;
    }
};

#endif