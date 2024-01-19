#ifndef OBJEKT_H
#define OBJEKT_H

template < typename T >
class _behaviour {
public:
    T* obj;
    virtual void Start ( ) { }
    virtual void Update ( ) { }

    virtual void Collision ( T* obj ) { }
    virtual void CollisionTrigger ( T* obj ) { }
};

#include "Utility/math.h"
#include <string>

class Objekt {
protected:
    std::string _name;
	bool _active = true;

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
	: _name(name), _size(size), _rot(rot), _rot_pivot(rot_pivot) { }

	void Set_father ( Objekt * father ) { _father = father; }
    void Add_Child ( Objekt * child ) {
		child->Set_father ( this );
		_childrens.append ( child );
	}
	void Rem_Child ( std::string name ) {
		auto O = Get_Children ( name );
		std::cout << "finding gio: " << O << '\n';
		_childrens.Print ( );
		_childrens.remove ( O );
		_childrens.Print ( );
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

	void Set_Pos ( vec3 pos = {0,0,0} ) { _pos = pos; }
	vec3 Get_Pos ( ) { return _pos; }

	void Set_Rot ( vec3 rot = {0,0,0} ) { _rot = rot; }
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
        	return c;
		}
		Error("Wrong component decraration");
		return nullptr;
    }
    template < class C > 
    C* Add_component ( C* c ) {
		if ( std::is_base_of<Behaviour, C>::value ) {
			c.obj = this;
			_components.append ( ( Behaviour * ) ( c ) );
        	return c;
		}
		Error("Wrong component Behaviour");
		return nullptr;
    }
	
    template < class C > 
	C* Get_component ( ) {
		auto C = _components.Get_Begin ( );
		while ( C != nullptr ) {
			if ( typeid(*(C->data)) == typeid (C) ) { return (C*)c; }
			C = C->next;
		}
		return nullptr;
	}

    virtual void Start ( ) {
		if ( !_active ) { return; }
        auto C = _components.Get_Begin ( );
		while ( C != nullptr ) {
			C->data->Start();
			C = C->next;
		}
        auto O = _childrens.Get_Begin ( );
		while ( O != nullptr ) {
			O->data->Start();
			O = O->next;
		}
    }

    virtual void Update ( ) {
		if ( !_active ) { return; }
        auto C = _components.Get_Begin ( );
		while ( C != nullptr ) {
			C->data->Update();
			C = C->next;
		}
		auto O = _childrens.Get_Begin ( );
		while ( O != nullptr ) {
			O->data->Update();
			O = O->next;
		}
    }

	mat4 Get_Model_Mat ( ) {
		// modify pos based on pivot (alwais on center)
        vec3 position = _pos + _father->Get_Pos() + vec3{ _rot_pivot.x * _size.x, _rot_pivot.y * _size.y, _rot_pivot.z * _size.z };

		mat4 rot_x = glm::rotate(glm::mat4(1.0f), glm::radians(_rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
    	mat4 rot_y = glm::rotate(glm::mat4(1.0f), glm::radians(_rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
    	mat4 rot_z = glm::rotate(glm::mat4(1.0f), glm::radians(_rot.z), glm::vec3(0.0f, 0.0f, 1.0f));

		_model = glm::translate(glm::mat4(1.0f), position) * ( rot_x * rot_y * rot_z ) * glm::scale(glm::mat4(1.0f), _size);
		
		return _model;
	}
};

#endif