#ifndef _Weaponer
#define _Weaponer

#include "Staff.h"
#include "Sword.h"

class Weaponer : public Behaviour {
	List < Weapon * > Wepons;
	Weapon * active_wepon = nullptr;
public:
    virtual void Start ( ) {
		// Wepons = obj->Get_Component_Recursive < Weapon > ( );
		// active_wepon = Wepons[(integer)0];
	}
    virtual void Update ( ) {
		if ( Key_Down ( "Mouse1" ) ) 
		{ active_wepon->Start_Attack ( angle( mouse_pos ) ); }
	}
	virtual void Fixed_Update ( ) { }
	virtual void UI_Update ( ) { }

    virtual void Collision ( Objekt* _obj ) { _obj; }
    virtual void Collision_Trigger ( Objekt* _obj ) { _obj; }
};

#endif