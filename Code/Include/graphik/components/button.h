#ifndef BUTTON
#define BUTTON
#include "../../objekt.hpp"
#include "../../objekt.hpp"
#include "../graphik.hpp"
using namespace ReKat::grapik::Input;

typedef void (Behaviour::*Behaviour_Fun) (void); 

class Button : public Behaviour {
private:
    std::string _click_is = "Mouse1";

	bool hover = false;
    bool click = false;

    void ( *_click_call ) ( ) = nullptr;
	void ( Behaviour::*_b_click_call ) ( ) = nullptr;
    void ( *_press_call ) ( ) = nullptr;
	void ( Behaviour::*_b_press_call ) ( ) = nullptr;
    void ( *_release_call ) ( ) = nullptr;
	void ( Behaviour::*_b_release_call ) ( ) = nullptr;
    void ( *_hover_call ) ( ) = nullptr;
	void ( Behaviour::*_b_hover_call ) ( ) = nullptr;
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
        { hover = true; } else { hover = false; }

		if ( hover ) {
			// call hover functions
			if ( _hover_call != nullptr )	{ _hover_call ( ); }
			if ( _b_hover_call != nullptr )	{ ( *this.*_b_hover_call ) ( ); }

			if ( Key_Down ( _click_is ) ) { // call on click function
				if ( _click_call != nullptr )	{ _click_call ( ); }
				if ( _b_click_call != nullptr )	{ ( *this.*_b_click_call ) ( ); }
			}

			if ( Key_Pressed ( _click_is ) ) { // call on press function
				if ( _press_call != nullptr )	{ _press_call ( ); }
				if ( _b_press_call != nullptr )	{ ( *this.*_b_press_call ) ( ); }
			}

			if ( Key_Up ( _click_is ) ) { // call on release function
				if ( _release_call != nullptr )	{ _release_call ( ); }
				if ( _b_release_call != nullptr )	{ ( *this.*_b_release_call ) ( ); }
			}
		}
	}

	Button* OnClick ( void ( Behaviour::*b_call ) ( ) ) 
	{ _b_click_call = b_call; return this; }
	Button* OnClick ( void ( *call ) ( ) ) 
	{ _click_call = call; return this; }

	Button* OnPress ( void ( Behaviour::*b_call ) ( ) ) 
	{ _b_press_call = b_call; return this; }
	Button* OnPress ( void ( *call ) ( ) ) 
	{ _press_call = call; return this; }

	Button* OnRelease ( void ( Behaviour::*b_call ) ( ) ) 
	{ _b_release_call = b_call; return this; }
	Button* OnRelease ( void ( *call ) ( ) ) 
	{ _release_call = call; return this; }

	Button* OnHover ( void ( Behaviour::*b_call ) ( ) ) 
	{ _b_hover_call = b_call; return this; }
	Button* OnHover ( void ( *call ) ( ) ) 
	{ _hover_call = call; return this; }

	Button* Set ( std::string click_is)
	{ _click_is = click_is; return this; }
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