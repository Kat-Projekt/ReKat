#ifndef OBJEKT_H
#define OBJEKT_H

#ifdef DIAGNOSTIC 
#include <iostream>
#define DEBUG(...) std::cout , __VA_ARGS__ , std::endl
template <typename T>
std::ostream& operator,(std::ostream& out, const T& t)
{ out << t; return out; }
std::ostream& operator,(std::ostream& out, std::ostream&(*f)(std::ostream&))
{ out << f; return out; }
#else
#define DEBUG(...)
#endif

template < typename T >
class _behaviour {
	bool _active = true;
	bool _started = false;
public:
    T* obj;

	void _Start ( ) { if ( _active && !_started ) { Start ( ); } _started = true; }
	void _Update ( ) { if ( _active ) { Update ( ); } }
	void _Fixed_Update ( ) { if ( _active ) { Fixed_Update ( ); } }
	void _UI_Update ( ) { if ( _active ) { UI_Update ( ); } }

	void _Collision ( T* _obj ) { if ( _active ) { Collision ( _obj ); } }
	void _Collision_Trigger ( T* _obj ) { if ( _active ) { Collision_Trigger ( _obj ); } }

    virtual void Start ( ) { }
    virtual void Update ( ) { }
	virtual void Fixed_Update ( ) { }
	virtual void UI_Update ( ) { }

    virtual void Collision ( T* _obj ) { _obj; }
    virtual void Collision_Trigger ( T* _obj ) { _obj; }

	void Set_Active ( bool active ) 
	{ _active = active; if ( !_started ) { Start( ); } }
	bool Get_Active ( ) { return _active; }

	virtual void Delete ( ) { }
};

#include "utility/printer.h"
#include "utility/list.h"
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

    Objekt ( ) { DEBUG ("Created Empty Objekt"); }
    Objekt ( std::string name, vec3 pos = {0,0,0}, vec3 size = {100,100,100}, vec3 rot = {0,0,0}, vec3 rot_pivot = {0,0,0} ) 
	: _name(name), _pos(pos), _size(size), _rot(rot), _rot_pivot(rot_pivot) 
	{ DEBUG ( "Inizializing Objekt: ", name, ", pos: ", pos, ", size: ", size, ", rot: ", rot, ", rot_pivot: ", rot_pivot ); }
	void Free ( ) {
		auto c = _components.Get_Begin ( );
		while ( c != nullptr ) 
		{ c->data->Delete ( ); c = c->next; }

		DEBUG ( "Freeing Objekt: ", _name );
		std::cout << _childrens << '\n';
		_childrens.Deallocate ( );
		DEBUG ( "Freed Childrens of: ", _name );
		std::cout << _components << '\n';
		_components.Deallocate ( );
		DEBUG ( "Freed Components of: ", _name );
	}
	~Objekt ( ) {
		Free ( );
	}

	void Set_Father ( Objekt * father ) {
		DEBUG ( "\tlinking father: ", father->Get_Name (), " to: ", _name );
		_father = father;
	}
	Objekt* Get_Father ( ) { return _father; }
    Objekt* Add_Child ( Objekt * child ) {
		DEBUG ( "\tadding child: ", child->Get_Name (), " to: ", _name );
		child->Set_Father ( this );
		_childrens.append ( child );
		if ( _started ) { child->Start( ); }
		return this;
	}
	void Rem_Child ( std::string name ) {
		auto O = Get_Children ( name );
		_childrens.remove ( O );
		DEBUG ( "\tremoving child: ", O->Get_Name (), " from: ", _name );
	}
	void Rem_Child ( Objekt* O ) {
		_childrens.remove ( O );
		DEBUG ( "\tremoving child: ", O->Get_Name (), " from: ", _name );
	}
	void Delete ( std::string p = "" ) {
		if ( _father != nullptr ) { _father->Rem_Child ( this ); }
		_active = false;
		DEBUG ( p + "Deleting Objekt: ", _name );
		auto C = _childrens.Get_Begin ( );
		while ( C != nullptr ) {
			C->data->Delete( p + "\t");
			C = C->next;
		}
		Free ( );
	}
    Objekt* Get_Children ( std::string name ) {
		auto C = _childrens.Get_Begin ( );
		while ( C != nullptr ) {
			if ( C->data->Get_Name() == name ) 
			{ return C->data; }
			C = C->next;
		}
		return nullptr;
	}
	List < Objekt* > Get_Childrens ( ) { return _childrens; }
	int Count_Childrens ( ) {
		int count = 0;
		auto C = _childrens.Get_Begin ( );
		while ( C != nullptr ) {
			count ++;
			count += C->data->Count_Childrens ( );
			C = C->next;
		}
		return count;
	}

	void Set_Pos ( vec3 pos = {0,0,0} ) { _pos = pos; }
	void Set_Pos ( float z = 0 ) { _pos.z = z; }
	void Inc_Pos ( vec3 pos = {0,0,0} ) { _pos += pos; }
	vec3 Get_Pos ( ) { return _pos + ( _father != nullptr ? _father->Get_Pos() : vec3{0,0,0}); }

	void Set_Rot ( vec3 rot = {0,0,0} ) { _rot = rot; }
	void Set_2D_Rot ( float rot ) { _rot.z = rot; }
	vec3 Get_Rot ( ) { return _rot; }

	void Set_Size ( vec3 size = {0,0,0} ) { _size = size; }
	vec3 Get_Size ( ) { return _size; }

	void Set_Rot_Pivot ( vec3 rot_pivot = {0,0,0} ) { _rot_pivot = rot_pivot; }
	vec3 Get_Rot_Pivot ( ) { return _rot_pivot; }

    void Set_Active ( bool active ) 
	{ _active = active; if ( !_started ) { Start( ); } }
    bool Get_Active ( ) { return _active; }

    void Set_Name ( std::string name ) { _name = name; }
    std::string Get_Name ( ) { return _name; }

    template < class C >
    C* Add_Component ( ) {
		DEBUG ( "- - - Adding Component to: ", _name );
        if ( std::is_base_of<Behaviour, C>::value ) { 
			C* c = new C ( );
			c->obj = this;
			_components.append ( ( Behaviour * ) ( c ) );
			if ( _started ) { c->_Start( ); }
        	return c;
		}
		Error("\t\tWrong component decraration");
    }
    template < class C > 
    C* Add_Component ( C* c ) {
		DEBUG ( "- - - Adding Component to: ", _name );
		if ( std::is_base_of<Behaviour, C>::value ) {
			c->obj = this;
			_components.append ( ( Behaviour * ) ( c ) );
			if ( _started ) { c->_Start( ); }
        	return c;
		}
		Error("\t\tWrong component Behaviour");
    }
	
    template < class C > 
	C* Get_Component ( ) {
		DEBUG ( "- - - Getting Component" );
		auto c = _components.Get_Begin ( );
		while ( c != nullptr ) {
			if ( typeid(*(c->data)) == typeid (C) ) { return (C*)c->data; }
			c = c->next;
		}
		DEBUG ( "\t\tComponent Not Found" );
		return nullptr;
	}
	template < class C > 
	List < C* > Get_Component_Recursive ( ) {
		DEBUG ( "Getting Components" );
		List < C* > L;
		auto c = _components.Get_Begin ( );
		while ( c != nullptr ) {
			if ( typeid(*(c->data)) == typeid (C) ) { L.append ( (C*)c->data ); }
			c = c->next;
		}
        auto O = _childrens.Get_Begin ( );
		while ( O != nullptr ) {
			L.append ( &O->data->Get_Component_Recursive < C > ( ) );
			O = O->next;
		}
		return L;
	}

	template < typename C >
	bool Has_Component ( ) {
		auto c = _components.Get_Begin ( );
		while ( c != nullptr ) {
			if ( typeid(*(c->data)) == typeid (C) ) { return true; }
			c = c->next;
		}
		return false;
	}

    virtual void Start ( ) {
		if ( _started ) { return; }
		if ( !_active ) { return; }
		DEBUG ( "Starting Objekt: " + Get_Name() );
        auto C = _components.Get_Begin ( );
		while ( C != nullptr ) {
			DEBUG ( " - Starting Componenet: ", std::string(typeid(*(C->data)).name()) );
			C->data->_Start ( );
			C = C->next;
			DEBUG ( " - Componenet Started" );
		}
		DEBUG ( " - - Starting Childrens" );
        auto O = _childrens.Get_Begin ( );
		while ( O != nullptr ) {
			O->data->Start ( );
			O = O->next;
		}
		DEBUG ( " - - Childrens Started" );
		DEBUG ( "Started Objekt: " + Get_Name() );
		_started = true;
    }

    virtual void Update ( ) {
		if ( !_active ) { return; }
		DEBUG ( "Updating Objekt: " + Get_Name() );
        auto C = _components.Get_Begin ( );
		while ( C != nullptr ) {
			DEBUG ( " - Updating Componenet: ", std::string(typeid(*C->data).name()));
			C->data->_Update ( );
			C = C->next;
		}
		DEBUG ( " - - Updating Childrens" );
		auto O = _childrens.Get_Begin ( );
		while ( O != nullptr ) {
			O->data->Update ( );
			O = O->next;
		}
		DEBUG ( " - - Childrens Updated" );
		DEBUG ( "Updated Objekt: " + Get_Name() );
    }

    virtual void Fixed_Update ( ) {
		if ( !_active ) { return; }
		DEBUG ( "Updating Fixed Objekt: " + Get_Name() );
        auto C = _components.Get_Begin ( );
		while ( C != nullptr ) {
			DEBUG ( " - Updating Fixed Componenet: ", std::string(typeid(*C->data).name()));
			C->data->_Fixed_Update ( );
			C = C->next;
		}
		DEBUG ( " - - Updating Childrens Fixed" );
		auto O = _childrens.Get_Begin ( );
		while ( O != nullptr ) {
			O->data->Fixed_Update ( );
			O = O->next;
		}
		DEBUG ( " - - Childrens Updated Fixed" );
		DEBUG ( "Updated Fixed Objekt: " + Get_Name() );
    }

	virtual void Andle_Collsions ( Objekt * collider, float trigger = false ) {
		if ( trigger ) { // the collision is of thigger type
			auto C = _components.Get_Begin ( );
			while ( C != nullptr ) {
				C->data->_Collision_Trigger ( collider );
				C = C->next;
			}
		} else {
			auto C = _components.Get_Begin ( );
			while ( C != nullptr ) {
				C->data->_Collision ( collider );
				C = C->next;
			}
		}
		
		auto O = _childrens.Get_Begin ( );
		while ( O != nullptr ) {
			O->data->Andle_Collsions ( collider, trigger );
			O = O->next;
		}
	} 

	mat4 Get_Model_Mat ( ) {
		vec3 Pivot = { (_rot_pivot.x + 0.5) * _size.x, (_rot_pivot.y + 0.5) * _size.y, (_rot_pivot.z) * _size.z };
		vec3 position = _pos;
		if ( _father != nullptr ) { position+= _father->Get_Pos(); }

		mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        if ( _rot.x != 0 ) { model = glm::rotate(model, glm::radians(_rot.x), glm::vec3(1.0f, 0.0f, 0.0f)); }
    	if ( _rot.y != 0 ) { model = glm::rotate(model, glm::radians(_rot.y), glm::vec3(0.0f, 1.0f, 0.0f)); }
    	if ( _rot.z != 0 ) { model = glm::rotate(model, _rot.z, glm::vec3(0.0f, 0.0f, 1.0f)); }
        model = glm::translate(model, -Pivot);

        model = glm::scale(model, _size);

		return model;
	}

	void Print_Tree ( std::string level ) {
		std::cout << level << _name << ( _active ? " v" : " x") << '\n';
		auto _C = _components.Get_Begin ( );
		while ( _C != nullptr ) {
			std::cout << level << "+ " << typeid(*_C->data).name() << ( _C->data->Get_Active () ? " v" : " x") << '\n';
			_C = _C->next;
		}
		level += "- ";
		auto C = _childrens.Get_Begin ( );
		while ( C != nullptr ) {
			C->data->Print_Tree ( level );
			C = C->next;
		}
	}
};

namespace Manager {
	static List < Objekt* > objekts;
	static Objekt* _current_scene = nullptr;

	static void Start ( ) 
	{ if ( _current_scene != nullptr ) { _current_scene->Start(); } };
	static void Update ( )
	{ if ( _current_scene != nullptr ) { _current_scene->Update(); } };

	static Objekt* Objekt_Load ( std::string name, vec3 pos = {0,0,0}, vec3 size = {100,100,100}, vec3 rot = {0,0,0}, vec3 rot_pivot = {0,0,0} ) {
		auto o = new Objekt(name, pos, size, rot, rot_pivot);
		objekts.append ( o );
		return o;
	}
	static Objekt* Objekt_Load ( Objekt * o ) 
	{ objekts.append( o ); return o;}
	static void Set_Active_Scene ( Objekt * o ) {
		if ( !objekts.contains( o ) ) { objekts.append ( o ); }
		_current_scene = o;
		Start ( );
	}
	static void Set_Active_Scene ( std::string s ) {
		std::cout << "adding acrive scene\n";
		auto S = objekts.Get_Begin ( );
		while ( S != nullptr ) {
			if ( S->data->Get_Name () == s ) 
			{ _current_scene = S->data; Start ( ); std::cout << "added main scene"; break; }
			S = S->next;
		}
	}
}

#endif