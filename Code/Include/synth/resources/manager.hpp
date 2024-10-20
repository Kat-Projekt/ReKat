#ifndef AUDIO_MANAGER
#define AUDIO_MANAGER

#include "buffer.h"
#include "source.h"

#include <unordered_map>

namespace Manager {
    static std::unordered_map < std::string, Buffer* > _buffers;
    static std::unordered_map < std::string, Source* >  _sources;

	// Gets a buffer
	// -------------
	static Buffer* Buffer_Get  ( std::string name ) {
		if ( name == "" ) { return nullptr; }
		auto findit = _buffers.find(name);
		if ( findit != _buffers.end( ) ) {
			return findit->second;
		} else { return nullptr; }
	}
	static int Buffer_Load ( std::string name, std::string path ) {
		Buffer *s = new Buffer ( );
		_buffers.insert( { name, s } );
		return (*s).Make( path );
	}
	static int Buffer_Load ( std::string name, double * buf , long len ) {
		Buffer *s = new Buffer ( );
		_buffers.insert( { name, s } );
		return (*s).Make( buf, len );
	}
	static int Buffer_Load ( std::string name, short * buf , long len ) {
		Buffer *s = new Buffer ( );
		_buffers.insert( { name, s } );
		return (*s).Make( buf, len );
	}

	// Gets a source
	// -------------
	static Source* Source_Get  ( std::string name ) {
		if ( name == "" ) { return nullptr; }
		auto findit = _sources.find(name);
		if ( findit != _sources.end() ) {
			return findit->second;
		} else { return nullptr; }
	}
	static int Source_Load ( std::string name, glm::vec3 pos = {0,0,0} ) {
		Source *s = new Source ( );
		_sources.insert( { name, s } );
		return (*s).Make( pos );
	}

	// Makes the resources disapear
	// ----------------------------
	static void Free_Audio ( ) {
		color ( "FREEING AUDIO\n", BACKGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY );
		for ( auto F : _buffers ) { F.second->End(); }
		for ( auto S : _sources ) { S.second->End(); }
	}
}; // namespace Manager

#endif