#pragma once

#include <utilities/debugger.hpp>
#include <utilities/printer.h>
#include <utilities/list.hpp>
#include <string>
#include <memory>
#include <concepts>
#include <functional>

#include "behaviour.hpp"
#include "component_manager.hpp"
#include "transform.hpp"

enum class collision_type {
	Enter,
	Stay,
	Exit
};

class Objekt : public std::enable_shared_from_this<Objekt>
{
protected:
	std::string _name = "";
	bool _active = true;
	bool _started = false;

	Transform _transform;

	std::shared_ptr < Objekt > _father = nullptr;
	List < std::shared_ptr < Objekt > > _children = { };
	List < std::shared_ptr < Behaviour > > _components = { };

private:
	template < typename Component_Call, typename Objekt_Call >
	void Recursive_Caller
	(
		std::string ind,
		std::string message,
		Component_Call component_call,
		Objekt_Call object_call
	);

public:
	Objekt ( void );

	Objekt ( 
		std::string name,
		vec3 pos = {0,0,0},
		vec3 size = {100,100,100},
		vec3 rot_pivot = {0,0,0}
	);

	~Objekt ( void );

	void Delete ( );

	void Set_Father ( std::shared_ptr < Objekt > father );
	std::shared_ptr < Objekt > Get_Father ( ) const;

	Transform& Get_Transform ( );
	const Transform& Get_Transform ( ) const;

	Objekt& Add_Child ( std::shared_ptr < Objekt > child );
	std::shared_ptr < Objekt > Get_Child ( std::string name );
	List < std::shared_ptr < Objekt > > & Get_Children ( );

	bool Has_Child ( std::shared_ptr < Objekt > child );
	bool Has_Child ( std::string name );
	Objekt& Rem_Child ( std::string name );
	Objekt& Rem_Child ( std::shared_ptr < Objekt > O );
	int  Count_Children ( );

	Objekt& Set_Pos ( vec3 pos = {0,0,0} );
	Objekt& Set_Pos ( float z = 0 );
	Objekt& Inc_Pos ( vec3 pos = {0,0,0} );
	Objekt& Set_Size ( vec3 size = {0,0,0} );
	Objekt& Set_Rot ( vec3 rot = {0,0,0} );
	Objekt& Set_2D_Rot ( float rot );
	Objekt& Set_Active ( bool active );
	Objekt& Set_Rot_Pivot ( vec3 rot_pivot = {0,0,0} );
	Objekt& Set_Name ( std::string name );
	
	const vec3 Get_Pos ( ) const;
	vec3* Expose_Pos ( );
	const vec3 Get_Size ( ) const;
	const vec3 Get_Rot ( ) const;
	const vec3 Get_Rot_Pivot ( ) const;
	const Transform::mono_axis_rotation Get_Rot_Mono ( ) const;
	bool Get_Active ( ) const;
	std::string Get_Name ( ) const;

	template < class C > std::shared_ptr < C > Add_Component ( );
	template < class C > std::shared_ptr < C > Add_Component ( std::shared_ptr < C > c );
	std::shared_ptr < Behaviour > Add_Component ( std::string type );

	template < class C > std::shared_ptr < C > Get_Component ( );
	template < class C > List < std::shared_ptr < C > >  Get_Components ( );
	template < class C > List < std::shared_ptr < C > > Get_Component_Recursive ( );
	template < class C > List < std::shared_ptr < C > > Get_Component_In_Children ( );

	std::shared_ptr < Behaviour > Get_Component ( std::string type );
	List < std::shared_ptr < Behaviour > >  Get_Components ( std::string type );
	List < std::shared_ptr < Behaviour > > Get_Component_Recursive ( std::string type );
	List < std::shared_ptr < Behaviour > > Get_Component_In_Children ( std::string type );

	template < class C > std::shared_ptr < C > Rem_Component ( );
	std::shared_ptr < Behaviour > Rem_Component ( std::string type );
	template < class C > std::shared_ptr < C > Rem_Components ( );

	template < class C > bool Has_Component ( );
	bool Has_Component ( std::string type );

	void Start ( std::string ind = "" );
	void Early_Update ( std::string ind = "" );
	void Update ( std::string ind = "" );
	void Late_Update ( std::string ind = "" );
	void Fixed_Update ( std::string ind = "" );

	void Handle_Collisions (
		Objekt* collider,
		bool trigger = false,
		collision_type Type = collision_type::Stay
	);

	mat4 Get_Model_Mat ( );

	void Print_Tree ( std::string level = "" );
	friend std::ostream& operator << ( std::ostream& os, Objekt& n );
	friend std::ostream& operator , ( std::ostream& out, Objekt& n );
};

#include "objekt.tpp"