#ifndef OBJEKT_H
#define OBJEKT_H

// #define DIAGNOSTIC
#ifdef DIAGNOSTIC 
#define DEBUG(msg) std::cout << msg
#else
#define DEBUG(msg)
#endif

template < typename T >
class _behaviour {
public:
    T* obj;
    virtual void Start ( ) { }
    virtual void Update ( ) { }
	virtual void Fixed_Update ( ) { }
	virtual void UI_Update ( ) { }

    virtual void Collision ( T* _obj ) { _obj; }
    virtual void Collision_Trigger ( T* _obj ) { _obj; }
};

#include "Utility/math.h"
#include <string>

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

    Objekt ( ) { }
    Objekt ( std::string name, vec3 pos = {0,0,0}, vec3 size = {100,100,100}, vec3 rot = {0,0,0}, vec3 rot_pivot = {0,0,0} ) 
	: _name(name), _pos(pos), _size(size), _rot(rot), _rot_pivot(rot_pivot) { }

	void Set_father ( Objekt * father ) { _father = father; }
    void Add_Child ( Objekt * child ) {
		child->Set_father ( this );
		_childrens.append ( child );
		if ( _started ) { child->Start( ); }
		std::cout << " - added obj: " << child << '\n';
		std::cout << " - objekts: " << _childrens << '\n';
	}
	void Rem_Child ( std::string name ) {
		auto O = Get_Children ( name );
		_childrens.Print ( );
		_childrens.remove ( O );
		_childrens.Print ( );
	}
	void Rem_Child ( Objekt* O ) {
		_childrens.Print ( );
		_childrens.remove ( O );
		_childrens.Print ( );
	}
	void Delete ( ) {
		if ( _father != nullptr ) { _father->Rem_Child ( this ); }
		_active = false;
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

	void Set_Pos ( vec3 pos = {0,0,0} ) { _pos = pos; }
	void Inc_Pos ( vec3 pos = {0,0,0} ) { _pos += pos; }
	vec3 Get_Pos ( ) { return _pos + ( _father != nullptr ? _father->Get_Pos() : vec3{0,0,0}); }

	void Set_Rot ( vec3 rot = {0,0,0} ) { _rot = rot; }
	void Set_2D_Rot ( float rot ) { _rot.z = rot; }
	vec3 Get_Rot ( ) { return _rot; }

	void Set_Size ( vec3 size = {0,0,0} ) { _size = size; }
	vec3 Get_Size ( ) { return _size; }

	void Set_Rot_Pivot ( vec3 rot_pivot = {0,0,0} ) { _rot_pivot = rot_pivot; }
	vec3 Get_Rot_Pivot ( ) { return _rot_pivot; }

    void Set_Active ( bool active ) { _active = active; }
    bool Get_Active ( ) { return _active; }

    void Set_Name ( std::string name ) { _name = name; }
    std::string Get_Name ( ) { return _name; }

    template < class C >
    C* Add_component ( ) {
        if ( std::is_base_of<Behaviour, C>::value ) { 
			C* c = new C ( );
			c->obj = this;
			_components.append ( ( Behaviour * ) ( c ) );
			if ( _started ) { c->Start( ); }
        	return c;
		}
		Error("Wrong component decraration");
    }
    template < class C > 
    C* Add_component ( C* c ) {
		if ( std::is_base_of<Behaviour, C>::value ) {
			c->obj = this;
			_components.append ( ( Behaviour * ) ( c ) );
			if ( _started ) { c->Start( ); }
        	return c;
		}
		Error("Wrong component Behaviour");
    }
	
    template < class C > 
	C* Get_component ( ) {
		auto c = _components.Get_Begin ( );
		while ( c != nullptr ) {
			if ( typeid(*(c->data)) == typeid (C) ) { return (C*)c->data; }
			c = c->next;
		}
		return nullptr;
	}
	template < class C > 
	List < C* > Get_component_recursive ( ) {
		List < C* > L;
		auto c = _components.Get_Begin ( );
		while ( c != nullptr ) {
			if ( typeid(*(c->data)) == typeid (C) ) { L.append ( (C*)c->data ); }
			c = c->next;
		}
        auto O = _childrens.Get_Begin ( );
		while ( O != nullptr ) {
			L.append ( &O->data->Get_component_recursive < C > ( ) );
			O = O->next;
		}
		return L;
	}

	template < typename C >
	bool Has_component ( ) {
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
        auto C = _components.Get_Begin ( );
		while ( C != nullptr ) {
			C->data->Start ( );
			C = C->next;
		}
        auto O = _childrens.Get_Begin ( );
		while ( O != nullptr ) {
			O->data->Start ( );
			O = O->next;
		}
		_started = true;
    }

    virtual void Update ( ) {
		if ( !_active ) { return; }
		auto O = _childrens.Get_Begin ( );
		while ( O != nullptr ) {
			O->data->Update ( );
			O = O->next;
		}
        auto C = _components.Get_Begin ( );
		DEBUG ( "Updating obj: " + Get_Name() + "\n" );
		while ( C != nullptr ) {
			DEBUG ( "Updating comp: " + std::string(typeid(*C->data).name()) + "\n" );
			C->data->Update ( );
			C = C->next;
		}
		DEBUG ( "Updated obj: " + Get_Name() + "\n" );
    }

    virtual void Fixed_Update ( ) {
		if ( !_active ) { return; }
        auto C = _components.Get_Begin ( );
		while ( C != nullptr ) {
			C->data->Fixed_Update ( );
			C = C->next;
		}
		auto O = _childrens.Get_Begin ( );
		while ( O != nullptr ) {
			O->data->Fixed_Update ( );
			O = O->next;
		}
    }

	virtual void Andle_Collsions ( Objekt * collider, float trigger = false ) {
		if ( trigger ) { // the collision is of thigger type
			auto C = _components.Get_Begin ( );
			while ( C != nullptr ) {
				C->data->Collision_Trigger ( collider );
				C = C->next;
			}
			auto O = _childrens.Get_Begin ( );
			while ( O != nullptr ) {
				O->data->Andle_Collsions ( collider, trigger );
				O = O->next;
			}
		}
		auto C = _components.Get_Begin ( );
		while ( C != nullptr ) {
			C->data->Collision ( collider );
			C = C->next;
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

	std::string Print_Tree ( ) {
		std::string out = _name;
		auto O = _childrens.Get_Begin ( );
		while ( O != nullptr ) {
			out += " : {" + O->data->Print_Tree () + '}';
			O = O->next;
		}
		
		return out;
	}
};

namespace Scene_Manager {
	static List < Objekt* > _scenes;
	static Objekt* _current_scene = nullptr;

	static void Start ( ) 
	{ if ( _current_scene != nullptr ) { _current_scene->Start(); } };
	static void Update ( )
	{ if ( _current_scene != nullptr ) { _current_scene->Update(); } };

	static void Add_Scene ( Objekt * o ) 
	{ _scenes.append( o ); }
	static void Set_Active_Scene ( Objekt * o ) {
		if ( !_scenes.contains( o ) ) { _scenes.append ( o ); }
		_current_scene = o;
		Start ( );
	}
	static void Set_Active_Scene ( std::string s ) {
		auto S = _scenes.Get_Begin ( );
		while ( S != nullptr ) {
			if ( S->data->Get_Name () == s ) 
			{ _current_scene = S->data; Start ( ); break; }
			S = S->next;
		}
	}
}

#endif