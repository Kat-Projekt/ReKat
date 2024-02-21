#ifndef _Spell
#define _Spell

#include <Graphik/objekt.h>
#include <Graphik/graphik.hpp>
#include <Graphik/Components/_catalog.hpp>
using namespace ReKat::grapik::Input;

class Spell : public Behaviour {
public:
	vec3 target;
	Camera * cam;
	
	std::string ignore = "Player";

    virtual void Start ( ) { }
    virtual void Update ( ) { }
	virtual void Fixed_Update ( ) { }
	virtual void UI_Update ( ) { }

    virtual void Collision ( Objekt* _obj ) {
		if ( _obj->Get_Name ( ) == ignore 
		|| _obj->Get_Name ( ) == "weapon"  || _obj->Get_Name ( ) == "speel" 
		|| _obj->Get_Name ( ) == "!weapon" || _obj->Get_Name ( ) == "!speel" ) { return; }
		obj->Set_Active(false); 
	}

	virtual void Collision_Trigger ( Objekt* _obj ) {
		if ( _obj->Get_Name ( ) == ignore 
		|| _obj->Get_Name ( ) == "weapon"  || _obj->Get_Name ( ) == "speel" 
		|| _obj->Get_Name ( ) == "!weapon" || _obj->Get_Name ( ) == "!speel" ) { return; }
		obj->Set_Active(false); 
	}
};

#endif