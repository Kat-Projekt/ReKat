#pragma once

#include "scene.h"

class Manager
{
private:
	static std::unordered_map < u_int32_t, std::string > _name_table;
	static std::unordered_map < u_int32_t, std::unique_ptr < Objekt > > _objekts;
	static Objekt * current_scene;

public:

	extern void Start ( );
	extern void Update ( );

	extern Objekt * Objekt_Load
	(
		std::string name,
		glm::vec3 pos = {0,0,0},
		glm::vec3 size = {100,100,100},
		glm::vec3 rot = {0,0,0},
		glm::vec3 rot_pivot = {0,0,0}
	);
	extern Objekt * Objekt_Load ( std::unique_ptr < Objekt > o );

	extern Objekt * Objekt_Get ( std::string name );

	extern int Register_Objekt ( Objekt* _ptr );
	
	extern void Free_Objekt ( std::string name );
	extern void Free_Objekts ( );

	extern void Set_Active_Scene ( std::string s );
	extern Objekt * Get_Active_Scene ( );

	extern void Check_Resource_Integrity ( );
}
