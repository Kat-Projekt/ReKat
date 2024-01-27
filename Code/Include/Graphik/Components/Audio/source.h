#ifndef AUDIO_LISTENER_H
#define AUDIO_LISTENER_H

#include "../objekt.h"
#include "../graphik.hpp"
#include "../../Synth/synth.hpp"
using namespace ReKat::synth;

#define Audio_Error(msg) if (alGenError() != AL_NO_ERROR) { std::cout << "error: " << msg << " at line: " << __LINE__ << " of file: " << __FILE__; } 

class Audio_Listener : public Behaviour {
    ALuint source;
public:
    std::string path;
    // create context
	void Start ( ) {
        ReKat::synth::Create_Buffer ( obj->Get_Name(), path );
        ReKat::synth::Create_Source ( obj->Get_Name() );
        source = ReKat::synth::Sources [ obj->Get_Name() ];
    }

    void Update ( ) {
        vec3 pos = obj->Get_Pos ();
		alSource3f(source, AL_POSITION, pos.x, pos.y, pos.z );
		Audio_Error(FAILED_SOURCE_CRETION);
    }

    void Play ( ) {
        ReKat::synth::Play ( obj->Get_Name(), obj->Get_Name() );
    }
};

#endif