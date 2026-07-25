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
<<<<<<< HEAD
	virtual glm::mat4 Prospective ( );
=======
	glm::mat4 Prospective ( );
>>>>>>> ee20bdd (fixed compilation issue in openal again, fixed compilation error in camera.h and fixed segfault in sprite.cpp)
	static glm::mat4 UI_Projkection ( );
};

#endif