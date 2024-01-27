#ifndef AUDIO_LISTENER_H
#define AUDIO_LISTENER_H

#include "../objekt.h"
#include "../graphik.hpp"
#include "../../Synth/synth.hpp"
using namespace ReKat::synth;

#define Audio_Error(msg) if (alGenError() != AL_NO_ERROR) { std::cout << "error: " << msg << " at line: " << __LINE__ << " of file: " << __FILE__; } 

class Audio_Listener : public Behaviour {
public:
    // create context
	void Start ( ) {
        ReKat::synth::Start ( );
    }

    void Update ( ) {
        vec3 pos = obj->Get_Pos ();
		alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
		Audio_Error("posizione Errata");
    }
};

#endif