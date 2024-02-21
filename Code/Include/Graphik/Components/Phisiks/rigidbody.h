#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "../objekt.h"
#include "../graphik.hpp"

class Rigidbody : public Behaviour {
public:
	vec3 acceleration = {0,0,0};
	vec3 velocity = {0,0,0};
	float mass = 1;
	float time_scale = 1;
	float bouncy = 0.7;
	bool block_z = true;

	void Fixed_Update ( ) {
		if ( block_z ) { velocity.z = 0; acceleration.z = 0;}
		if ( acceleration != vec3{0,0,0} ) { velocity += acceleration * Timer::fixed_delta_time * time_scale; }
		obj->Inc_Pos ( velocity * Timer::fixed_delta_time * time_scale );
	}

	void Vincolar_Reaction ( vec3 plane_normal ) {
		//std::cout << " normal: " << plane_normal << " vel: " << velocity << " acc: " << acceleration << '\n';
		acceleration -= plane_normal * ( dot ( plane_normal, acceleration ) ) * ( 1 + bouncy );
		velocity -= plane_normal * ( dot ( plane_normal, velocity ) ) * ( 1 + bouncy );
		//std::cout << " normal: " << plane_normal << " vel: " << velocity << " acc: " << acceleration << '\n';
	}
};

#endif