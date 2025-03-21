#ifndef GRAPH_MANAGER
#define GRAPH_MANAGER

#include "../../resource.hpp"

#include "shader.h"
#include "texture.h"
#include "font.h"
#include "tilemap_map.h"
#include "instance.h"
#include "../components/camera.h"

namespace Manager {
	// Gets a Font
	// -------------
    static Font* Font_Get  ( std::string name ) {
		auto ff = Get < Font > ( name );
		if ( !ff ) {
			Font *s = new Font ( 98, 5 );
			_resources.insert( { (name + std::string(typeid(Font).name())) , s } );
			(*s).Make( name.c_str ( ) );
			return s;
		}
		return ff;
	}
    static int Font_Load ( std::string name, const char* fontPath, int heigth = 48, int letters_spacing = 0 ) {
        Font *s = new Font ( heigth, letters_spacing );
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
    static int Shader_Load ( std::string name, const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr, const char* tessControlPath = nullptr, const char* tessEvalPath = nullptr, bool skip_reading = false ) {
        Shader *s = new Shader ( );
        _resources.insert( { (name + std::string(typeid(Shader).name())) , s } );
        return ( skip_reading 
		? (*s).Make( true, vertexPath, fragmentPath, geometryPath, tessControlPath, tessEvalPath )
		: (*s).Make( vertexPath, fragmentPath, geometryPath, tessControlPath, tessEvalPath ) );
    }

	// Gets a Texture
	// --------------
    static Texture* Texture_Get  ( std::string name ) { 
		if ( name == "" ) { return nullptr; }
		auto findit = _resources.find(name + std::string(typeid(Texture).name()));
		if ( findit != _resources.end() ) {
			return (Texture*)findit->second;
		} else { 
			// try to create texture
			DEBUG ( 3, "Tring to create a texture" );
			Texture *t = new Texture ( (unsigned int)0 );
			if ( (*t).Make (name.c_str( ) ) ) {
				DEBUG (2, "cannot create texture" );
				return nullptr;
			}
			_resources.insert( { (name + std::string(typeid(Texture).name())) , t} );
			return t;
			// on fail raise error
		}
	}
    static int Texture_Load ( std::string name, const char* path, unsigned int Texture_Number = 0 ) {
        Texture *t = new Texture ( (unsigned int)0, Texture_Number );
        _resources.insert( { (name + std::string(typeid(Texture).name())) , t} );
        return (*t).Make(path);
    }
	static void Texture_Load ( std::string name, Texture* texture ) {
        _resources.insert( { (name + std::string(typeid(Texture).name())) , texture} );
    }

	// Gets a Tilemap
	// --------------
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

	// Gets a Instance
	// ---------------
    static int Instance_Load ( std::string name, int data_size, int chunk_size = 16 ) {
        Instance *t = new Instance;
        _resources.insert( { (name + std::string(typeid(Instance).name())) , t} );
        return (*t).Make(data_size, chunk_size);
    }
	static void Instance_Load ( std::string name, Instance * Jonh ) {
        _resources.insert( { (name + std::string(typeid(Instance).name())) , Jonh} );
    }

	// Gets a Camera
	// -------------
    static std::unordered_map < std::string, Camera* > cameras;
    static int Camera_Load ( std::string name, std::string pointer ) {
		if ( pointer == "" ) 
		{ DEBUG ( 1, "Invalid camera pointer" ); return 1; }
		auto C = Manager::Objekt_Get ( pointer )->Add_Component < Camera > ( );
		cameras.insert ( {name, C} );
		return 0;
	}
	static int Camera_Load ( std::string name, Objekt* pointer ) {
		if ( pointer == nullptr ) 
		{ DEBUG ( 1, "Invalid camera pointer" ); return 1; }
		auto C = pointer->Add_Component < Camera > ( );
		cameras.insert ( {name, C} );
		return 0;
	}
	static Camera * Camera_Get ( std::string name ) {
		if ( name == "" ) { return nullptr; }
		auto findit = cameras.find(name);
		if ( findit != cameras.end() ) {
			return findit->second;
		} else { return nullptr; }
	}
}; // namespace Manager

#endif