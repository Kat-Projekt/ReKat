#ifndef _Dungeon
#define _Dungeon

#include <Graphik/objekt.h>
#include <Graphik/graphik.hpp>
using namespace ReKat::grapik::Input;

class Dungeon : public Behaviour {
public:
    virtual void Start ( ) { }
    virtual void Update ( ) { }
	virtual void Fixed_Update ( ) { }
	virtual void UI_Update ( ) { }

    virtual void Collision ( Objekt* _obj ) { _obj; }
    virtual void Collision_Trigger ( Objekt* _obj ) { _obj; }
};

#endif