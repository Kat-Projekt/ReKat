#ifndef OBJEKT_H
#define OBJEKT_H

#include "debugger.hpp"

template < typename T >
class _behaviour {
	bool _active = true;
	bool _started = false;
public:
    T* obj;

	void _Start ( ) { if ( _active && !_started ) { Start ( ); } _started = true; }
	void _Update ( ) { if ( _active ) { Update ( ); } }
	void _Fixed_Update ( ) { if ( _active ) { Fixed_Update ( ); } }

	void _Collision ( T* _obj ) { if ( _active ) { Collision ( _obj ); } }
	void _Collision_Trigger ( T* _obj ) { if ( _active ) { Collision_Trigger ( _obj ); } }

    virtual void Start ( ) { }
    virtual void Update ( ) { }
	virtual void Fixed_Update ( ) { }

    virtual void Collision ( T* _obj ) { _obj; }
    virtual void Collision_Trigger ( T* _obj ) { _obj; }

	void Set_Active ( bool active ) 
	{ _active = active; if ( !_started ) { Start( ); } }
	bool Get_Active ( ) { return _active; }

	virtual void Delete ( ) {
		delete this;
	}
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

    Objekt ( ) { DEBUG ( 4,"Created Empty Objekt"); }
    Objekt ( std::string name, vec3 pos = {0,0,0}, vec3 size = {100,100,100}, vec3 rot = {0,0,0}, vec3 rot_pivot = {0,0,0} ) 
	: _name(name), _pos(pos), _size(size), _rot(rot), _rot_pivot(rot_pivot) 
	{ DEBUG ( 4,"Inizializing Objekt: ", name, ", pos: ", pos, ", size: ", size, ", rot: ", rot, ", rot_pivot: ", rot_pivot ); }
	void Free ( ) {
		auto c = _components.Get_Begin ( );
		while ( c != nullptr ) 
		{ c->data->Delete ( ); c = c->next; }

		DEBUG ( 4,"Freeing Objekt: ", _name );
		auto ci = _childrens.Get_Begin ( );
		while ( ci != nullptr ) 
		{ ci->data->Free ( ); ci = ci->next; }

		_childrens.Deallocate ( );
		DEBUG ( 5,"Freed Childrens of: ", _name );
		_components.Deallocate ( );
		DEBUG ( 5,"Freed Components of: ", _name );

		delete this;
		DEBUG ( 4, _name, " is Free" );
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
		_childrens.remove ( O );
		DEBUG ( 3,"\tremoving child: ", O->Get_Name (), " from: ", _name );
	}
	void Rem_Child ( Objekt* O ) {
		_childrens.remove ( O );
		DEBUG ( 3,"\tremoving child: ", O->Get_Name (), " from: ", _name );
	}
	void Delete ( std::string p = "" ) {
		if ( _father != nullptr ) { _father->Rem_Child ( this ); }
		_active = false;
		DEBUG ( 3, p + "Deleting Objekt: ", _name );
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
	bool Has_Children ( Objekt* child ) {
		auto C = _childrens.Get_Begin ( );
		while ( C != nullptr ) {
			if ( C->data == child ) 
			{ return true; }
			if ( C->data->Has_Children ( child ) ) { return true; }
			C = C->next;
		}
		return false;
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
		auto c = _components.Get_Begin ( );
		while ( c != nullptr ) {
			if ( typeid(*(c->data)) == typeid (C) ) { return (C*)c->data; }
			c = c->next;
		}
		DEBUG ( 3,"\t\tComponent Not Found" );
		return nullptr;
	}
	template < class C > 
	List < C* > Get_Component_Recursive ( ) {
		DEBUG ( 6,"Getting Components" );
		List < C* > L;
		auto c = _components.Get_Begin ( );
		while ( c != nullptr ) {
			if ( typeid(*(c->data)) == typeid (C) ) { L.append ( (C*)c->data ); }
			c = c->next;
		}
        auto O = _childrens.Get_Begin ( );
		while ( O != nullptr ) {
			auto Data = O->data->Get_Component_Recursive < C > ( );
			L.append ( &Data );
			O = O->next;
		}
		if ( L.size () == 0 ) { DEBUG ( 3,"No components found");}
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
		DEBUG ( 4,"Starting Objekt: " + Get_Name() );
        auto C = _components.Get_Begin ( );
		while ( C != nullptr ) {
			DEBUG ( 6," - Starting Componenet: ", std::string(typeid(*(C->data)).name()) );
			C->data->_Start ( );
			C = C->next;
			DEBUG ( 6," - Componenet Started" );
		}
		DEBUG ( 6,"Starting Childrens" );
        auto O = _childrens.Get_Begin ( );
		while ( O != nullptr ) {
			O->data->Start ( );
			O = O->next;
		}
		DEBUG ( 6,"Childrens Started" );
		DEBUG ( 5,"Started Objekt: " + Get_Name() );
		_started = true;
    }

    virtual void Update ( ) {
		if ( !_active ) { return; }
		DEBUG ( 4,"Updating Objekt: " + Get_Name() );
        auto C = _components.Get_Begin ( );
		while ( C != nullptr ) {
			DEBUG ( 6,"Updating Componenet: ", std::string(typeid(*C->data).name()));
			C->data->_Update ( );
			C = C->next;
		}
		DEBUG ( 6,"Updating Childrens" );
		auto O = _childrens.Get_Begin ( );
		while ( O != nullptr ) {
			O->data->Update ( );
			O = O->next;
		}
		DEBUG ( 6,"Childrens Updated" );
		DEBUG ( 5,"Updated Objekt: " + Get_Name() );
    }

    virtual void Fixed_Update ( ) {
		if ( !_active ) { return; }
		DEBUG ( 5,"Updating Fixed Objekt: " + Get_Name() );
        auto C = _components.Get_Begin ( );
		while ( C != nullptr ) {
			DEBUG ( 6," - Updating Fixed Componenet: ", std::string(typeid(*C->data).name()));
			C->data->_Fixed_Update ( );
			C = C->next;
		}
		DEBUG ( 6,"Updating Childrens Fixed" );
		auto O = _childrens.Get_Begin ( );
		while ( O != nullptr ) {
			O->data->Fixed_Update ( );
			O = O->next;
		}
		DEBUG ( 6,"Childrens Updated Fixed" );
		DEBUG ( 5,"Updated Fixed Objekt: " + Get_Name() );
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

	static void Start ( ) { if ( _current_scene != nullptr ) { 
		color ( "STARTING\n", BACKGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY ); 
		_current_scene->Start(); 
	} };
	static void Update ( ) { if ( _current_scene != nullptr ) { 
		color ( "UPDATING\n", BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY );
		_current_scene->Update();
	} };

	static Objekt* Objekt_Load ( std::string name, vec3 pos = {0,0,0}, vec3 size = {100,100,100}, vec3 rot = {0,0,0}, vec3 rot_pivot = {0,0,0} ) {
		auto o = new Objekt(name, pos, size, rot, rot_pivot);
		objekts.append ( o );
		return o;
	}
	static Objekt* Objekt_Load ( Objekt * o ) 
	{ objekts.append( o ); return o;}
	static void Set_Active_Scene ( Objekt * o ) {
		DEBUG ( 3,"adding active scene" );
		if ( !objekts.contains( o ) ) { objekts.append ( o ); }
		_current_scene = o;
		Start ( );
		DEBUG ( 5,"added main scene" );
	}
	static void Set_Active_Scene ( std::string s ) {
		DEBUG ( 3,"adding active scene" );
		auto S = objekts.Get_Begin ( );
		while ( S != nullptr ) {
			if ( S->data->Get_Name () == s ) 
			{ _current_scene = S->data; Start ( ); DEBUG ( 5,"added main scene" ); break; }
			S = S->next;
		}
	}
	
	static Objekt* Get_Active_Scene ( ) { return _current_scene; }
}

#endif