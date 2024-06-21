#ifndef _Staff
#define _Staff

#include "Weapon.h"

class Staff : public Weapon {
public:
    virtual void Start ( ) { }
    virtual void Update ( ) { }
	virtual void Fixed_Update ( ) { }
	virtual void UI_Update ( ) { }

    virtual void Collision ( Objekt* _obj ) { _obj; }
    virtual void Collision_Trigger ( Objekt* _obj ) { _obj; }
};

#endif