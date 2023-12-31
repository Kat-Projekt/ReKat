#ifndef SYNTH
#define SYNTH

// include openal libs
#include <AL/al.h>
#include <AL/alc.h>

// load lib
#include "audioFile.h"

//  position lib
#include <glm/glm.hpp>

#include <unordered_map>
#include <string>

// per Gatone
/* Guardati openal-soft.org
 * Ho gia incluso la libreria in Code/Lib/openal-soft
 * cerca di scrivere l'implementazione descritta sotto
 * Grazie
*/

namespace ReKat {
namespace synth {
    #define BUF_LEN 1024
	#define SAMPLE_FREQ 44100
	static ALCdevice *device;
	static ALCcontext *context;
	static ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
	static std::unordered_map < std::string, ALuint > Buffers;
	static std::unordered_map < std::string, ALuint > Sources;

    enum STATUS {
        SUCCESS,
		FAILED_INIT,
		FAILED_CONTEXT,
		FAILED_LISTENER,
        FAILED_CHECK,
		FAILED_OPENING_FILE,
		FAILED_BUFFER_CRETION,
		FAILED_SOURCE_CRETION,
		FAILED_BINDING,
		FAILED_PLAYING,
		FAILED_STATUS_GET
    };

    /// @brief Starts engine (work with static varibles)
    static int Start ( );
    /// @brief Terminates angine and deallocates resources
    static void End ( );

	/// @brief Creates a buffer named name
	/// @param _buf 
	/// @param _len 
	/// @return status
	static int Create_Buffer ( std::string name, short* _buf, long _len );
	static int Create_Buffer ( std::string name, std::string path );

    /// @brief plays a buffer (_buf) of lenth (_len)
    /// @param _buf 
    /// @param _len 
    /// @return status
    static int Play ( std::string where, std::string what, bool wait = false );
} /* Synth */ } // ReKat

// implementation
namespace ReKat::synth {
	#define CHECK_ERROR( e ) 			 \
		if (alGetError() != AL_NO_ERROR) \
		{ return e; }

    static int Start ( ) {
		// creating device
		device = alcOpenDevice ( NULL );
		if ( !device ) { return FAILED_INIT; }

		// creating context
		context = alcCreateContext ( device, NULL );
		if ( !alcMakeContextCurrent ( context ) ) 
		{ return FAILED_CONTEXT; }
		CHECK_ERROR(FAILED_CONTEXT);

		// set orientation
		alListener3f(AL_POSITION, 0, 0, 1.0f);
		CHECK_ERROR(FAILED_LISTENER);
		alListener3f(AL_VELOCITY, 0, 0, 0);
		CHECK_ERROR(FAILED_LISTENER);
		alListenerfv(AL_ORIENTATION, listenerOri);
		CHECK_ERROR(FAILED_LISTENER);
	}
    static void End ( ) {
		// ternimating source
		for ( auto B : Buffers ) 
		{ alDeleteBuffers ( 1, &B.second ); }
		// terminating buffers
		for ( auto S : Sources )
		{ alDeleteSources ( 1, &S.second );}

		// terminating device and context
		device = alcGetContextsDevice(context);
		alcMakeContextCurrent(NULL);
		alcDestroyContext(context);
		alcCloseDevice(device);
	}

	static int Create_Buffer ( std::string name, short* _buf, long _len ) {
		ALuint buffer;
		alGenBuffers(1, &buffer);
		CHECK_ERROR(FAILED_BUFFER_CRETION);

		alBufferData ( buffer, AL_FORMAT_MONO16, _buf, _len, SAMPLE_FREQ );
		std::cout << "_len: " << _len << " _sam: " << SAMPLE_FREQ << '\n';
		CHECK_ERROR(FAILED_BUFFER_CRETION);

		Buffers.insert ( { name, buffer } );
		return SUCCESS;
	}
	static int Create_Buffer ( std::string name, std::string path ) {
		AudioFile < float > a;
		if ( ! a.load ( path ) ) 
		{ return FAILED_OPENING_FILE; }

		ALsizei _len = a.getNumSamplesPerChannel ( );
		ALsizei _sample_rate = a.getSampleRate ( );
		float * data = a.samples[0].data();

		// reformat data
		short *_buf = (short*) calloc ( _len, sizeof (short) );
		for (size_t i = 0; i < _len; i++) {
			// convert float from (-1) - (1) to (-32.768) - (32.767)
			_buf[i] = data[i] * 32000.0f;
			// std::cout << i << "\t" << _buf[i] << '\n';
		}

		ALuint buffer;
		alGenBuffers(1, &buffer);
		CHECK_ERROR(FAILED_BUFFER_CRETION);

		alBufferData ( buffer, AL_FORMAT_MONO16, _buf, _len, _sample_rate );
		std::cout << "_len: " << _len << " _sam: " << _sample_rate << '\n';
		CHECK_ERROR(FAILED_BUFFER_CRETION);

		Buffers.insert ( { name, buffer } );
		return 0;
	}

	static int Create_Source ( std::string name, glm::vec3 _pos ) {
		ALuint source;
		alGenSources((ALuint)1, &source);
		CHECK_ERROR(FAILED_SOURCE_CRETION);

		// confinguration
		alSourcef(source, AL_PITCH, 1);
		CHECK_ERROR(FAILED_SOURCE_CRETION);
		alSourcef(source, AL_GAIN, 1);
		CHECK_ERROR(FAILED_SOURCE_CRETION);
		alSource3f(source, AL_POSITION, _pos.x, _pos.y, _pos.z );
		CHECK_ERROR(FAILED_SOURCE_CRETION);
		alSource3f(source, AL_VELOCITY, 0, 0, 0);
		CHECK_ERROR(FAILED_SOURCE_CRETION);
		alSourcei(source, AL_LOOPING, AL_FALSE);
		CHECK_ERROR(FAILED_SOURCE_CRETION);

		Sources.insert ( { name, source } );
		return SUCCESS;
	}

	static int Play ( std::string where, std::string what, bool wait ) {
		std::cout << "playing: " << where << " " << what << '\n';
		alSourcei( Sources[where], AL_BUFFER, Buffers[what] );
		CHECK_ERROR(FAILED_BINDING);

		alSourcePlay( Sources[where] );
		CHECK_ERROR(FAILED_PLAYING);

		if ( wait ) {
			ALint source_state;
			alGetSourcei ( Sources[where], AL_SOURCE_STATE, &source_state );
			CHECK_ERROR(FAILED_STATUS_GET);
			while (source_state == AL_PLAYING) {
				alGetSourcei(Sources[where], AL_SOURCE_STATE, &source_state);
				CHECK_ERROR(FAILED_STATUS_GET);
			}
		}
		std::cout << "Played\n";

		return SUCCESS;
	}
}


#endif