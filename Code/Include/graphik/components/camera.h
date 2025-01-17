#ifndef CAMERA_H
#define CAMERA_H

#include "../../objekt.hpp"
#include "../graphik.hpp"
#include "../graphik_debugger.hpp"

class Camera : public Behaviour {
public:
	float * fb_scale = nullptr;
	float Scale = 1;

	auto Projkection ( ) {
		float X = Bound_Window_Handler->Screen_Ratio;
		if ( fb_scale != nullptr ) 
		{ X = *fb_scale; }
		X *= 500.0f * Scale;
		float Y = 500.0f * Scale;
		vec3 pos = obj->Get_Pos ( );
        return glm::ortho ( pos.x - X, pos.x + X, pos.y - Y, pos.y + Y, -1.0f, 1.0f );
	}

	static auto UI_Projkection ( ) {
        DEBUG ( 5, "Getting_UI_Projkection" );
		float X = Bound_Window_Handler->Screen_Ratio * 500.0f;
        return glm::ortho ( - X, + X, - 500.0f, 500.0f, -1.0f, 1.0f );
	}
};

#endif