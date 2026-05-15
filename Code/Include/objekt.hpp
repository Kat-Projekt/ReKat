#ifndef OBJEKT_H
#define OBJEKT_H

#include "debugger.hpp"
#include "utility/printer.h"
#include "utility/map.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <boost/config.hpp>

template < typename T >
class BOOST_SYMBOL_VISIBLE _behaviour
{
protected:
	bool _active = true;
	bool _started = false;

public:
	T* obj = nullptr;

	_behaviour ( ) // empty constructor
	{ }

	virtual ~_behaviour ( ) // empty deconstructor
	{ }

	// call protection prevents repeted starts of components and prevents calling when the copmonent is inactive
	void 
	_Start ( void )
	{
		if ( _active && !_started )
			Start ( );
		
		_started = true;
	}
	void 
	_Update ( void )
	{
		if ( _active )
			Update ( );
	}
	void 
	_Fixed_Update ( void ) 
	{ 
		if ( _active )
			Fixed_Update ( );
	}

	// Called only once Before every other function
	virtual void
	Start ( void ) { };	
	// Called every Time Update is called on the parent objekt
	virtual void 
	Update ( void ) { };	
	// Called every Time Fixed Update is called on the parent objekt
	virtual void
	Fixed_Update ( void ) { };	

	// Collision handleing
	virtual void 
	Collision
	( T* _obj ) 
	{
		( void ) _obj; // for preventing the error 
	}
	virtual void
	Collision_Exit
	( T* _obj )
	{
		( void ) _obj;
	}
	virtual void
	Collision_Enter
	( T* _obj )
	{
		( void ) _obj;
	}

	virtual void
	Collision_Trigger
	( T* _obj )
	{
		( void ) _obj;
	}
	virtual void
	Collision_Trigger_Exit
	( T* _obj )
	{
		( void ) _obj;
	}
	virtual void
	Collision_Trigger_Enter
	( T* _obj )
	{
		( void ) _obj;
	}

	void
	Set_Active 
	( bool active )
	{
		_active = active;
		if ( !_started ) 
			Start( );
	}
	bool
	Get_Active ( void )
	{
		return _active;
	}

	virtual void
	Delete ( void )
	{
		delete this;
	}

	virtual _behaviour *
	Set ( void )
	{
		return this;
	}
};

class Objekt {
protected:
	std::string _name = "";
	bool _active = true;
	bool _started = false;

		// Transform
	glm::vec3 _pos  = {0,0,0};
	vec3 _size = {100,100,100};
	struct quat
	{
		float w,x,y,z;
	};
	
	quat _rot = {1,0,0,0};
	vec3 _rot_pivot = {0,0,0};

		// Render Matrix
	bool _recalcutate = true;
	mat4 _model = mat4(1.0f);

		// CES
	Objekt* _father = nullptr;
	List < Objekt* > _childrens = { };
	List < _behaviour < Objekt >* > _components = { };
public:
	#define Behaviour _behaviour<Objekt>

	/* constructor */
	Objekt 
	( void ) 
	{ }

	Objekt
	(
		std::string name,
		vec3 pos = {0,0,0},
		vec3 size = {100,100,100},
		vec3 rot_pivot = {0,0,0}
	) 
	:
		_name(name),
		_pos(pos),
		_size(size),
		_rot_pivot(rot_pivot) 
	{
		DEBUG ( 4,
			"Inizializing Objekt: ", name,
			", pos: ", pos, 
			", size: ", size,
			// ", rot: ", rot,
			", rot_pivot: ", rot_pivot
		);
	}

	/* Free function */
	void Free
	( std::string p = "" )
	{
		DEBUG ( 4, p + "Deleting components of: ", _name );

		for 
		( auto C : _components )
		{
			C->Delete ( );
		}

		_components.Deallocate ( );
		_components = List < _behaviour < Objekt >* > ( );

		DEBUG ( 4, p + _name, " is Free - Deleting childrens" );
	
		for
		( auto C : _childrens )
		{
			C->Free ( p + "\t" );
			delete C;
		}
	
		_childrens.Deallocate ( );
		_childrens = List < Objekt* > ( );
	}

	/* Deconstructor */
	~Objekt
	( void )
	{
		Free ( );
	}

	void Set_Father
	( Objekt * father )
	{
		DEBUG ( 4,"linking father: ", father->Get_Name (), " to: ", Get_Name ( ) );
		_father = father;
	}
	Objekt* Get_Father ( ) { return _father; }
	Objekt* Add_Child ( Objekt * child ) {
		DEBUG ( 5,"adding child: ", child->Get_Name (), " to: ", Get_Name ( ) );
		child->Set_Father ( this );
		_childrens.append ( child );
		if ( _started ) { child->Start( ); }
		return this;
	}
	void Rem_Child ( std::string name ) {
		auto O = Get_Children ( name );
		if ( O == nullptr ) { DEBUG ( 3, "child", name, "not found"); return; }
		_childrens.remove ( O );
		DEBUG ( 4, "removing child: ", O->Get_Name (), " from: ", Get_Name ( ) );
	}
	void Rem_Child ( Objekt* O ) {
		_childrens.remove ( O );
		DEBUG ( 3,"\tremoving child: ", O->Get_Name (), " from: ", Get_Name ( ) );
	}
	void Delete ( std::string p = "" ) {
		if ( _father != nullptr ) { _father->Rem_Child ( this ); }
		_active = false;
		DEBUG ( 3, p + "Deleting Objekt: ", Get_Name ( ) );
		Free ( );
		Print_Tree ( p );
		_started = false;
	}
	Objekt* Get_Children ( std::string name ) {
		for ( auto C : _childrens )  {
			if ( C->Get_Name() == name ) 
			{ return C; }
		}
		return nullptr;
	}
	bool Has_Children ( Objekt* child ) {
		for ( auto C : _childrens )  {
			if ( C == child ) 
			{ return true; }
			if ( C->Has_Children ( child ) ) { return true; }
		}
		return false;
	}
	List < Objekt* > Get_Childrens ( ) { return _childrens; }
	int Count_Childrens ( ) {
		int count = 0;
		for ( auto C : _childrens ) {
			count ++;
			count += C->Count_Childrens ( );
		}
		return count;
	}

	void Set_Pos ( vec3 pos = {0,0,0} ) { _pos = pos; _recalcutate = true; }
	void Set_Pos ( float z = 0 ) { _pos.z = z; _recalcutate = true;  }
	void Inc_Pos ( vec3 pos = {0,0,0} ) { _pos += pos; _recalcutate = true;  }
	vec3 Get_Pos ( ) { return _pos + ( _father != nullptr ? _father->Get_Pos() : vec3{0,0,0}); }
	vec3 * Expose_Pos ( ) { return &_pos; }

	// input rotation is in euclidean angles and is aplied by z,y,x or yaw,pitch,roll
	void Set_Rot ( vec3 rot = {0,0,0} )
	{
		if ( rot == vec3{0,0,0} ) // edge case
		{
			_rot = {1,0,0,0};
			return;
		}
		// precopute cosine
		double sz = sin ( rot.z / 2 ), cz = cos ( rot.z / 2 );
		double sy = sin ( rot.y / 2 ), cy = cos ( rot.y / 2 );
		double sx = sin ( rot.x / 2 ), cx = cos ( rot.x / 2 );

		// eq 10 from https://danceswithcode.net/engineeringnotes/quaternions/quaternions.html
		_rot.w = cx * cy * cz + sx * sy * sz;
		_rot.x = sx * cy * cz - cx * sy * sz;
		_rot.y = cx * sy * cz + sx * cy * sz;
		_rot.z = cx * cy * sz - sx * sy * cz;

		_recalcutate = true;
	}
	void Set_2D_Rot ( float rot )
	{ 
		// the quaternion will be {cos r, 0,0, sin r } the rotation axis is [0,0,1] angle r
		_rot.w = cos ( rot / 2 );
		_rot.z = sin ( rot / 2 );

		_recalcutate = true;
	}
	// returns in euclidean rotation
	vec3 Get_Rot ( )
	{
		// method from https://danceswithcode.net/engineeringnotes/quaternions/quaternions.html
		
		// safe to run every time
		double pitch = asin ( 2 * (_rot.w * _rot.y + _rot.z * _rot.x) );
		double roll, yaw;

		// gimbal lock check
		if ( pitch == M_PI_2 || pitch == - M_PI_2 )
		{
			roll = 0;
			yaw = - sign ( pitch ) * 2 * atan2 ( _rot.x, _rot.w );
		} 
		else // lock occured
		{
			roll  = atan2 ( 2 * (_rot.w * _rot.x + _rot.y * _rot.z)
						, pow (_rot.w,2) - pow (_rot.x,2) - pow (_rot.y,2) + pow (_rot.z,2) );
			yaw   = atan2 ( 2 * (_rot.w * _rot.z + _rot.y * _rot.x)
					, pow (_rot.w,2) + pow (_rot.x,2) - pow (_rot.y,2) - pow (_rot.z,2) );
		}

		return vec3 { roll,pitch,yaw };
	}
	struct mono_axis_rotation {
		float angle;
		vec3 axis;
	};
	mono_axis_rotation Get_Rot_Mono ( )
	{
		// convert quat to modo axis angle
		// using 6 form https://danceswithcode.net/engineeringnotes/quaternions/quaternions.html
		mono_axis_rotation rot;

		if ( _rot.w == 1 ) // no rotation
		{
			rot.angle = 0;
			rot.axis = {1,0,0};
		}
		else
		{
			rot.angle = 2 * acos ( _rot.w );
			float st = sin ( rot.angle / 2 ); // sin \theta
			rot.axis = { _rot.x / st, _rot.y / st, _rot.z / st };
		}

		return rot;
	}

	void Set_Size ( vec3 size = {0,0,0} ) { _size = size; }
	vec3 Get_Size ( ) { return _size; }

	void Set_Rot_Pivot ( vec3 rot_pivot = {0,0,0} ) { _rot_pivot = rot_pivot; }
	vec3 Get_Rot_Pivot ( ) { return _rot_pivot; }

	void Set_Active ( bool active ) {
		_active = active;
		if ( !_started && active == true )
		{ Start( ); }
		for ( auto child : _childrens ) 
		{ child->Set_Active ( active ); }
	}
	bool Get_Active ( ) { return _active; }

	void Set_Name ( std::string name ) { _name = name; }
	inline std::string Get_Name ( ) { return _name; }

	template < class C >
	C* Add_Component ( ) {
        	if ( std::is_base_of<Behaviour, C>::value ) { 
			C* c = new C ( );
			DEBUG ( 4, "Adding Component: ", std::string(typeid(*c).name()), " to: ", _name );
			c->obj = this;
			_components.append ( ( Behaviour * ) ( c ) );
			if ( _started ) { c->_Start( ); }
        		return c;
		}
		DEBUG ( 2, "Wrong component decraration" );
	}
	template < class C > 
	C* Add_Component ( C* c ) {
		DEBUG ( 4,"Adding Component: ", std::string(typeid(*c).name()), " to: ", _name );
		if ( std::is_base_of<Behaviour, C>::value ) {
			c->obj = this;
			_components.append ( ( Behaviour * ) ( c ) );
			if ( _started ) { c->_Start( ); }
        	return c;
		}
		DEBUG ( 2, "Wrong component Behaviour" );
	}
	
	template < class C > 
	C* Get_Component ( ) {
		DEBUG ( 6,"Getting Component" );
		for ( auto c : _components ) 
		{ if ( typeid(*(c)) == typeid (C) ) { return (C*)c; } }
		DEBUG ( 3,"\t\tComponent Not Found" );
		return nullptr;
	}
	template < class C > 
	List < C* > Get_Component_Recursive ( ) {
		DEBUG ( 6,"Getting Components" );
		List < C* > L;
		for ( auto c : _components ) 
		{ if ( typeid(*(c)) == typeid (C) ) { L.append ( (C*)c ); } }
		for ( auto O : _childrens ) {
			auto Data = O->Get_Component_Recursive < C > ( );
			L.append ( Data );
		}
		if ( L.size () == 0 ) { DEBUG ( 3, "No components found" ); }
		return L;
	}

	template < typename C >
	bool Has_Component ( ) {
		for ( auto c : _components ) 
		{ if ( typeid(*(c)) == typeid (C) ) { return true; } }
		return false;
	}

	void Start ( std::string ind = "" ) {
		if ( _started ) { return; }
		if ( !_active ) { return; }
		DEBUG ( 5, ind,"Starting Objekt: ", Get_Name ( ) );
		for ( auto &&C : _components ) {
			DEBUG ( 6, ind," - Starting Componenet: ", std::string(typeid(*(C)).name()) );
			C->_Start ( );
			DEBUG ( 6,"Done" );
			DEBUG ( 6,*this );
		}
		DEBUG ( 5, ind, "Starting Childrens of ", Get_Name ( ) );
		for ( auto &&O : _childrens ) 
		{ O->Start ( ind + "- "); }

		DEBUG ( 5,ind,"Started Objekt: ", Get_Name() );
		_started = true;
    }

	void Update ( std::string ind = "" ) {
		if ( !_active ) { return; }

		if ( _components.size ( ) != 0 )
		{
			DEBUG ( 5,ind,"Updating Components of: ", Get_Name ( ) );
			
			for ( auto &&C : _components ) {
				DEBUG ( 6,ind,"+ Updating Componenet: ", std::string(typeid(*C).name()));
				C->_Update ( );
			}
		}

		if ( _childrens.size ( ) != 0 )
		{
			DEBUG ( 5,ind,"Updating Childrens of: ", Get_Name ( ) );
			for ( auto &&O : _childrens )
			{ O->Update ( ind + "- " ); }
		}
	
		DEBUG ( 5,ind,"Updated Objekt: ", Get_Name ( ) );
	}

	void Fixed_Update ( std::string ind = "" ) {
		if ( !_active ) { return; }
		DEBUG ( 5,ind,"Updating Fixed Components of: ", Get_Name() );
        	
		for ( auto C : _components ) {
			DEBUG ( 6,ind,"+ Updating Fixed Componenet: ", std::string(typeid(*C).name()));
			C->_Fixed_Update ( );
		}
		
		DEBUG ( 5,ind,"Updating Fixed Childrens of: ", Get_Name ( ) );
		for ( auto O : _childrens ) 
		{ O->Fixed_Update ( ind + "- "); }

		DEBUG ( 5,ind,"Updated Fixed Objekt: " + Get_Name() );
	}

	void Andle_Collsions ( Objekt * collider, float trigger = false, int Type = 0 ) {
		for ( auto C : _components ) {
			if ( !trigger ) {
				switch ( Type ) {
					case 0: C->Collision ( collider ); break; // Stay 
					case 1: C->Collision_Enter ( collider ); break; // Enter 
					case 2: C->Collision_Exit ( collider ); break; // Exit
				}
				DEBUG ( 4, "calling ", typeid ( *C ).name ( ) );
			} else {
				switch ( Type ) {
					case 0: C->Collision_Trigger ( collider ); break; // Stay 
					case 1: C->Collision_Trigger_Enter ( collider ); break; // Enter 
					case 2: C->Collision_Trigger_Exit ( collider ); break; // Exit
				}
			}
		}

		DEBUG ( 5, "A collision of type: ", Type, ( trigger ? "(trigger)" : "(solid)" ), 
			   " between ", Get_Name ( ), " and ", collider->Get_Name () );
	} 

	mat4 Get_Model_Mat ( ) {
		if ( _recalcutate )
		{
			// this is for centering during render trust me bro i know ( reduces 1 transation => good ) 
			vec3 Pivot = { (_rot_pivot.x + 0.5) * _size.x, (_rot_pivot.y + 0.5) * _size.y, (_rot_pivot.z) * _size.z };
			vec3 position = _pos;

			if ( _father != nullptr ) // get world postion recursively
			{ position+= _father->Get_Pos(); }

			_model = glm::mat4(1.0f);

			// apply rotation
			_model = glm::translate(_model, position);

			// _model = glm::rotate(_model, ang.angle, ang.axis);
			mat4 rotation_matrix = {
				{ 1 - 2 * pow ( _rot.y,2 ) - 2 * pow ( _rot.z,2 ), 2 * _rot.x * _rot.y - 2 * _rot.w * _rot.z, 2 * _rot.x * _rot.z + 2 * _rot.w * _rot.y, 0 },
				{ 2 * _rot.x * _rot.y + 2 * _rot.w * _rot.z, 1 - 2 * pow ( _rot.x,2 ) - 2 * pow ( _rot.z,2 ), 2 * _rot.y * _rot.z - 2 * _rot.w * _rot.x, 0 },
				{ 2 * _rot.x * _rot.z - 2 * _rot.w * _rot.y, 2 * _rot.y * _rot.z - 2 * _rot.w * _rot.x, 1 - 2 * pow ( _rot.x,2 ) - 2 * pow ( _rot.y,2 ), 0 },
				{0,0,0,1}
			};
			_model *= rotation_matrix;

			_model = glm::translate(_model, -Pivot);
			_model = glm::scale(_model, _size);

			_recalcutate = false;
			return _model;
		}
		else
		{
			return _model;
		}

		
	}

	void Print_Tree ( std::string level = "" ) {
		DEBUG ( 4, level, (*this) );
		level += "- ";
		for ( auto C : _childrens )
		{ C->Print_Tree ( level ); }
	}
	
	friend std::ostream& operator << ( std::ostream& os, Objekt& n ) {
        	os << n.Get_Name ( ) << " " << n._pos << " " << n._size << ( n._active ? " v" : " x" );
		if ( n._components.size ( ) == 0 )
		{
			return os;
		}
		os << " { ";
		// print components
		os << n._components.size ( ) << " ";
		for ( auto C : n._components ) 
		{ os << typeid(*C).name() << ( C->Get_Active () ? " v" : " x") << " "; }

		os << "}";
		
		return os;
	}

	friend std::ostream& operator,(std::ostream& out, Objekt& n )
	{ out << n; return out; }
};

namespace Manager {
	static List < Objekt* > objekts;
	static Objekt* _current_scene = nullptr;

	static void Start ( ) { if ( _current_scene != nullptr ) { 
		// color ( "STARTING\n", BACKGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY ); 
		_current_scene->Start(); 
	} };
	static void Update ( ) { if ( _current_scene != nullptr ) { 
		// color ( "UPDATING\n", BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY );
		_current_scene->Update();
		// color ( "UPDATED\n", BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY );
	} };

	static Objekt* Objekt_Get ( std::string name ) {
		for ( auto S : objekts )  {
			if ( S->Get_Name () == name ) 
			{ return S; }
		}
		DEBUG ( 1, "Cannot find objekt ", name );
		return nullptr;
	}
	static Objekt* Objekt_Load ( std::string name, vec3 pos = {0,0,0}, vec3 size = {100,100,100}, vec3 rot_pivot = {0,0,0} ) {
		auto o = new Objekt(name, pos, size, rot_pivot);
		objekts.append ( o );
		return o;
	}
	static Objekt* Objekt_Load ( Objekt * o ) 
	{ objekts.append ( o ); return o;}
	static void Set_Active_Scene ( Objekt * o ) {
		DEBUG ( 3,"adding active scene" );
		objekts.append ( o );
		_current_scene = o;
		Start( );
		DEBUG ( 5,"added main scene" );
	}
	static void Set_Active_Scene ( std::string s ) {
		DEBUG ( 3,"adding active scene" );
		for ( auto S : objekts ) {
			if ( S->Get_Name () == s ) 
			{ _current_scene = S; DEBUG ( 5,"added main scene" ); Start( ); return; }
		}
	}
	
	static Objekt* Get_Active_Scene ( ) { return _current_scene; }

	static void Check_Resource_Integrity ( ) {
		DEBUG ( 3, "STARTING INTEGRITY CHECK" );
		DEBUG ( 4, "Current Scene: '", _current_scene->Get_Name ( ) );
		
		for ( auto C : objekts ) 
		{ DEBUG ( 4,*C ); }
	}

	static void Free_Objekt ( std::string name ) {
		auto D = Objekt_Get ( name );
		D->Delete ( );
		objekts.remove ( D );

		if( D == _current_scene ) 
		{ _current_scene = nullptr; }

		delete D;
	}
}

#endif
