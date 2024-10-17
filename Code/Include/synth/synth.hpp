#ifndef SYNTH
#define SYNTH

// include openal libs
#include <AL/al.h>
#include <AL/alc.h>

// load lib
#include "audioFile.h"

// debuging
#include "synth_debugger.hpp"

#include <unordered_map>
#include <string>

namespace ReKat {
namespace synth {
    #define BUF_LEN 1024
	#define SAMPLE_FREQ 44100
	static ALCdevice *device;
	static ALCcontext *context;
	// points to z positive and up is y positive ( as locking to the screen )
	static ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f }; 

    /// @brief Starts engine (work with static varibles)
    static int Start ( );
    /// @brief Terminates angine and deallocates resources
    static void End ( );
} /* Synth */ } // ReKat

// implementation
#include <iostream>
namespace ReKat::synth {
    static int Start ( ) {
		// creating device
		device = alcOpenDevice ( NULL );
		if ( !device ) { DEBUG ( 2, "Failed to inizializate al device" ); }

		// creating context
		context = alcCreateContext ( device, NULL );
		if ( !alcMakeContextCurrent ( context ) ) 
		{ DEBUG ( 2, "Failed to inizializate al context" ); } AL_CHECK_ERROR;

		// set orientation
		alListener3f ( AL_POSITION, 0, 0, 1.0f ); AL_CHECK_ERROR;
		alListener3f ( AL_VELOCITY, 0, 0, 0 ); AL_CHECK_ERROR;
		alListenerfv ( AL_ORIENTATION, listenerOri ); AL_CHECK_ERROR;

		DEBUG ( 4, "Inizialized Synth System" );
		return 0;
	}

	static void SetGain ( float _gain ) 
	{ alListenerf ( AL_GAIN, _gain ); AL_CHECK_ERROR; }

	static void Reposition ( float* _pos ) 
	{ alListenerfv ( AL_POSITION, _pos ); AL_CHECK_ERROR; }

	static void Set_Velocity ( float* _vel ) 
	{ alListenerfv ( AL_VELOCITY, _vel ); AL_CHECK_ERROR; }

	static void Reorientate ( float* _ori ) 
	{ alListenerfv ( AL_ORIENTATION, _ori ); AL_CHECK_ERROR; }

    static void End ( ) {
		// terminating device and context
		device = alcGetContextsDevice(context);
		alcMakeContextCurrent(NULL);
		alcDestroyContext(context);
		alcCloseDevice(device);
		DEBUG ( 3, "Terminated Synth System" );
	}
}


#endif