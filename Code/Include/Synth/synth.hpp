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
	static std::unordered_map < std::string, ALuint > P_Sources;

    enum STATUS {
        SUCCESS,
		FAILED_INIT,
		FAILED_CONTEXT,
		FAILED_LISTENER,
        FAILED_CHECK,
		FAILED_OPENING_FILE,
		FAILED_BUFFER_CRETION,
		FAILED_BUFFER_COPY,
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
#include <iostream>
ALenum Errore;
namespace ReKat::synth {
	#define _CHECK_ERROR( e, l ) 		\
		Errore = alGetError();			\
		if (Errore != AL_NO_ERROR) 		\
		{ std::cout << "\terrore: " << Errore << " at line: " << l << '\t'; return e; }
	#define CHECK_ERROR(e) _CHECK_ERROR(e,__LINE__)

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

		return SUCCESS;
	}
    static void End ( ) {
		// ternimating source
		for ( auto B : Buffers ) 
		{ alDeleteBuffers ( 1, &B.second ); }
		// terminating buffers
		for ( auto S : Sources )
		{ alDeleteSources ( 1, &S.second );}
		for ( auto S : P_Sources )
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
		CHECK_ERROR(FAILED_BUFFER_COPY);

		Buffers.insert ( { name, buffer } );
		//std::cout << "name: " << name << " _len: " << _len << " _sam: " << SAMPLE_FREQ << '\n';
		return SUCCESS;
	}
	
	static int Create_Buffer ( std::string name, double *data, long _len ) {
		ALuint buffer;
		// reformat data
		short *_buf = (short*) calloc ( ( _len % 2 == 0 ? _len : _len + 1 ), sizeof (short) );
		for (size_t i = 0; i < _len; i++) {
			// convert float from (-1) - (1) to (-32.768) - (32.767)
			_buf[i] = data[i] * 32000.0f;
			// std::cout << i << "\t" << _buf[i] << '\n';
		}

		alGenBuffers(1, &buffer);
		CHECK_ERROR(FAILED_BUFFER_CRETION);

		alBufferData ( buffer, AL_FORMAT_MONO16, _buf, _len, SAMPLE_FREQ );
		CHECK_ERROR(FAILED_BUFFER_COPY);

		Buffers.insert ( { name, buffer } );
		//std::cout << "name: " << name << " _len: " << _len << " _sam: " << SAMPLE_FREQ << '\n';
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
		short *_buf = (short*) calloc ( ( _len % 2 == 0 ? _len : _len + 1 ), sizeof (short) );
		for (size_t i = 0; i < _len; i++) {
			// convert float from (-1) - (1) to (-32.768) - (32.767)
			_buf[i] = data[i] * 32000.0f;
			// std::cout << i << "\t" << _buf[i] << '\n';
		}

		ALuint buffer;
		alGenBuffers(1, &buffer);
		CHECK_ERROR(FAILED_BUFFER_CRETION);

		alBufferData ( buffer, (a.getNumChannels() == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16), _buf, ( _len % 2 == 0 ? _len : _len + 1 ), _sample_rate );
		CHECK_ERROR(FAILED_BUFFER_COPY);

		Buffers.insert ( { name, buffer } );
		//std::cout << "name: " << name << " _len: " << _len << " _sam: " << _sample_rate << '\n';
		return 0;
	}

	static int Create_Source ( std::string name, glm::vec3 _pos, bool internal = false ) {
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

		if ( internal ) { P_Sources.insert ( { name, source } ); return SUCCESS; }
		Sources.insert ( { name, source } );
		//std::cout << "name: " << name << '\n';
		return SUCCESS;
	}

	static int Play ( std::string where, std::string what, bool wait ) {
		//std::cout << "playing: " << where << " " << what << '\n';
		alSourcei( Sources[where], AL_BUFFER, Buffers[what] );
		CHECK_ERROR(FAILED_BINDING);

		alSourcePlay( Sources[where] );
		CHECK_ERROR(FAILED_PLAYING);

		if ( wait ) {
			ALint source_state;
			alGetSourcei ( Sources[where], AL_SOURCE_STATE, &source_state );
			CHECK_ERROR(FAILED_STATUS_GET);
			while ( source_state == AL_PLAYING ) {
				alGetSourcei(Sources[where], AL_SOURCE_STATE, &source_state);
				CHECK_ERROR(FAILED_STATUS_GET);
			}
		}
		return SUCCESS;
	}
	static int Play ( std::string what ) {
		// get a free source
		std::string source = "";
		for ( auto s : P_Sources ) {
			ALint source_state;
			alGetSourcei ( s.second, AL_SOURCE_STATE, &source_state );
			CHECK_ERROR(FAILED_STATUS_GET);
			if ( source_state != AL_PLAYING ) { source = s.first; break; }
		}
		// if no aviable source create new
		if ( source == "" ) { 
			source = "s" + std::to_string ( P_Sources.size( ) );
			Create_Source ( source , {0,0,0}, true ); 
		}
		std::cout << "playing: " << source << " - " << what << '\n';
		
		alSourcei( P_Sources[source], AL_BUFFER, Buffers[what] );
		CHECK_ERROR(FAILED_BINDING);

		alSourcePlay( P_Sources[source] );
		CHECK_ERROR(FAILED_PLAYING);

		std::cout << "p\n";
		
		return SUCCESS;
	}
}


#endif