#pragma once
#include <objekt/objekt.hpp>
#include "../components/rigidbody.hpp"

enum class COLLIDER_TYPE
{
	BOX,
	SPHERE,
	NONE
};

class Collider : public Behaviour {
private:
	bool _trigger = false;
	vec3 _offset;
public:
	Collider ( )
	{
		Informations = { "Collider", 1.0, "The basic collider" };
	}

	bool Is_Trigger ( ) { return _trigger; }
	Collider *Set_Trigger ( bool trigger ) { _trigger = trigger; return this; }

	bool Is_Static ( ) { return ! obj->Has_Component < Rigidbody > ( ); }

	vec3 Get_Pos ( ) {
		vec3 pos = obj->Get_Pos ( );
		vec3 size = obj->Get_Size ( );
		float rot = obj->Get_Rot ( ).z;
		// center to sprite
		glm::vec3 p = obj->Get_Rot_Pivot ( );
		vec3 rot_vector = vec3{ size.x * ( p.x*cos(rot) - p.y*sin(rot) ), size.y * ( p.x*sin(rot) + p.y * cos(rot) ), 0 };
		pos -= rot_vector;
		pos += _offset;
		return pos;
	}

	Collider* Set_Offset ( vec3 offset ) { _offset = offset; return this; }

	virtual COLLIDER_TYPE Collider_Type ( ) { return COLLIDER_TYPE::NONE; }
};