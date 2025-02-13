#ifndef OBJEKT_H
#define OBJEKT_H

#include "debugger.hpp"

template < typename T >
class _behaviour {
protected:
	bool _active = true;
	bool _started = false;
public:
    T* obj;

	virtual void _Start ( ) { if ( _active && !_started ) { Start ( ); } _started = true; }
	virtual void _Update ( ) { if ( _active ) { Update ( ); } }
	virtual void _Fixed_Update ( ) { if ( _active ) { Fixed_Update ( ); } }

	void _Collision ( T* _obj ) { if ( _active ) { Collision ( _obj ); } }
	void _Collision_Trigger ( T* _obj ) { if ( _active ) { Collision_Trigger ( _obj ); } }

	// Called only once Before every other function
    virtual void Start ( ) { }
	// Called every Time Update is called on the parent objekt
    virtual void Update ( ) { }
	// Called every Time Fixed Update is called on the parent objekt
	virtual void Fixed_Update ( ) { }

	// Collision handleing
	virtual void Collision ( T* _obj ) { }
	virtual void Collision_Exit ( T* _obj ) { }
	virtual void Collision_Enter ( T* _obj ) { }

    virtual void Collision_Trigger ( T* _obj ) { }
	virtual void Collision_Trigger_Exit ( T* _obj ) { }
	virtual void Collision_Trigger_Enter ( T* _obj ) { }

	void Set_Active ( bool active ) 
	{ _active = active; if ( !_started ) { Start( ); } }
	bool Get_Active ( ) { return _active; }

	virtual void Delete ( ) {
		delete this;
	}
};

#include "utility/printer.h"
#include "utility/map.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

using namespace glm;

class Objekt {
protected:
    std::string _name;
	bool _active = true;
	bool _started = false;

    // Transform
    vec3 _pos  = {0,0,0};
    vec3 _size = {100,100,100};
    vec3 _rot  = {0,0,0};
	vec3 _rot_pivot = {0,0,0};

    // Render Matrix
    mat4 _model = mat4(1.0f);

	Objekt* _father = nullptr;
	List < Objekt* > _childrens;
	List < _behaviour < Objekt >* > _components;

public:
	#define Behaviour _behaviour<Objekt>

    Objekt ( ) { DEBUG ( 4,"Created Empty Objekt"); }
    Objekt ( std::string name, vec3 pos = {0,0,0}, vec3 size = {100,100,100}, vec3 rot = {0,0,0}, vec3 rot_pivot = {0,0,0} ) 
	: _name(name), _pos(pos), _size(size), _rot(rot), _rot_pivot(rot_pivot) 
	{ DEBUG ( 4,"Inizializing Objekt: ", name, ", pos: ", pos, ", size: ", size, ", rot: ", rot, ", rot_pivot: ", rot_pivot ); }
	void Free ( std::string p = "" ) {
		DEBUG ( 4, p + "Deleting components of: ", _name );
		for ( auto C : _components )
		{ C->Delete ( ); }
		// _components.Deallocate ( );
		_components = List < _behaviour < Objekt >* > ( );
		DEBUG ( 4, p + _name, " is Free - Deleting childrens" );
		for ( auto C : _childrens ) {
			C->Free ( p + "\t" );
			delete C;
		}
		// _childrens.Deallocate ( );
		_childrens = List < Objekt* > ( );
	}
	~Objekt ( ) {
		Free ( );
	}

	void Set_Father ( Objekt * father ) {
		DEBUG ( 4,"\tlinking father: ", father->Get_Name (), " to: ", _name );
		_father = father;
	}
	Objekt* Get_Father ( ) { return _father; }
    Objekt* Add_Child ( Objekt * child ) {
		DEBUG ( 5,"\tadding child: ", child->Get_Name (), " to: ", _name );
		child->Set_Father ( this );
		_childrens.append ( child );
		if ( _started ) { child->Start( ); }
		return this;
	}
	void Rem_Child ( std::string name ) {
		auto O = Get_Children ( name );
		if ( O == nullptr ) { DEBUG ( 3, "child", name, "not found"); return; }
		_childrens.remove ( O );
		DEBUG ( 4, "removing child: ", O->Get_Name (), " from: ", _name );
	}
	void Rem_Child ( Objekt* O ) {
		_childrens.remove ( O );
		DEBUG ( 3,"\tremoving child: ", O->Get_Name (), " from: ", _name );
	}
	void Delete ( std::string p = "" ) {
		if ( _father != nullptr ) { _father->Rem_Child ( this ); }
		_active = false;
		DEBUG ( 3, p + "Deleting Objekt: ", _name );
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

	void Set_Pos ( vec3 pos = {0,0,0} ) { _pos = pos; }
	void Set_Pos ( float z = 0 ) { _pos.z = z; }
	void Inc_Pos ( vec3 pos = {0,0,0} ) { _pos += pos; }
	vec3 Get_Pos ( ) { return _pos + ( _father != nullptr ? _father->Get_Pos() : vec3{0,0,0}); }
	vec3 * Expose_Pos ( ) { return &_pos; }

	void Set_Rot ( vec3 rot = {0,0,0} ) { _rot = rot; }
	void Set_2D_Rot ( float rot ) { _rot.z = rot; }
	vec3 Get_Rot ( ) { return _rot; }

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

    virtual void Start ( ) {
		if ( _started ) { return; }
		if ( !_active ) { return; }
		DEBUG ( 4,"Starting Objekt: ", Get_Name() );
		for ( auto &&C : _components ) {
			DEBUG ( 6," - Starting Componenet: ", std::string(typeid(*(C)).name()) );
			C->_Start ( );
			DEBUG ( 6,"Done" );
			DEBUG ( 6,*this );
		}
		DEBUG ( 6,"Starting Childrens" );
		for ( auto &&O : _childrens ) 
		{ O->Start ( ); }
		DEBUG ( 6,"Childrens Started" );
		DEBUG ( 5,"Started Objekt: ", Get_Name() );
		_started = true;
    }

    virtual void Update ( std::string ind = "" ) {
		if ( !_active ) { return; }
		DEBUG ( 4,ind,"Updating Objekt: ", Get_Name() );
        for ( auto &&C : _components ) {
			DEBUG ( 6,ind,"+ Updating Componenet: ", std::string(typeid(*C).name()));
			C->_Update ( );
			DEBUG ( 6,ind,"+ Done" );
		}
		DEBUG ( 6,ind,"Updating Childrens" );
		for ( auto &&O : _childrens ) 
		{ O->Update ( ind + "- " ); }
		DEBUG ( 6,ind,"Childrens Updated" );
		DEBUG ( 5,ind,"Updated Objekt: ", Get_Name() );
    }

    virtual void Fixed_Update ( ) {
		if ( !_active ) { return; }
		DEBUG ( 5,"Updating Fixed Objekt: ", Get_Name() );
        for ( auto C : _components ) {
			DEBUG ( 6,"Updating Fixed Componenet: ", std::string(typeid(*C).name()));
			C->_Fixed_Update ( );
			DEBUG ( 6,"Done" );
		}
		DEBUG ( 6,"Updating Childrens Fixed" );
		for ( auto O : _childrens ) 
		{ O->Fixed_Update ( ); }
		DEBUG ( 6,"Childrens Updated Fixed" );
		DEBUG ( 5,"Updated Fixed Objekt: " + Get_Name() );
    }

	virtual void Andle_Collsions ( Objekt * collider, float trigger = false, int Type = 0 ) {
		for ( auto C : _components ) {
	if ( !trigger ) {
		switch ( Type ) {
			case 0: C->Collision ( collider ); break; // Stay 
			case 1: C->Collision_Enter ( collider ); break; // Enter 
			case 2: C->Collision_Exit ( collider ); break; // Exit
		}
		std::cout << "calling " << typeid ( *C ).name ( ) << '\n';
	} else {
		switch ( Type ) {
			case 0: C->Collision_Trigger ( collider ); break; // Stay 
			case 1: C->Collision_Trigger_Enter ( collider ); break; // Enter 
			case 2: C->Collision_Trigger_Exit ( collider ); break; // Exit
		}
	}
		}
		std::cout << "andling collision of type: " << Type << " triggered " << trigger << '\n';
	} 

	mat4 Get_Model_Mat ( ) {
		vec3 Pivot = { (_rot_pivot.x + 0.5) * _size.x, (_rot_pivot.y + 0.5) * _size.y, (_rot_pivot.z) * _size.z };
		vec3 position = _pos;
		if ( _father != nullptr ) { position+= _father->Get_Pos(); }

		mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        if ( _rot.x != 0 ) { model = glm::rotate(model, glm::radians(_rot.x), glm::vec3(1.0f, 0.0f, 0.0f)); }
    	if ( _rot.y != 0 ) { model = glm::rotate(model, glm::radians(_rot.y), glm::vec3(0.0f, 1.0f, 0.0f)); }
    	if ( _rot.z != 0 ) { model = glm::rotate(model, glm::radians(_rot.z), glm::vec3(0.0f, 0.0f, 1.0f)); }
        model = glm::translate(model, -Pivot);

        model = glm::scale(model, _size);

		return model;
	}

	void Print_Tree ( std::string level = "" ) {
		DEBUG ( 4, level, _name, " ", _pos, " ", _size, ( _active ? " v" : " x") );
		for ( auto C : _components ) 
		{ DEBUG ( 5, level, "+ ", typeid(*C).name(), ( C->Get_Active () ? " v" : " x") ); }
		level += "- ";
		for ( auto C : _childrens )
		{ C->Print_Tree ( level ); }
	}
	void Print_Direct_Tree ( std::string level = "" ) {
		std::cout << level << _name << " " << _pos << " " << _size << ( _active ? " v" : " x") << '\n';
		for ( auto C : _components ) 
		{ std::cout << level << "+ " << typeid(*C).name() << ( C->Get_Active () ? " v" : " x") << '\n'; }
		level += "- ";
		std::cout << level << " Childs " << _childrens.size ( ) << '\n'; 
		for ( auto C : _childrens )
		{ C->Print_Direct_Tree ( level ); }
	}
	
	friend std::ostream& operator << ( std::ostream& os, Objekt& n ) {
        os << n.Get_Name ( ) << " " << n._pos << " " << n._size << ( n._active ? " v" : " x" ) << " { ";
		// print components
		os << n._components.size ( ) << " ";
		for ( auto C : n._components ) 
		{ os << typeid(*C).name() << ( C->Get_Active () ? " v" : " x") << " "; }

		os << " }";
		
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
	}
	static Objekt* Objekt_Load ( std::string name, vec3 pos = {0,0,0}, vec3 size = {100,100,100}, vec3 rot = {0,0,0}, vec3 rot_pivot = {0,0,0} ) {
		auto o = new Objekt(name, pos, size, rot, rot_pivot);
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
		delete D;
	}
}

#endif
