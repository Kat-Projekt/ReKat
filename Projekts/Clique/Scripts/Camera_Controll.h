#ifndef _Camera_Controll
#define _Camera_Controll

#include <Graphik/objekt.h>
#include <Graphik/graphik.hpp>
using namespace ReKat::grapik::Input;

class Camera_Controll : public Behaviour {
public:
	Objekt * target;
    virtual void Update ( ) {
		obj->Get_Component < Camera > ()->Scale = ( abs ( scrool_pos ) + 10) * 0.1;
		obj->Set_Pos ( lerp ( obj->Get_Pos( ), target->Get_Pos( ), Timer::delta_time) );
	}
};

#endif