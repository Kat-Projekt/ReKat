#ifndef PRIMITIVE_COLLISION_H
#define PRIMITIVE_COLLISION_H

#include "../objekt.h"
#include "rigidbody.h"

class Collider : public Behaviour {
private:
	bool _trigger = false;
	bool _static = true;
public:
	virtual void _Start ( ) { }
	virtual void Start ( ) { _static = ! obj->Has_Component < Rigidbody > ( ); _Start( ); }

	bool Is_Trigger ( ) { return _trigger; }
	Collider *Set_Trigger ( bool trigger ) { _trigger = trigger; return this; }

	bool Is_Static ( ) { return _static; }
	Collider * Set_Static ( bool S ) { _static = S; return this; }

	vec3 Get_Pos ( ) {
		vec3 pos = obj->Get_Pos ( );
		vec3 size = obj->Get_Size ( );
		float rot = obj->Get_Rot ( ).z;
		// center to sprite
		glm::vec3 p = obj->Get_Rot_Pivot ( );
		vec3 rot_vector = vec3{ size.x * ( p.x*cos(rot) - p.y*sin(rot) ), size.y * ( p.x*sin(rot) + p.y * cos(rot) ), 0 };
		pos -= rot_vector;
		return pos;	
	}
};

// ------------------------------- Collider Types -----------------------------------
// ----------------------------------------------------------------------------------
class Box_Collider : public Collider {
private:
	vec3 _size = {0,0,0};
public:
	void _Start ( ) { if ( _size == vec3{0,0,0} ) { _size = obj->Get_Size (); }}
	Collider* Set_Size ( float size ) { _size = vec3(size); return this; }
	Collider *Set_Size ( glm::vec3 size ) { _size = size; return this; }
	vec3 Get_Size ( ) { return _size; }
};

class Sfere_Collider : public Collider {
private:
	float _size = 0;
public:
	Collider *Set_Size ( float size ) { _size = size * 0.5; return this; }
	float Get_Size ( ) { return _size; }
};

struct Collision_Result {
	// specifies if the collision appened
	bool triggered = false;
	// specifies the direction for the shortes exit
	vec3 exit_direction = {0,0,0};
};

std::ostream& operator << ( std::ostream& os, const Collision_Result& C ) {
	os << ( C.triggered ? "collision { " :  "no collision { " ) << C.exit_direction << " } ";
	return os;
}

Collision_Result Check_Collision ( Box_Collider   *B1, Box_Collider   *B2 ) {
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
			float Sing = ( P_Delta.z >= 0 ? -1 : 1 );
			move_vector = { 0, 0, Delta.z * Sing };
		} else { // Delta.y is Low
			float Sing = ( P_Delta.y >= 0 ? -1 : 1 );
			move_vector = { 0, Delta.y * Sing, 0 };
		}
	} else {
		if ( abs ( Delta.x ) > abs ( Delta.z ) ) { // Delta.z is Low
			float Sing = ( P_Delta.z >= 0 ? -1 : 1 );
			move_vector = { 0, 0, Delta.z * Sing };
		} else { // Delta.x is Max
			float Sing = ( P_Delta.x >= 0 ? -1 : 1 );
			move_vector = { Delta.x * Sing, 0, 0 };
		}
	}

	return {true,move_vector};
}
Collision_Result Check_Collision ( Box_Collider   *B1, Sfere_Collider *S2 ) {
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
Collision_Result Check_Collision ( Sfere_Collider *S1, Sfere_Collider *S2 ) {
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

#endif