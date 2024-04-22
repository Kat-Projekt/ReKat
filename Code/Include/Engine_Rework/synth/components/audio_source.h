#ifndef AUDIO_SOUCE_H
#define AUDIO_SOUCE_H

#include "../../objekt.hpp"
#include "../resources/manager.hpp"

class Audio_Source : public Behaviour {
	ALuint source;
	ALuint buffer;
public:
	std::string source_name;
	std::string buffer_name;
	// create context
	void Start ( ) {
		std::cout << "started";
		source = Manager::Source_Get ( source_name )->source;
		buffer = Manager::Buffer_Get ( buffer_name )->buffer;
	}

	void Update ( ) {
		vec3 pos = obj->Get_Pos ();
		alSource3f(source, AL_POSITION, pos.x, pos.y, pos.z );
		int error;
		AL_CHECK_ERROR;
	}

	void Play ( ) {
		int error;
		// std::cout << ( alIsBuffer (buffer) ? "buffer valido\n" : "buffer non valido\n");
		// std::cout << ( alIsSource (source) ? "source valido\n" : "source non valido\n");
		AL_CHECK_ERROR;
		ALint source_state; 
		std::cout << "source " << source << "\nbuffer " << buffer << '\n';
		alGetSourcei ( source, AL_SOURCE_STATE, &source_state );
		if ( source_state == AL_PLAYING ) { return; }
		
		std::cout << ( alIsBuffer (buffer) ? "buffer valido\n" : "buffer non valido\n");
		std::cout << ( alIsSource (source) ? "source valido\n" : "source non valido\n");
		alSourcei( source, AL_BUFFER, buffer );
		AL_CHECK_ERROR;

		alSourcePlay( source );
		AL_CHECK_ERROR;

		if ( false ) { // waiting function neaded
			source_state;
			alGetSourcei ( source, AL_SOURCE_STATE, &source_state );
			AL_CHECK_ERROR;
			while ( source_state == AL_PLAYING ) {
				alGetSourcei( source, AL_SOURCE_STATE, &source_state );
				AL_CHECK_ERROR;
			}
		}
	}

	void Set ( std::string source, std::string buffer ) { source_name = source; buffer_name = buffer;	}
};

#endif