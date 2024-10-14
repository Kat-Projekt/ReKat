#ifndef BUTTON
#define BUTTON
#include "../../objekt.hpp"
#include "../graphik.hpp"
using namespace ReKat::grapik::Input;

typedef void (Behaviour::*Behaviour_Fun) (void); 

class Button : public Behaviour {
private:
    bool hower = false;
    bool click = false;
    int  start_frame = 0;
    void (* _call ) () = nullptr;
	Behaviour* _cla;
	Behaviour_Fun _b_call = nullptr;
public:
	void Update ( ) {
		// calculate hover
		vec2 Pos = obj->Get_Pos ( );
		vec2 Size = obj->Get_Size ( );
        if ( ( mouse_pos.x - Pos.x > 0 && mouse_pos.x - Pos.x < Size.x ) && 
             ( mouse_pos.y - Pos.y > 0 && mouse_pos.y - Pos.y < Size.y ) ) 
        { hower = true; } else { hower = false; }

		if ( Key_Down ("Mouse1") && hower ) {
			if ( _call != nullptr ) { (*_call) (); }
			if ( _b_call != nullptr ) { std::invoke ( _b_call, _cla ); }
		}
	}

	template < class C >
	void Set ( C* _class, void ( C::*_fun ) ( void ) ) {
		DEBUG ( 4,"Adding Function from class: ", std::string(typeid(*_class).name()) );
		if ( std::is_base_of<Behaviour, C>::value ) {
			_cla = ( Behaviour* ) _class;
            _b_call = ( Behaviour_Fun ) _fun;
			return;
		}
		DEBUG ( 2, "Wrong component Behaviour" );
	}
};

#endif