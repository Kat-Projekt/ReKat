#ifndef SCENE_H
#define SCENE_H
#include "object.h"
#include "object_UI.h"
#include "button.h"
#include "camera.h"
#include "manager.h"
#include "tilemap.h"
#include "graphik.hpp"

template < typename T >
void remove_element ( std::vector<T>&vec, T &element ) {
	// find
    int it = -1;
    for (size_t i = 0; i < vec.size(); i++) { 
        if ( vec[i] == element ) 
        { it = i; } 
    }
    if ( it < 0 ) { return; }
	// remove
	vec.erase ( vec.begin() + it );
}

class Scene {
private:
    std::vector < Object* > Objects;
    std::vector < UI_Object* > UI_Objects;
    std::vector < Shader* > Shaders;
    std::vector < Shader* > UI_Shaders;
    std::vector < Tilemap* > Tilemaps;
public:
    Camera cam;
	glm::vec2 world_mouse_pos;

    Scene ( ) { }

    Scene* Add_Object    ( Object* _obj )        { Objects.push_back(_obj); return this; }
    Scene* Rem_Object	 ( Object* _obj )		 { remove_element ( Objects, _obj ); return this;}
	Scene* Add_UI_Object ( UI_Object* _obj )     { UI_Objects.push_back(_obj); return this; }
    Scene* Add_Shader    ( Shader* _obj )        { Shaders.push_back(_obj); return this; }
    Scene* Add_UI_Shader ( Shader* _obj )        { UI_Shaders.push_back(_obj); return this; }
    Scene* Add_Tilemap   ( Tilemap* _obj )       { Tilemaps.push_back(_obj); return this; }

    enum Mouse_Status { Pressed, Release };
    void Update_Mouse_Position ( glm::vec2 pos ) {
        for ( auto b : UI_Objects ) 
        { b->Update_mause_pos ( pos ); }
    }
    void Update_Mouse_Status ( Mouse_Status status ) {
        for ( auto b : UI_Objects ) 
        { b->Update_mause_action ( status ); }
    }

    void Start ( ) {
		std::cout << "stating scene: ";
		Objects.reserve(100);
        for ( auto o : Objects ) 
        { o->Start(); }
        for ( auto o : UI_Objects ) 
        { o->Start(); }
    }

    void Update ( ) {		
		world_mouse_pos = cam.Get_Pos() + ReKat::grapik::Input::mouse_pos;

        glClearColor(0.6, 0.4, 0.5, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
        // Update camera projection
        for ( auto s : Shaders ) 
        { s->setMat4( "projection", cam.Get_Wiew() ); }
        for ( auto s : UI_Shaders ) 
        { s->setMat4( "projection", cam.Get_UI_Wiew() ); }

        // Update objects
        for ( auto o : Objects ) {
			if ( o == nullptr ) { continue; }
			o->Draw(); 
			o->Update(); 
		}
        for ( auto o : UI_Objects ) 
        { o->Draw(); o->Update(); }
		glDisable(GL_DEPTH_TEST);
    }
};

#endif