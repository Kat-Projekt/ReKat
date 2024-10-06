#ifndef AUDIO_BUFFER
#define AUDIO_BUFFER

// include openal libs
#include <AL/al.h>
#include <AL/alc.h>

// debuging
#include "synth_debugger.hpp"

// load lib
#include "audioFile.h"

class Buffer {
public:
	ALuint buffer;
	
	Buffer ( ) { };
	~Buffer ( ) { End( ); };

	int Make ( short* _buf, long _len, int _sample_freq = 44100 ) {
		int error;
		alGenBuffers ( 1, &buffer ); AL_CHECK_ERROR;
		alBufferData ( buffer, AL_FORMAT_MONO16, _buf, _len, _sample_freq ); AL_CHECK_ERROR;

		DEBUG ( 4, "Created Buffer ", buffer );
		return 0;
	}
	int Make ( double *data, long _len, int _sample_freq = 44100  ) {
		short *_buf = (short*) calloc ( ( _len % 2 == 0 ? _len : _len + 1 ), sizeof (short) );
		for (size_t i = 0; i < _len; i++) 
		{ _buf[i] = data[i] * 32768.0; }
		return Make ( _buf, _len, _sample_freq );
	}
	int Make ( std::string path, int _sample_freq = 44100  ) {
		AudioFile < double > a;
		if ( ! a.load ( path ) ) 
		{ DEBUG ( 2, "Failed Opening File ", path ) }

		ALsizei _len = a.getNumSamplesPerChannel ( );
		ALsizei _sample_rate = a.getSampleRate ( );
		double * data = a.samples[0].data();

		return Make ( data, _len, _sample_freq );
	}
	void End ( )
	{ alDeleteBuffers ( 1, &buffer ); }
};

#endif