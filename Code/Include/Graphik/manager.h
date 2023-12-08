#ifndef MANAGER
#define MANAGER
#include "text.h"
#include "sprite.h"
#include "shader.h"
#include "texture.h"
#include "tilemap.h"
#include "object.h"
#include "button.h"

#include <map>

namespace Manager {
// rescources
    static std::map < std::string, Text* >    Texts;
    static std::map < std::string, Shader* >  Shaders;
    static std::map < std::string, Sprite* >  Sprites;
    static std::map < std::string, Texture* > Textures;
    static std::map < std::string, Tilemap* > Tilemaps;

// objects
    static std::map < std::string, Object* > Objects;
    static std::map < std::string, Button* > Buttons;

// Shader logic
    static Shader* Shader_Get  ( std::string name ) 
    { return Shaders [name]; }
    static int Shader_Load ( std::string name, const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr, const char* tessControlPath = nullptr, const char* tessEvalPath = nullptr ) {
        Shader *s = new Shader ( );
        Shaders.insert( { name, s } );
        return (*s).Make( vertexPath, fragmentPath, geometryPath, tessControlPath, tessEvalPath );
    }
// Texture logic
    static Texture* Texture_Get  ( std::string name ) 
    { return Textures [name]; }
    static int Texture_Load ( std::string name, const char* path ) {
        Texture *t = new Texture ( );
        Textures.insert( { name, t} );
        return (*t).Make(path);
    }
// Text logic
    static Text* Text_Get  ( std::string name ) 
    { return Texts [name]; }
    static int Text_Load ( std::string name, const char* path, std::string shader ) {
        Text *t = new Text ( );
        Texts.insert( { name, t } );
        return (*t).Make(path, Shaders[shader]);
    }
// Sprite logic 
    static Sprite* Sprite_Get  ( std::string name ) 
    { return Sprites [name]; }
    static int Sprite_Load ( std::string name, std::string shader, std::string texture, glm::vec2 set = {1,1} ) {
        Sprite *s = new Sprite ( Shaders[shader], Textures[texture], set );
        Sprites.insert( { name, s } );
        return 0;
    }
// Tilemap logic 
    static Tilemap* Tilemap_Get  ( std::string name ) 
    { return Tilemaps [name]; }
    static int Tilemap_Load ( std::string name, std::string sprite, std::string path ) {
        Tilemap *t = new Tilemap ( Sprites[sprite] );
        Tilemaps.insert( { name, t } );
        return (*t).Make ( path );
    }
// Object logic 
    static Object* Object_Get  ( std::string name ) 
    { return Objects [name]; }
    static int Object_Load ( std::string name, std::string sprite, glm::vec2 pos, glm::vec2 size  ) {
        Object *o = new Object ( name, Sprites[sprite], pos, size );
        Objects.insert( { name, o } );
        return 0;
    }
// Button logic 
    static Button* Button_Get  ( std::string name ) 
    { return Buttons [name]; }
    static int Buttton_Load ( std::string name, std::string text, std::string sprite, glm::vec2 pos, glm::vec2 size, void(*click_callback)( ), int start_frame = 0 ) {
        Button *b = new Button ( name, text, Sprites[sprite], pos, size, click_callback, start_frame );
        Buttons.insert( { name, b } );
        return 0;
    }

// draw logic
    static void Draw ( ) {
        for ( auto o : Objects ) 
        { o.second->Draw ( ); }
       // for ( auto b : Buttons ) 
       // { b.second->Draw ( ); }
    }

// UI update logic
    enum Mouse_Status { Pressed, Release };
    static void Update_Mouse_Position ( glm::vec2 pos ) {
        for ( auto b : Buttons ) 
        { b.second->Update_mause ( pos ); }
    }
    static void Update_Mouse_Status ( Mouse_Status status ) {
        switch ( status ) {
        case Pressed:
            for ( auto b : Buttons ) 
            { b.second->Pressed ( ); }
            break;
        case Release:
            for ( auto b : Buttons ) 
            { b.second->Release ( ); }
            break;
        }
    }

    static void Start ( ) {
        for ( auto o : Objects ) 
        { o.second->Start(); }
    }

    static void Update ( ) {
        for ( auto o : Objects ) 
        { o.second->Update(); }
    }

}; // namespace Manager


#endif