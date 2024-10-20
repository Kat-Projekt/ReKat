#ifndef OBJEKT_H
#define OBJEKT_H

//#define DIAGNOSTIC
#ifdef DIAGNOSTIC 
#define DEBUG(msg) std::cout << msg
#else
#define DEBUG(msg)
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

	void Set_Father ( Objekt * father ) { _father = father; }
	Objekt * Get_Father ( ) { return _father; }
    Objekt * Add_Child ( Objekt * child ) {
		child->Set_Father ( this );
		_childrens.append ( child );
		if ( _started ) { child->Start( ); }
		return this;
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
        if ( std::is_base_of<Behaviour, C>::value ) { 
			C* c = new C ( );
			c->obj = this;
			_components.append ( ( Behaviour * ) ( c ) );
			if ( _started ) { c->_Start( ); }
        	return c;
		}
		Error("Wrong component decraration");
    }
    template < class C > 
    C* Add_Component ( C* c ) {
		if ( std::is_base_of<Behaviour, C>::value ) {
			c->obj = this;
			_components.append ( ( Behaviour * ) ( c ) );
			if ( _started ) { c->_Start( ); }
        	return c;
		}
		Error("Wrong component Behaviour");
    }
	
    template < class C > 
	C* Get_Component ( ) {
		auto c = _components.Get_Begin ( );
		while ( c != nullptr ) {
			if ( typeid(*(c->data)) == typeid (C) ) { return (C*)c->data; }
			c = c->next;
		}
		return nullptr;
	}
	template < class C > 
	List < C* > Get_Component_Recursive ( ) {
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
		DEBUG ( "Starting obj: " + Get_Name() + "\n" );
		if ( _started ) { return; }
		if ( !_active ) { return; }
        auto C = _components.Get_Begin ( );
		while ( C != nullptr ) {
			DEBUG ( " - Starting comp: " + std::string(typeid(*(C->data)).name()) + "\n" );
			C->data->_Start ( );
			C = C->next;
			DEBUG ( " - Started\n" );
		}
		DEBUG ( " - Updating Subobjs\n" );
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
			C->data->_Update ( );
			C = C->next;
		}
		DEBUG ( "Updated obj: " + Get_Name() + "\n" );
    }

    virtual void Fixed_Update ( ) {
		if ( !_active ) { return; }
        auto C = _components.Get_Begin ( );
		while ( C != nullptr ) {
			C->data->_Fixed_Update ( );
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

	std::string Print_Tree ( ) {
		std::string out = _name;
		auto O = _childrens.Get_Begin ( );
		while ( O != nullptr ) {
			out += " : {" + O->data->Print_Tree () + '}';
			O = O->next;
		}
		
		return out;
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
		std::cout << "changing scene\n";
		std::cout << s << '\n';
		std::cout << _current_scene << '\n';
		std::cout << "scenes" << _scenes << '\n';

		auto S = _scenes.Get_Begin ( );
		while ( S != nullptr ) {
			std::cout << S->data->Get_Name () << '\n';
			if ( S->data->Get_Name () == s ) 
			{ _current_scene = S->data; Start ( ); break; }
			S = S->next;
		}
		std::cout << _current_scene << '\n';
	}
}

#endif