#ifndef AUDIO_SOUCE_H
#define AUDIO_SOUCE_H

#include "../../objekt.hpp"
#include "../resources/manager.hpp"

class Audio_Source : public Behaviour {
	Source* source;
	List < Buffer* > buffers;
public:
	// create context
	void Start ( ) {
		DEBUG ( 4, "Audio Source Component Started" );
	}

	void Update ( ) {
		source->Move ( obj->Get_Pos ( ) );
		DEBUG ( 5, "Updated Audio Source Position");
	}

	void Play ( int what ) {		
		if ( source->Is_Busy ( ) ) { DEBUG (3, "Trying to play a busy source"); return; }
		
		alSourcei( source->Get_Source ( ), AL_BUFFER, buffers[what]->Get_Buffer ( ) ); AL_CHECK_ERROR;
		alSourcePlay( source->Get_Source ( ) ); AL_CHECK_ERROR;
	}

	Audio_Source* Set ( Source* _source, Buffer* _buffer )
	{ source = _source; buffers.append ( _buffer ); return this; }
	Audio_Source* Set ( std::string _source, std::string _buffer ) 
	{ source = Manager::Source_Get ( _source ); buffers.append ( Manager::Buffer_Get ( _buffer ) ); return this; }

	Audio_Source* Set ( Buffer* _buffer ) 
	{ buffers.append ( _buffer ); return this; }
	Audio_Source* Set ( std::string _buffer ) 
	{ buffers.append ( Manager::Buffer_Get ( _buffer ) ); return this; }
};

#endif