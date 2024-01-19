#pragma once
#include "../global.h"
using namespace ReKat::grapik::Input;

class Pointer : public Behaviour {
private:
public:
	void Start ( ) {
		std::cout << "\nstarted\n";
		for ( auto e : Manager::UI_Objects ) { 
			std::cout << e.first << " : " << e.second << '\n'; 
			if ( e.first == name ) { std::cout << "---------------- found ----------------\n"; }
		}
	}
    void Update ( ) {
		Manager::UI_Object_Get(name)->Move(mouse_pos);
	}
};