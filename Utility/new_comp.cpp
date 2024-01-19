#include <fstream>
#include <iostream>

int main ( int argc, char const *argv[] ) {
	std::string Cname = "";
	if ( argc == 2 ) { Cname = argv[1]; }
	if ( argc != 2 ) { std::cout << "Insert Componenent name: "; std::cin >> Cname; }
	std::string component = "#ifndef _" + Cname + "\n#define _" + Cname + "\n\
\n\
#include <Graphik/objekt.h>\n\
class " + Cname + " : public Behaviour {\n\
	void Start ( ) { }\n\
    void Update ( ) { }\n\
\n\
    void Collision ( T* obj ) { }\n\
    void CollisionTrigger ( T* obj ) { }\n\
\n\
	public:\n\
};\n\
\n\
#endif";

	std::ofstream out( Cname + ".h" );
	out << component;
	out.close();

	return 0;
}

/*
#ifndef _NAME
#define _NAME

#include <Graphik/manager.h>
class NAME : public Behaviour {
	void Start ( ) { }
	void Update ( ) { }

	void Collision ( Objekt* obj ) { }
	void CollisionTrigger ( Objekt* obj ) { }
};
#endif
*/