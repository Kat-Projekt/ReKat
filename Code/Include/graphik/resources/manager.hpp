#ifndef GRAPH_MANAGER
#define GRAPH_MANAGER

#include "../../resource.hpp"

#include "shader.h"
#include "texture.h"
#include "font.h"
#include "tilemap_map.h"

namespace Manager {
	// Gets a Font
	// -------------
    static Font* Font_Get  ( std::string name )
	{ return Get < Font > ( name ); }
    static int Font_Load ( std::string name, const char* fontPath, int heigth = 48 ) {
        Font *s = new Font ( heigth );
        _resources.insert( { (name + std::string(typeid(Font).name())) , s } );
        return (*s).Make( fontPath );
    }

	// Gets a Shader
	// -------------
    static Shader* Shader_Get  ( std::string name ) {
		if ( name == "" ) { return nullptr; }
		auto findit = _resources.find(name + std::string(typeid(Shader).name()));
		if ( findit != _resources.end() ) {
			return (Shader*)findit->second;
		} else { return nullptr; }
	}
    static int Shader_Load ( std::string name, const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr, const char* tessControlPath = nullptr, const char* tessEvalPath = nullptr ) {
        Shader *s = new Shader ( );
        _resources.insert( { (name + std::string(typeid(Shader).name())) , s } );
        return (*s).Make( vertexPath, fragmentPath, geometryPath, tessControlPath, tessEvalPath );
    }

	// Gets a Texture
	// --------------
    static Texture* Texture_Get  ( std::string name ) { 
		if ( name == "" ) { return nullptr; }
		auto findit = _resources.find(name + std::string(typeid(Texture).name()));
		if ( findit != _resources.end() ) {
			return (Texture*)findit->second;
		} else { return nullptr; }
	}
    static int Texture_Load ( std::string name, const char* path ) {
        Texture *t = new Texture ( );
        _resources.insert( { (name + std::string(typeid(Texture).name())) , t} );
        return (*t).Make(path);
    }
	static void Texture_Load ( std::string name, Texture* texture ) {
        _resources.insert( { (name + std::string(typeid(Texture).name())) , texture} );
    }

    static Tilemap_Map* Tilemap_Get  ( std::string name ) { 
		if ( name == "" ) { return nullptr; }
		auto findit = _resources.find(name + std::string(typeid(Tilemap_Map).name()));
		if ( findit != _resources.end() ) {
			return (Tilemap_Map*)findit->second;
		} else { return nullptr; }
	}
    static int Tilemap_Load ( std::string name, const char * path ) {
        Tilemap_Map *t = new Tilemap_Map;
        _resources.insert( { (name + std::string(typeid(Texture).name())) , t} );
        return (*t).Make(path);
    }
}; // namespace Manager


#endif