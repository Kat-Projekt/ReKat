#pragma once
#include "../global.h"
using namespace ReKat::grapik::Input;

class Exact : public Behaviour {
private:
    float speed = 3.0f;
public:
    void Start ( ) { }

    void Update ( ) {
        Manager::Object_Get ( name )->Move( Manager::Object_Get("Spada")->Get_pos() );
		// std::cout << "pos: " << Manager::Object_Get("Spada")->Get_pos().x << " x " << Manager::Object_Get("Spada")->Get_pos().y <<'\n';
	}
};