#pragma once

#include "objekt.hpp"

namespace Manager
{
	extern List < Objekt* > objekts;
	extern Objekt* _current_scene;

	void Start ( );
	void Update ( );

	Objekt* Objekt_Load
	(
		std::string name,
		vec3 pos = {0,0,0},
		vec3 size = {100,100,100},
		vec3 rot_pivot = {0,0,0}
	);
	Objekt* Objekt_Load ( Objekt * o );
	Objekt* Objekt_Get ( std::string name );
	void Free_Objekt ( std::string name );

	void Set_Active_Scene ( Objekt * o );
	void Set_Active_Scene ( std::string s );
	Objekt* Get_Active_Scene ( );

	void Check_Resource_Integrity ( );
}