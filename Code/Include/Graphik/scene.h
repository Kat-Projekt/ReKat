#ifndef SCENE_H
#define SCENE_H
#include "object.h"
#include "object_UI.h"
#include "button.h"
#include "camera.h"
#include "manager.h"
#include "tilemap.h"

class Scene {
private:
    std::vector < Object* > Objects;
    std::vector < UI_Object* > UI_Objects;
    std::vector < Shader* > Shaders;
    std::vector < Shader* > UI_Shaders;
    std::vector < Shader* > UI_Sprite_Shaders;
    std::vector < Tilemap* > Tilemaps;
public:
    Camera cam;

    Scene ( ) { }

    void Add_Object    ( Object* _obj )        { Objects.push_back(_obj); }
    void Add_UI_Object ( UI_Object* _obj )     { UI_Objects.push_back(_obj); }
    void Add_Shader    ( Shader* _obj )        { Shaders.push_back(_obj); }
    void Add_UI_Shader ( Shader* _obj )        { UI_Shaders.push_back(_obj); }
    void Add_UI_Sprite_Shader ( Shader* _obj ) { UI_Sprite_Shaders.push_back(_obj); }
    void Add_Tilemap   ( Tilemap* _obj )       { Tilemaps.push_back(_obj); }

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
        for ( auto o : Objects ) 
        { o->Start(); }
    }
    void Update ( ) {
        glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        // Update camera projection
        for ( auto s : Shaders ) 
        { s->setMat4( "projection", cam.Get_Wiew() ); }
        for ( auto s : UI_Shaders ) 
        { s->setMat4( "projection", cam.Get_UI_Wiew() ); }
        for ( auto s : UI_Sprite_Shaders ) 
        { s->setMat4( "projection", cam.Get_UI_Wiew() ); }

        // Update objects
        for ( auto o : Objects ) 
        { o->Draw(); 
          o->Update(); }
        for ( auto o : UI_Objects ) 
        { o->Draw(); }
        
        ReKat::grapik::Input::Update();
    }
};

#endif