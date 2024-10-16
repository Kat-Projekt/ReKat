#ifndef CAMERA_H
#define CAMERA_H

#include "../../objekt.hpp"
#include "../graphik.hpp"
#include "../graphik_debugger.hpp"

class Camera : public Behaviour {
public:
	float Scale = 1;

	auto Projkection ( ) {
		float X = ReKat::grapik::Input::screen_ration * 500.0f * Scale;
		float Y = 500.0f * Scale;
		vec3 pos = obj->Get_Pos ( );
        return glm::ortho ( pos.x - X, pos.x + X, pos.y - Y, pos.y + Y, -1.0f, 1.0f );
	}

	static auto UI_Projkection ( ) {
        DEBUG ( 5, "Getting_UI_Projkection" );
		float X = ReKat::grapik::Input::screen_ration * 500.0f;
        return glm::ortho ( - X, + X, - 500.0f, 500.0f, -1.0f, 1.0f );
	}
};

#endif