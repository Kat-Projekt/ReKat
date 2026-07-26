#ifndef AUDIO_SOUCE_H
#define AUDIO_SOUCE_H

#include "objekt/objekt.hpp"
#include "../resources/manager.hpp"

class Audio_Source : public Behaviour {
	std::shared_ptr < Source > source = nullptr;
	List < std::shared_ptr < Buffer > > buffers;
public:
	// create context
	void Start ( ) {
		DEBUG ( 4, "Audio Source Component Started" );
	}

	void Update ( ) {
		if ( source != nullptr ) 
		{ source->Move ( obj->Get_Pos ( ) ); }
		DEBUG ( 5, "Updated Audio Source Position");
	}

	void Play ( int what )
	{		
		if ( source->Is_Busy ( ) )
		{ DEBUG (3, "Trying to play a busy source"); return; }
		if ( what >= buffers.size( ) )
		{ DEBUG (3, "Trying to play an non existing buffer"); return; }

		alSourcei( source->Get_Source ( ), AL_BUFFER, buffers[what]->Get_Buffer ( ) ); AL_CHECK_ERROR;
		alSourcePlay( source->Get_Source ( ) ); AL_CHECK_ERROR;
	}

	Audio_Source* Set ( std::string _source, std::string _buffer ) 
	{
		source = Manager::Get < Source >  ( _source );
		buffers.append ( Manager::Get < Buffer > ( _buffer ) );
		return this;
	}

	Audio_Source* Set ( std::string _buffer ) 
	{
		buffers.append ( Manager::Get < Buffer > ( _buffer ) );
		return this;
	}
};

#endif