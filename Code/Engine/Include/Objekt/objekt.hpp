#pragma once

#include "debugger.hpp"
#include "utility/printer.h"
#include "utility/map.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

#include "behaviour.hpp"

class Objekt
{
protected:
	std::string _name = "";
	bool _active = true;
	bool _started = false;

	glm::vec3 _pos = {0,0,0};
	vec3 _size = {100,100,100};

	struct quat
	{
		float w,x,y,z;
	};

	quat _rot = {1,0,0,0};
	vec3 _rot_pivot = {0,0,0};

	bool _recalcutate = true;
	mat4 _model = mat4(1.0f);

	Objekt* _father = nullptr;
	List < Objekt* > _childrens = { };
	List < Behaviour > _components = { };

public:

	#define Behaviour _behaviour < Objekt >

	Objekt ( void );

	Objekt 
	(
		std::string name,
		vec3 pos = {0,0,0},
		vec3 size = {100,100,100},
		vec3 rot_pivot = {0,0,0}
	);

	~Objekt ( void );

	void Free ( std::string p = "" );

	void Set_Father ( Objekt * father );
	Objekt* Get_Father ( );
	Objekt* Add_Child ( Objekt * child );
	void Rem_Child ( std::string name );
	void Rem_Child ( Objekt* O );
	void Delete ( std::string p = "" );

	Objekt* Get_Children ( std::string name );
	bool Has_Children ( Objekt* child );
	List < Objekt* > Get_Childrens ( );
	int Count_Childrens ( );

	void Set_Pos ( vec3 pos = {0,0,0} );
	void Set_Pos ( float z = 0 );
	void Inc_Pos ( vec3 pos = {0,0,0} );
	vec3 Get_Pos ( );
	vec3 * Expose_Pos ( );

	void Set_Rot ( vec3 rot = {0,0,0} );
	void Set_2D_Rot ( float rot );
	vec3 Get_Rot ( );

	struct mono_axis_rotation
	{
		float angle;
		vec3 axis;
	};
	mono_axis_rotation Get_Rot_Mono ( );

	void Set_Size ( vec3 size = {0,0,0} );
	vec3 Get_Size ( );

	void Set_Rot_Pivot ( vec3 rot_pivot = {0,0,0} );
	vec3 Get_Rot_Pivot ( );

	void Set_Active ( bool active );
	bool Get_Active ( );

	void Set_Name ( std::string name );
	std::string Get_Name ( );

	template < class C > C* Add_Component ( );
	template < class C > C* Add_Component ( std::shared_ptr < C > c );
	template < class C > C* Get_Component ( );
	template < class C > List < C* > Get_Component_Recursive ( );
	template < typename C > bool Has_Component ( );

	void Start ( std::string ind = "" );
	void Update ( std::string ind = "" );
	void Fixed_Update ( std::string ind = "" );

	enum collision_type {
		Enter,
		Stay,
		Exit
	};
	void Andle_Collsions ( Objekt * collider, float trigger = false, collision_type Type = Stay );

	mat4 Get_Model_Mat ( );

	void Print_Tree ( std::string level = "" );

	friend std::ostream& operator << ( std::ostream& os, Objekt& n );
	friend std::ostream& operator , ( std::ostream& out, Objekt& n );
};