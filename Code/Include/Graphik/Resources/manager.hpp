#ifndef MANAGER
#define MANAGER

#include "shader.h"
#include "texture.h"
#include "font.h"

#include <unordered_map>
#include <iostream>

namespace Manager {
    static std::unordered_map < std::string, Font* > _fonts;
    static std::unordered_map < std::string, Shader* >  _shaders;
    static std::unordered_map < std::string, Texture* > _textures;

	// Gets a Font
	// -------------
    static Font* Font_Get  ( std::string name ) {
		if ( name == "" ) { return nullptr; }
		auto findit = _fonts.find(name);
		if ( findit != _fonts.end() ) {
			return findit->second;
		} else { return nullptr; }
	}
    static int Font_Load ( std::string name, const char* fontPath, int heigth = 48 ) {
        Font *s = new Font ( heigth );
        _fonts.insert( { name, s } );
        return (*s).Make( fontPath );
    }

	// Gets a Shader
	// -------------
    static Shader* Shader_Get  ( std::string name ) {
		if ( name == "" ) { return nullptr; }
		auto findit = _shaders.find(name);
		if ( findit != _shaders.end() ) {
			return findit->second;
		} else { return nullptr; }
	}
    static int Shader_Load ( std::string name, const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr, const char* tessControlPath = nullptr, const char* tessEvalPath = nullptr ) {
        Shader *s = new Shader ( );
        _shaders.insert( { name, s } );
        return (*s).Make( vertexPath, fragmentPath, geometryPath, tessControlPath, tessEvalPath );
    }

	// Gets a Texture
	// --------------
    static Texture* Texture_Get  ( std::string name ) { 
		if ( name == "" ) { return nullptr; }
		auto findit = _textures.find(name);
		if ( findit != _textures.end() ) {
			return findit->second;
		} else { return nullptr; }
	}
    static int Texture_Load ( std::string name, const char* path ) {
        Texture *t = new Texture ( );
        _textures.insert( { name, t} );
        return (*t).Make(path);
    }
	static void Texture_Load ( std::string name, Texture* texture ) {
        _textures.insert( { name, texture} );
    }

	// Makes the resources disapear
	// ----------------------------
	static void Free ( ) {
		for ( auto F : _fonts    ) { F.second->End(); }
		for ( auto S : _shaders  ) { S.second->End(); }
		for ( auto T : _textures ) { T.second->End(); }
	}
}; // namespace Manager


#endif