#include "extensions/graphik/components/camera.h"

glm::mat4 Camera::Projkection
( ) {
	float X = ReKat::Graphik::_current_window->Screen_Ratio;
	if ( fb_scale != nullptr ) 
	{ X = *fb_scale; }
	X *= 500.0f * Scale;
	float Y = 500.0f * Scale;
	vec3 pos = obj->Get_Pos ( );
	return glm::ortho ( pos.x - X, pos.x + X, pos.y - Y, pos.y + Y, -1.0f, 1.0f );
}

glm::mat4 Camera::Prospective
( ) {
	DEBUG ( 1, "FUNCTION CAMERA::PROSPECTIVE NOT IMPLEMENTED" );
	vec3 Front; // the rotated [1,0,0] by the quaternion
	vec3 Up;    // the rotated [0,0,1] by the quaternion
	return glm::lookAt( obj->Get_Pos ( ), obj->Get_Pos ( ) + Front, Up);
}

glm::mat4 Camera::UI_Projkection
( ) {
	DEBUG ( 5, "Getting_UI_Projkection" );
	float X = ReKat::Graphik::_current_window->Screen_Ratio * 500.0f;
	// std::cout << X << '\n';
	return glm::ortho ( - X, + X, - 500.0f, 500.0f, -1000.0f, 1000.0f );
}