#ifndef SYNTH
#define SYNTH

#include "../objekt.hpp"

// include openal libs
#include <AL/al.h>
#include <AL/alc.h>

#define BUF_LEN 1024
#define SAMPLE_FREQ 44100

#define AL_CHECK_ERROR				\
	error = alGetError ( );			\
	if ( error != AL_NO_ERROR) 		\
	{ std::cout << "\tErrore: " << error << " at line: " << __LINE__ << " of file: " << __FILE__ << '\n'; throw; }

class Synth : public Behaviour {
private:
	ALCdevice *device;
	ALCcontext *context;
public:
	void Start ( ) {
		int error;
		// inizialize device
		device = alcOpenDevice ( NULL );
		if ( !device ) { return; }

		// inizialize context
		context = alcCreateContext ( device, NULL );
		if ( !alcMakeContextCurrent ( context ) ) { return; }
		AL_CHECK_ERROR;
		
		static ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };

		alListener3f(AL_POSITION, 0, 0, 1.0f);
		AL_CHECK_ERROR;
		alListener3f(AL_VELOCITY, 0, 0, 0);
		AL_CHECK_ERROR;
		alListenerfv(AL_ORIENTATION, listenerOri);
		AL_CHECK_ERROR;

		DEBUG ( "started audio drivers\n" );
	}

	void Delete ( ) {
		color ( "deleting audio drives\n", FOREGROUND_BLUE );
		device = alcGetContextsDevice(context);
		// delete context
		alcMakeContextCurrent(NULL);
		alcDestroyContext(context);
		// delete device
		alcCloseDevice(device);
	}
};

#endif