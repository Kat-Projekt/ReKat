#pragma once
#include "../global.h"
using namespace ReKat::grapik::Input;

class _Camera : public Behaviour {
private:
	float smooth = 3.1415;
	float seconds = 1 / smooth;
	float progress = 0;
	vec2 point = {0,0};
public:
	Object *Pointer;

    void Update ( ) {
		vec2 Start_pos = Pointer->Get_pos();
		vec2 New_pos = Start_pos + point * ( Manager::Active_Scene->cam.Get_Scale ( ) * 0.7f );

		if ( Key_Pressed ( "Mouse3" ) ) {
			point = mouse_pos;
			progress += ( progress >= seconds ? 0 : Timer::delta_time ); 
		} else { progress -= ( progress <= 0 ? 0 : Timer::delta_time); }
		
		vec2 Lerp_pos =  Qerp ( Start_pos, New_pos , smooth * progress );

		Manager::Active_Scene->cam.Move ( Lerp_pos );
	}
};