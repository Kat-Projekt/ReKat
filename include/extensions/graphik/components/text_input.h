#ifndef INPUT_BOX
#define INPUT_BOX

#include "../../objekt.hpp"
#include "../graphik.hpp"

class Input_Box : public Behaviour {
	std::string _click_is = "Mouse1";
	std::string input = "";
	std::string * text_updater = nullptr;
	bool Getting_string = false;
	
	void Start ( ) {

	}

	void Update ( ) {
		// calculate hover
		vec2 Pos = obj->Get_Pos ( );
		vec2 Size = obj->Get_Size ( ) * 0.5f;
		vec2 Mouse_pos = ReKat::Graphik::_current_window->input.mouse_pos;

		if (	( Mouse_pos.x - Pos.x > -Size.x && Mouse_pos.x - Pos.x < Size.x ) && 
             		( Mouse_pos.y - Pos.y > -Size.y && Mouse_pos.y - Pos.y < Size.y ) ) 
        	{ // hovering
			if ( ReKat::Graphik::Key_Down ( _click_is ) ) { Getting_string = true; }
			DEBUG (4, "in bounds" );
		} else { // click out of box
			DEBUG (4, "out of bounds" );
			if ( ReKat::Graphik::Key_Down ( _click_is ) ) { Getting_string = false; }
		}
		if ( Getting_string ) {
			DEBUG ( 4, "Getting caracter" );
			// delete
			if ( ReKat::Graphik::Key_Down ( "Del" ) && input != "" ) {
				input.pop_back ( );
			} else if ( ReKat::Graphik::Key_Down ( "Enter" )) {
				input += "'\n";
			} else {
				input += ReKat::Graphik::_current_window->input.typed;
			}
		}
		if ( text_updater == nullptr ) { return; }
		*text_updater = input;
	}
public:

	Input_Box * Set ( std::string * point_text ) {
		text_updater = point_text;
		return this;
	}
};

#endif