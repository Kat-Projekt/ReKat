#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "../objekt.h"
#include "../graphik.hpp"

class Rigidbody : public Behaviour {
public:
	vec3 acceleration;
	vec3 velocity;
	float mass = 1;

	void Fixed_Update ( ) {
		velocity += acceleration * Timer::fixed_delta_time;
		obj->Inc_Pos ( velocity * Timer::fixed_delta_time );
	}
};

#endif