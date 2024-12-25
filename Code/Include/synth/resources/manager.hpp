#ifndef AUDIO_MANAGER
#define AUDIO_MANAGER

#include "buffer.h"
#include "source.h"

namespace Manager {
	// Gets a buffer
	// -------------
	static Buffer* Buffer_Get  ( std::string name )
	{ return Get < Buffer > ( name ); }
	static int Buffer_Load ( std::string name, std::string path ) {
		Buffer *s = new Buffer ( );
		_resources.insert( { (name + std::string(typeid(Buffer).name())), s } );
		return (*s).Make( path );
	}
	static int Buffer_Load ( std::string name, double * buf , long len ) {
		Buffer *s = new Buffer ( );
		_resources.insert( { (name + std::string(typeid(Buffer).name())), s } );
		return (*s).Make( buf, len );
	}
	static int Buffer_Load ( std::string name, short * buf , long len ) {
		Buffer *s = new Buffer ( );
		_resources.insert( { (name + std::string(typeid(Buffer).name())), s } );
		return (*s).Make( buf, len );
	}

	// Gets a source
	// -------------
	static Source* Source_Get  ( std::string name )
	{ return Get < Source > ( name ); }
	static int Source_Load ( std::string name, glm::vec3 pos = {0,0,0} ) {
		Source *s = new Source ( );
		_resources.insert( { (name + std::string(typeid(Source).name())), s } );
		return (*s).Make( pos );
	}
}; // namespace Manager

#endif