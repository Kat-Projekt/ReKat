#include <fstream>
#include <iostream>

int main ( int argc, char const *argv[] ) {
	std::string Cname = "";
	if ( argc == 2 ) { Cname = argv[1]; }
	if ( argc != 2 ) { std::cout << "Insert Componenent name: "; std::cin >> Cname; }
	std::string component = "#ifndef _" + Cname + "\n#define _" + Cname + "\n\
\n\
#include <Graphik/manager.h>\n\
class " + Cname + " : public Behaviour {\n\
	void Start ( ) { }\n\
	void Update ( ) { }\n\
\n\
	void Collision ( std::string obj ) { }\n\
	void CollisionTrigger ( std::string obj ) { }\n\
	void CollisionEnter ( std::string obj ) { }\n\
	void CollisionExit ( std::string obj ) { }\n\
	void CollisionTriggerEnter ( std::string obj ) { }\n\
	void CollisionTriggerExit ( std::string obj ) { }\n\
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

	void Collision ( std::string obj ) { }
	void CollisionTrigger ( std::string obj ) { }
	void CollisionEnter ( std::string obj ) { }
	void CollisionExit ( std::string obj ) { }
	void CollisionTriggerEnter ( std::string obj ) { }
	void CollisionTriggerExit ( std::string obj ) { }

	public:
	
};
#endif
*/