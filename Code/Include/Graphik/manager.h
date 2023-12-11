#ifndef MANAGER
#define MANAGER
#include "text.h"
#include "sprite.h"
#include "shader.h"
#include "texture.h"
#include "tilemap.h"
#include "animator.h"
#include "object.h"
#include "button.h"
#include "scene.h"
#include "collision.h"
#include "graphik.hpp"

#include <map>

namespace Manager {
// rescources
    static std::map < std::string, Text* >     Texts;
    static std::map < std::string, Scene* >   Scenes;
    static std::map < std::string, Shader* >   Shaders;
    static std::map < std::string, Sprite* >   Sprites;
    static std::map < std::string, Texture* >  Textures;
    static std::map < std::string, Tilemap* >  Tilemaps;
    static std::map < std::string, Animator* > Animators;

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
    static int Draw_Text ( std::string name,std::string text, glm::vec2 pos, glm::vec2 dim, float scale, glm::vec4 color = {0, 0, 0, 1}, 
                           int start_rows = 0, int wrap_h = -1, Text::Text_guistifiaction text_guistifiaction = Text::LEFT ) {
        return Text_Get(name)->RenderText( text, { pos.x * ReKat::grapik::Internal::SCR_WIDTH / ReKat::grapik::Internal::SCR_HEIGTH, pos.y }, 
                                                 { dim.x * ReKat::grapik::Internal::SCR_WIDTH / ReKat::grapik::Internal::SCR_HEIGTH, dim.y }, 
                                                 scale, color, start_rows, wrap_h, text_guistifiaction );
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
// Animator logic 
    static Animator* Animator_Get  ( std::string name ) 
    { return Animators [name]; }
    static int Animator_Load ( std::string name ) {
        Animator *a = new Animator ( );
        Animators.insert( { name, a } );
        return 0;
    }

// Object logic 
    static Object* Object_Get  ( std::string name ) 
    { return Objects [name]; }
    static Object* Object_Load ( std::string name, std::string sprite, glm::vec2 pos, glm::vec2 size  ) {
        Object *o = new Object ( name, Sprites[sprite], pos, size );
        Objects.insert( { name, o } );
        Collision::Add_Object( o );
        return o;
    }
// Button logic 
    static Button* Button_Get  ( std::string name ) 
    { return Buttons [name]; }
    static Button* Button_Load ( std::string name, std::string text, std::string sprite, glm::vec2 pos, glm::vec2 size, void(*click_callback)( ), int start_frame = 0 ) {
        Button *b = new Button ( name, text, Sprites[sprite], pos, size, click_callback, start_frame );
        Buttons.insert( { name, b } );
        return b;
    }

// Scene logic 
    static Scene* Scene_Get  ( std::string name ) 
    { return Scenes [name]; }
    static Scene* Scene_Load ( std::string name ) {
        Scene *s = new Scene ( );
        Scenes.insert( { name, s } );
        return s;
    }

// draw logic
    static void Draw ( ) {
        for ( auto o : Objects ) 
        { o.second->Draw ( ); }
        for ( auto b : Buttons ) 
        { b.second->Draw ( ); }
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

        ReKat::grapik::Input::Update();
        Collision::Check_collisons( );
    }

}; // namespace Manager


#endif