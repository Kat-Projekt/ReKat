#pragma once
#include "../collision_handle/collider.hpp"

class Box_Collider : public Collider {
private:
	vec3 _size = {0,0,0};
public:
	Collider* Set_Size ( float size ) { _size = vec3(size); return this; }
	Collider* Set_Size ( glm::vec3 size ) { _size = size; return this; }
	vec3 Get_Size ( ) { return _size; }
	COLLIDER_TYPE Collider_Type ( ) { return COLLIDER_TYPE::BOX; }
};