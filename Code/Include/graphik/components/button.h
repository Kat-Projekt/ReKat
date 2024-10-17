#ifndef BUTTON
#define BUTTON
#include <functional>
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
	Behaviour* _b_click_class = nullptr;
	Behaviour_Fun _b_click_call = nullptr;

    void ( *_press_call ) ( ) = nullptr;
	Behaviour* _b_press_class = nullptr;
	Behaviour_Fun _b_press_call = nullptr;

    void ( *_release_call ) ( ) = nullptr;
	Behaviour* _b_release_class = nullptr;
	Behaviour_Fun _b_release_call = nullptr;

    void ( *_hover_call ) ( ) = nullptr;
	Behaviour* _b_hover_class = nullptr;
	Behaviour_Fun _b_hover_call = nullptr;
public:
	void Update ( ) {
		// calculate hover
		vec2 Pos = obj->Get_Pos ( );
		vec2 Size = obj->Get_Size ( ) * 0.5f;
		std::cout << mouse_pos;
        if ( ( mouse_pos.x - Pos.x > -Size.x && mouse_pos.x - Pos.x < Size.x ) && 
             ( mouse_pos.y - Pos.y > -Size.y && mouse_pos.y - Pos.y < Size.y ) ) 
        { hover = true; } else { hover = false; }

		if ( hover ) {
			// call hover functions
			if ( _hover_call != nullptr )	{ _hover_call ( ); }
			if ( _b_hover_call != nullptr )	{ std::invoke ( _b_hover_call, _b_hover_class ); }

			if ( Key_Down ( _click_is ) ) { // call on click function
				if ( _click_call != nullptr )	{ _click_call ( ); }
				if ( _b_click_call != nullptr )	{ std::invoke ( _b_click_call, _b_click_class ); }
			}

			if ( Key_Pressed ( _click_is ) ) { // call on press function
				if ( _press_call != nullptr )	{ _press_call ( ); }
				if ( _b_press_call != nullptr )	{ std::invoke ( _b_press_call, _b_press_class ); }
			}

			if ( Key_Up ( _click_is ) ) { // call on release function
				if ( _release_call != nullptr )	{ _release_call ( ); }
				if ( _b_release_call != nullptr )	{ std::invoke ( _b_release_call, _b_release_class ); }
			}
		}
	}

	template < class C >
	Button* OnClick ( C* objekt_class, void ( C::*b_call ) ( void ) ) {
		DEBUG ( 4,"Adding Function from class: ", std::string(typeid(*objekt_class).name()) );
		if ( std::is_base_of<Behaviour, C>::value ) {
			_b_click_class = ( Behaviour* ) objekt_class;
			_b_click_call = ( Behaviour_Fun ) b_call;
			return this;
		}
		DEBUG ( 2, "Wrong component Behaviour" );
	}
	Button* OnClick ( void ( *call ) ( ) ) 
	{ _click_call = call; return this; }

	template < class C >
	Button* OnPress ( C* objekt_class, void ( C::*b_call ) ( void ) ) {
		DEBUG ( 4,"Adding Function from class: ", std::string(typeid(*objekt_class).name()) );
		if ( std::is_base_of<Behaviour, C>::value ) {
			_b_press_class = ( Behaviour* ) objekt_class;
			_b_press_call = ( Behaviour_Fun ) b_call;
			return this;
		}
		DEBUG ( 2, "Wrong component Behaviour" );
	}
	Button* OnPress ( void ( *call ) ( ) ) 
	{ _press_call = call; return this; }

	template < class C >
	Button* OnRelease ( C* objekt_class, void ( C::*b_call ) ( void ) ) {
		DEBUG ( 4,"Adding Function from class: ", std::string(typeid(*objekt_class).name()) );
		if ( std::is_base_of<Behaviour, C>::value ) {
			_b_release_class = ( Behaviour* ) objekt_class;
			_b_release_call = ( Behaviour_Fun ) b_call;
			return this;
		}
		DEBUG ( 2, "Wrong component Behaviour" );
	}
	Button* OnRelease ( void ( *call ) ( ) ) 
	{ _release_call = call; return this; }

	template < class C >
	Button* OnHover ( C* objekt_class, void ( C::*b_call ) ( void ) ) {
		DEBUG ( 4,"Adding Function from class: ", std::string(typeid(*objekt_class).name()) );
		if ( std::is_base_of<Behaviour, C>::value ) {
			_b_hover_class = ( Behaviour* ) objekt_class;
			_b_hover_call = ( Behaviour_Fun ) b_call;
			return this;
		}
		DEBUG ( 2, "Wrong component Behaviour" );
	}
	Button* OnHover ( void ( *call ) ( ) ) 
	{ _hover_call = call; return this; }

	Button* Set ( std::string click_is)
	{ _click_is = click_is; return this; }
};

#endif