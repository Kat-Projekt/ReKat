#pragma once
#include "../collision_handle/collider.hpp"

class Sphere_Collider : public Collider {
private:
	float _size = 0;
public:
	Collider* Set_Size ( float size ) { _size = size * 0.5f; return this; }
	float Get_Size ( ) { return _size; }
	COLLIDER_TYPE Collider_Type ( ) { return COLLIDER_TYPE::SPHERE; }
};