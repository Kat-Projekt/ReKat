#pragma once

#include <utilities/debugger.hpp>
#include <utilities/printer.h>
#include <utilities/list.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <memory>

#include "behaviour.hpp"
#include "component_manager.hpp"


class Objekt : public std::enable_shared_from_this<Objekt>
{
protected:
	std::string _name = "";
	bool _active = true;
	bool _started = false;

	struct quat
	{
		float w,x,y,z;
	};

	vec3 _pos = {0,0,0};
	vec3 _size = {100,100,100};
	quat _rot = {1,0,0,0};
	vec3 _rot_pivot = {0,0,0};

	bool _recalcutate = true;
	mat4 _model = mat4(1.0f);

	std::shared_ptr < Objekt > _father = nullptr;
	List < std::shared_ptr < Objekt > > _childrens = { };
	List < std::shared_ptr < Behaviour > > _components = { };

public:
	Objekt ( void );

	Objekt ( 
		std::string name,
		vec3 pos = {0,0,0},
		vec3 size = {100,100,100},
		vec3 rot_pivot = {0,0,0}
	);

	~Objekt ( void );

	void Free ( );

	void Set_Father ( std::shared_ptr < Objekt > father );
	std::shared_ptr < Objekt > Get_Father ( );
	std::shared_ptr < Objekt > Add_Child ( std::shared_ptr < Objekt > child );
	std::shared_ptr < Objekt > Get_Children ( std::string name );
	List < std::shared_ptr < Objekt > > & Get_Childrens ( );
	bool Has_Children ( std::shared_ptr < Objekt > child );
	bool Has_Children ( std::string name );
	std::shared_ptr < Objekt > Rem_Child ( std::string name );
	std::shared_ptr < Objekt > Rem_Child ( std::shared_ptr < Objekt > O );
	int  Count_Childrens ( );

	std::shared_ptr < Objekt > Set_Pos ( vec3 pos = {0,0,0} );
	std::shared_ptr < Objekt > Set_Pos ( float z = 0 );
	std::shared_ptr < Objekt > Inc_Pos ( vec3 pos = {0,0,0} );
	std::shared_ptr < Objekt > Set_Size ( vec3 size = {0,0,0} );
	std::shared_ptr < Objekt > Set_Rot ( vec3 rot = {0,0,0} );
	std::shared_ptr < Objekt > Set_2D_Rot ( float rot );
	std::shared_ptr < Objekt > Set_Active ( bool active );
	std::shared_ptr < Objekt > Set_Rot_Pivot ( vec3 rot_pivot = {0,0,0} );
	std::shared_ptr < Objekt > Set_Name ( std::string name );
	
	// from quaternion rotation results
	struct mono_axis_rotation {
		float angle;
		vec3 axis;
	};
	
	vec3 Get_Pos ( );
	vec3& Expose_Pos ( );
	vec3 Get_Size ( );
	vec3 Get_Rot ( );
	vec3 Get_Rot_Pivot ( );
	mono_axis_rotation Get_Rot_Mono ( );
	bool Get_Active ( );
	std::string Get_Name ( );

	template < class C > std::shared_ptr < C > Add_Component ( );
	template < class C > std::shared_ptr < C > Add_Component ( std::shared_ptr < C > c );
	std::shared_ptr < Behaviour > Add_Component_Special ( std::string type );

	template < class C > std::shared_ptr < C > Get_Component ( );
	std::shared_ptr < Behaviour > Get_Component ( std::string type );
	template < class C > List < std::shared_ptr < C > > Get_Component_Recursive ( );
	List < std::shared_ptr < Behaviour > > Get_Component_Recursive ( std::string type );

	template < class C > bool Has_Component ( );
	bool Has_Component ( std::string type );

	void Start ( std::string ind = "" );
	void Update ( std::string ind = "" );
	void Fixed_Update ( std::string ind = "" );

	enum collision_type {
		Enter,
		Stay,
		Exit
	};
	void Handle_Collsions (
		std::shared_ptr < Objekt > collider,
		float trigger = false,
		collision_type Type = Stay
	);

	mat4 Get_Model_Mat ( );

	void Print_Tree ( std::string level = "" );
	friend std::ostream& operator << ( std::ostream& os, Objekt& n );
	friend std::ostream& operator , ( std::ostream& out, Objekt& n );
};

#include "objekt.tpp"