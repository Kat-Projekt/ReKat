#ifndef BUTTON
#define BUTTON
#include "../objekt.h"
#include "../graphik.hpp"
using namespace ReKat::grapik::Input;

class Button : public Behaviour {
private:
    bool hower = false;
    bool click = false;
    int  start_frame = 0;
    void (* _call ) ();
	void (Behaviour::*_b_call) ();
public:
	void UI_Update ( ) {
		// calculate hover
		vec2 Pos = obj->Get_Pos ( );
		vec2 Size = obj->Get_Size ( );
        if ( ( mouse_pos.x - Pos.x > 0 && mouse_pos.x - Pos.x < Size.x ) && 
             ( mouse_pos.y - Pos.y > 0 && mouse_pos.y - Pos.y < Size.y ) ) 
        { hower = true; } else { hower = false; }

		if ( Key_Down ("Mouse1") && hower ) {
			if ( _call != nullptr ) { (*_call) (); }
			if ( _b_call != nullptr ) { (*this.*_b_call) (); }
		}
	}
};

#endif