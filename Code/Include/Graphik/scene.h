#ifndef SCENE_H
#define SCENE_H
#include "object.h"
#include "button.h"
#include "camera.h"
#include "manager.h"

class Scene {
private:
    Camera cam;
    std::vector < Object* > Objects;
public:
    Scene ( ) { }

    void Add_object ( Object* _obj ) { Objects.push_back(_obj); }
    void Start ( ) {
        for ( auto o : Objects ) 
        { o->Start(); }
    }
    void Update ( ) {
        for ( auto o : Objects ) 
        { o->Draw(); 
          o->Update(); }
    }
};

#endif