#ifndef _camera_controll
#define _camera_controll

#include <Graphik/objekt.h>
#include <Graphik/graphik.hpp>
using namespace ReKat::grapik::Input;

class Camera_Controll : public Behaviour {
public:
	Objekt * _target;
	float _speed = 200;
    virtual void Start ( ) { }
    virtual void Update ( ) {
		obj->Get_component < Camera > ()->Scale = (abs(scrool_pos) + 1) * 0.1;
		obj->Set_Pos ( lerp ( obj->Get_Pos( ), _target->Get_Pos( ), Timer::delta_time) );
	}
};

#endif