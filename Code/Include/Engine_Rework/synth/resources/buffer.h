#include "../synth.hpp"

// load lib
#include "audioFile.h"

class Buffer {
public:
	ALuint buffer;
	
    enum STATUS {
        SUCCESS,
		FAILED_OPENING_FILE,
		FAILED_BUFFER_CRETION,
		FAILED_BUFFER_COPY,
		FAILED_BINDING
    };

	Buffer ( ) { };
	~Buffer ( ) { End( ); };

	int Make ( short* _buf, long _len ) {
		int error;
		alGenBuffers(1, &buffer);
		AL_CHECK_ERROR;

		alBufferData ( buffer, AL_FORMAT_MONO16, _buf, _len, SAMPLE_FREQ );
		AL_CHECK_ERROR;

		std::cout << "buffer: " << buffer << '\n';
		return SUCCESS;
	}
	int Make ( double *data, long _len ) {
		short *_buf = (short*) calloc ( ( _len % 2 == 0 ? _len : _len + 1 ), sizeof (short) );
		for (size_t i = 0; i < _len; i++) 
		{ _buf[i] = data[i] * 32768.0; }
		return Make ( _buf, _len );
	}
	int Make ( std::string path ) {
		AudioFile < double > a;
		if ( ! a.load ( path ) ) 
		{ return FAILED_OPENING_FILE; }

		ALsizei _len = a.getNumSamplesPerChannel ( );
		ALsizei _sample_rate = a.getSampleRate ( );
		double * data = a.samples[0].data();

		return Make ( data, _len );
	}
	void End ( )
	{ alDeleteBuffers ( 1, &buffer ); }
};
