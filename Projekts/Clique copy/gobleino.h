#ifndef _gobleino
#define _gobleino

#include <Graphik/objekt.h>
#include <Graphik/Components/_catalog.hpp>
#include <Graphik/graphik.hpp>
using namespace ReKat::grapik::Input;

class Gobleino : public Behaviour {
public:
	float speed = 100;
	Objekt * target;
    virtual void Start ( ) { }
    virtual void Update ( ) {
		glm::vec3 vector = target->Get_Pos() - obj->Get_Pos();
        
        if ( vector == vec3{0,0,0} ) { return; }
        vector = normalize(vector);
        obj->Get_component < Rigidbody > ()->velocity = ( vector  * speed );
	}
	virtual void Fixed_Update ( ) { }
	virtual void UI_Update ( ) { }

    virtual void Collision ( Objekt* _obj ) { _obj; }
    virtual void Collision_Trigger ( Objekt* _obj ) {
		std::cout << obj->Get_Name () << '\n';
		if ( obj->Get_Name () == "Bean" ) { speed = 50; return; }
		obj->Set_Active ( false );
	}
};

#endif