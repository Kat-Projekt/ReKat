#ifndef _hello_world
#define _hello_world

#include <engine.hpp>
using namespace ReKat::grapik::Input;

class hello_world : public Behaviour {
public:
	virtual void Start ( ) { }
	virtual void Update ( ) {
		std::cout << "cacca";
	}
	virtual void Fixed_Update ( ) { }
	virtual void UI_Update ( ) { }
	virtual void Delete ( ) { }
};

#endif