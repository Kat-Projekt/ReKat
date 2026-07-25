#ifndef CAMERA_H
#define CAMERA_H

#include "../../../objekt/objekt.hpp"
#include "../graphik.hpp"
#include "../graphik_debugger.hpp"

class Camera : public Behaviour {
public:
	float * fb_scale = nullptr;
	float Scale = 1;
	float fov = 90;

	virtual glm::mat4 Projkection ( );
	virtual glm::mat4 Prospective ( );
	static glm::mat4 UI_Projkection ( );
};

#endif