#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <objekt/objekt.hpp>
#include "../timer.hpp"

class Rigidbody : public Behaviour {
public:
	vec3 acceleration = {0,0,0};
	vec3 velocity = {0,0,0};
	float mass = 1;
	float time_scale = 1;
	float bouncy = 0.7f;
	bool block_z = true;

	Rigidbody ( )
	{
		Informations = { "Rigidbody", 1.0, "The rigidbody controller for movent and collision resolution" };
	}

	void Fixed_Update ( ) {
		// implement verlet integration
		// x_{n+1} = 2 x_n - x_{n+1} + acc_n * dt
		if ( block_z ) { velocity.z = 0; acceleration.z = 0;}
		if ( acceleration != vec3{0,0,0} )
		{ velocity += acceleration * Timer::Get_Delta ( ) * time_scale; }
		obj->Inc_Pos ( velocity * Timer::Get_Delta ( ) * time_scale );
	}

	void Vincolar_Reaction ( vec3 plane_normal ) {
		acceleration -= plane_normal * ( dot ( plane_normal, acceleration ) ) * ( 1 + bouncy );
		velocity -= plane_normal * ( dot ( plane_normal, velocity ) ) * ( 1 + bouncy );
	}
};

#endif