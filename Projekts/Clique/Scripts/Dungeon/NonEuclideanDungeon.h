#ifndef _NonEuclideanDungeon
#define _NonEuclideanDungeon

#include <Graphik/objekt.h>
#include <Graphik/graphik.hpp>
using namespace ReKat::grapik::Input;

class NonEuclideanDungeon : public Behaviour {
public:
    virtual void Start ( ) { }
    virtual void Update ( ) { }
	virtual void Fixed_Update ( ) { }
	virtual void UI_Update ( ) { }

    virtual void Collision ( Objekt* _obj ) { _obj; }
    virtual void Collision_Trigger ( Objekt* _obj ) { _obj; }
};

#endif