#pragma once
#include "../global.h"
using namespace ReKat::grapik::Input;

class Dialog : public Behaviour {
private:
	bool now_collision = false;
	bool ended = false;
	int current_line = 0;
	std::vector < std::string > lines = {"Ciao", "come va", "balle"};
public:
	void Start ( ) {
		ended = false;
		current_line = 0;
		Manager::UI_Object_Get("Dialog")->Set_text(lines[current_line]);
	}
    void Update ( ) {
		if ( ended ) { return; }
		Manager::UI_Object_Get("Dialog")->Set_Active(now_collision);
		// advance dialog
		if ( now_collision ) {
			if ( Key_Down ( "F" ) ) {
				if ( ! ( current_line < lines.size() - 1 ) ) {
					ended = true;
					Manager::UI_Object_Get("Dialog")->Set_Active ( false );
				} else { current_line++; }
				Manager::UI_Object_Get("Dialog")->Set_text(lines[current_line]);
			}
		}
		now_collision = false;
	}

	void Collision ( std::string obj ) {  }
	void CollisionTrigger ( std::string obj ) { if ( obj == "Player" ) { now_collision = true; } }
};