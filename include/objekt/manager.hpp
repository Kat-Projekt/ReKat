#pragma once

#include "objekt.hpp"

namespace Manager
{
	extern List < std::shared_ptr < Objekt > > objekts;
	extern std::shared_ptr < Objekt > _current_scene;

	extern void Start ( );
	extern void Update ( );

	extern std::shared_ptr < Objekt > Objekt_Load
	(
		std::string name,
		vec3 pos = {0,0,0},
		vec3 size = {100,100,100},
		vec3 rot_pivot = {0,0,0}
	);
	extern std::shared_ptr < Objekt > Objekt_Load ( std::shared_ptr < Objekt > o );

	extern std::shared_ptr < Objekt > Objekt_Get ( std::string name );
	
	extern void Free_Objekt ( std::string name );
	extern void Free_Objekts ( );

	extern void Set_Active_Scene ( std::shared_ptr < Objekt > o );
	extern void Set_Active_Scene ( std::string s );
	extern std::shared_ptr < Objekt > Get_Active_Scene ( );

	extern void Check_Resource_Integrity ( );
}