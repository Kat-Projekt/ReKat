#include <fstream>
#include <iostream>

int main ( int argc, char const *argv[] ) {
	std::string Cname = "";
	if ( argc == 2 ) { Cname = argv[1]; }
	if ( argc != 2 ) { std::cout << "Insert Componenent name: "; std::cin >> Cname; }
	std::string component = "#ifndef _" + Cname + "\n#define _" + Cname + "\n\
\n\
#include <engine.hpp>\n\
using namespace ReKat::grapik::Input;\n\
\n\
class " + Cname + " : public Behaviour {\n\
public:\n\
	virtual void Start ( ) { }\n\
	virtual void Update ( ) { }\n\
	virtual void Fixed_Update ( ) { }\n\
	virtual void UI_Update ( ) { }\n\
\
	virtual void Collision ( T* _obj ) { _obj; }\n\
	virtual void Collision_Trigger ( T* _obj ) { _obj; }\n\
\
	virtual void Delete ( ) { }\n\
};\n\
\n\
#endif";

	std::ofstream out( Cname + ".h" );
	out << component;
	out.close();

	return 0;
}

/*
#ifndef AUDIO_LISTENER_H
#define AUDIO_LISTENER_H

#include <Graphik/objekt.h>
#include <Graphik/graphik.hpp>
using namespace ReKat::grapik::Input;

class Audio_Listener : public Behaviour {
public:
    virtual void Start ( ) { }
    virtual void Update ( ) { }
	virtual void Fixed_Update ( ) { }
	virtual void UI_Update ( ) { }

    virtual void Collision ( Objekt* _obj ) { _obj; }
    virtual void Collision_Trigger ( Objekt* _obj ) { _obj; }
};

#endif
*/