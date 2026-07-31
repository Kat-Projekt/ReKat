#pragma once

#include "collision_result.hpp"
#include "../components/box_collider.hpp"
#include "../components/sphere_collider.hpp"

collision_result Check_Collision ( Box_Collider   *B1, Box_Collider   *B2 ) {
	vec3 P_Delta = B1->Get_Pos ( ) - B2->Get_Pos ( );
	vec3 Delta = abs ( P_Delta ) - ( B1->Get_Size () + B2->Get_Size() ) * 0.5f;

	// collision not appeing
	if ( Delta.x > 0 || Delta.y > 0 || Delta.z > 0 ) { return {false,{0,0,0}}; }

	// trigger collision ( no nead for fastest exit point )
	if ( B1->Is_Trigger( ) || B2->Is_Trigger ( ) ) { return {true,{0,0,0}}; }
		
	glm::vec3 move_vector;

	// determine exit point
	if ( abs ( Delta.x ) > abs ( Delta.y ) ) {
		if ( abs ( Delta.y ) > abs ( Delta.z ) ) { // Delta.z is Low
			move_vector = { 0, 0, Delta.z * ( P_Delta.z >= 0 ? -1 : 1 ) };
		} else { // Delta.y is Low
			move_vector = { 0, Delta.y * ( P_Delta.y >= 0 ? -1 : 1 ), 0 };
		}
	} else {
		if ( abs ( Delta.x ) > abs ( Delta.z ) ) { // Delta.z is Low
			move_vector = { 0, 0, Delta.z * ( P_Delta.z >= 0 ? -1 : 1 ) };
		} else { // Delta.x is Max
			move_vector = { Delta.x * ( P_Delta.x >= 0 ? -1 : 1 ), 0, 0 };
		}
	}

	return {true,move_vector};
}
collision_result Check_Collision ( Box_Collider   *B1, Sphere_Collider *S2 ) {
	// get center point circle first 
    glm::vec3 center = S2->Get_Pos ( );
    // calculate AABB info (center, half-extents)
    glm::vec3 aabb_half_extents = B1->Get_Size ( ) * 0.5f;
    glm::vec3 aabb_center = B1->Get_Pos ( );
    // get difference vector between both centers
    glm::vec3 difference = center - aabb_center;
    glm::vec3 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // add clamped value to AABB_center and we get the value of box closest to circle
    glm::vec3 closest = aabb_center + clamped;
    // retrieve vector between center circle and closest point AABB and check if length <= radius
    difference = closest - center;

	float D = glm::length(difference);
	// collision not appened
	if ( D > S2->Get_Size ( ) ) { return {false,{0,0,0}}; }

	// trigger collision ( no nead for fastest exit point )
	if ( B1->Is_Trigger( ) || S2->Is_Trigger ( ) ) { return {true,{0,0,0}}; }

	// determine exit point
	return {true, -difference*(D-S2->Get_Size ( ))/(D == 0 ? 1 : D)};
}
collision_result Check_Collision ( Sphere_Collider *S1, Sphere_Collider *S2 ) {
	vec3 P = S1->Get_Pos ( ) - S2->Get_Pos ( );

	float D = glm::length ( P );
	float S = S1->Get_Size ( ) + S2->Get_Size ( );

	// collision not appened
	if ( D > S ) { return {false,{0,0,0}}; }

	// trigger collision ( no nead for fastest exit point )
	if ( S1->Is_Trigger( ) || S2->Is_Trigger ( ) ) { return {true,{0,0,0}}; }

	// determine exit point
	return {true, P*(S-D)/D};
}
