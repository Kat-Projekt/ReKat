#ifndef MANAGER
#define MANAGER
#include "text.h"
#include "scene.h"
#include "sprite.h"
#include "shader.h"
#include "object.h"
#include "texture.h"
#include "tilemap.h"
#include "animator.h"
#include "object_UI.h"
#include "graphik.hpp"
#include "collision.h"
#include "framebuffer.h"

#include <map>

namespace Manager {
// rescources
    static std::map < std::string, Text* >     Texts;
    static std::map < std::string, Shader* >   Shaders;
    static std::map < std::string, Sprite* >   Sprites;
    static std::map < std::string, Texture* >  Textures;
    static std::map < std::string, Tilemap* >  Tilemaps;
    static std::map < std::string, Animator* > Animators;
	static std::map < std::string, Framebuffer* > Framebuffers;

// objects
    static std::map < std::string, Object* > Objects;
    static std::map < std::string, UI_Object* > UI_Objects;

// scene management
    static std::map < std::string, Scene* > Scenes;
    Scene* Active_Scene = nullptr;

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
    static int Sprite_Load ( std::string name, std::string shader, Framebuffer * F, glm::vec2 set = {1,1} ) {
        Sprite *s = new Sprite ( Shaders[shader], F, set );
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
        return o;
    }
// UI_Object logic 
    static UI_Object* UI_Object_Get  ( std::string name ) 
    { return UI_Objects [name]; }
    static UI_Object* UI_Object_Load ( std::string name, std::string text, std::string sprite, std::string text_renderer, glm::vec2 pos, glm::vec2 size, void(*click_callback)( ), int start_frame = 0 ) {
        UI_Object *b = new UI_Object ( name, text, Sprites[sprite],Texts[text_renderer], pos, size, click_callback, start_frame );
        UI_Objects.insert( { name, b } );
        return b;
    }
// Framebuffer logic 
    static Framebuffer* Framebuffer_Get  ( std::string name ) 
    { return Framebuffers [name]; }
    static int Framebuffer_Load ( std::string name, int W, int H ) {
        Framebuffer *b = new Framebuffer ( );
        Framebuffers.insert( { name, b } );
        return (*b).Make( W, H );
    }

// collider logic
	static Collider* Add_Collider ( std::string name ) {
		Collider * c = Manager::Object_Get(name)->Add_component<Collider> ( );
		c->Set_Obj(Manager::Object_Get(name));
		c->Start();
		return c;
	}

// Scene logic 
    static Scene* Scene_Get  ( std::string name ) 
    { return Scenes [name]; }
    static Scene* Scene_Load ( std::string name ) {
        Scene *s = new Scene ( );
        Scenes.insert( { name, s } );
        return s;
    }
    static Scene* Set_Active_Scene ( std::string name ) {
        Active_Scene = Scenes[name];
        return Active_Scene;
    }

// UI update logic
    static void Update_Mouse_Position ( ) {
        if ( Active_Scene != nullptr ) { Active_Scene->Update_Mouse_Position( ReKat::grapik::Input::mouse_pos ); }
    }
    static void Update_Mouse_Status ( Scene::Mouse_Status status ) {
        if ( Active_Scene != nullptr ) { Active_Scene->Update_Mouse_Status ( status ); }
    }

	static void Empty ( ) {
		// adds empty values
		Texts.insert ( {"", nullptr} );
    	Shaders.insert ( {"", nullptr} );
    	Sprites.insert ( {"", nullptr} );
    	Textures.insert ( {"", nullptr} );
    	Tilemaps.insert ( {"", nullptr} );
    	Animators.insert ( {"", nullptr} );
	}

    static void Start ( ) {
        if ( Active_Scene != nullptr ) { Active_Scene->Start ( ); }
    }

    static void Update ( ) {
		Collision::Check_collisions ( );
        Timer::Update_Delta_time ( );
        if ( Active_Scene != nullptr ) { Active_Scene->Update ( ); }
        ReKat::grapik::Input::Update();
    }

}; // namespace Manager


#endif