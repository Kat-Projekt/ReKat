#include <fstream>
#include <iostream>
#include <filesystem>

int main ( int argc, char const *argv[] ) {
		std::string Pname = "";
	if ( argc == 2 ) { Pname = argv[1]; }
	if ( argc != 2 ) { std::cout << "Insert Projekt name: "; std::cin >> Pname; }

	// projekts needs:
	// Projekt folder
	// ├ main.cpp
	// ├ load.h
	// └ Resources
	std::filesystem::create_directory ( Pname );
	std::filesystem::create_directory ( Pname + "/Resources" );
	
	std::string loadn = "\
#define OPENGL\n\
#define OPENAL\n\
#define ONLINE_PEER\n\
#define ALL_MODULES\n\
#include <ReKat.hpp>\n\
\n\
int Load ( ) {\n\
	int result = 0;\n\
	Objekt * scene = new Objekt ( \"Scene\" );\n\
	scene->Add_Component < Phisiks > ( );\n\
	scene->Add_Component < Fps > ( );\n\
\n\
	Scene_Manager::Set_Active_Scene ( &scene );\n\
	return result;\n\
}";

	std::string mainn = "\
#include \"load.h\"\n\
\n\
int main ( int argc, char const *argv[] ) {\n\
    ReKat::grapik::Start ( \"" + Pname + "\", 800, 600, false, false, true );\n\
	ReKat::synth::Start ( );\n\
	if ( Load () != 0 ) { return 1; }\n\
	Manager::Start ( );\n\
	while ( ReKat::grapik::IsEnd ( ) ) {\n\
		glClearColor(0.0, 0.0, 0.0, 1.0f);\n\
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);\n\
		glEnable(GL_DEPTH_TEST);\n\
		Scene_Manager::Update()\n\
		ReKat::grapik::Pool ( );\n\
    }\n\
	ReKat::synth::End();\n\
	ReKat::grapik::Terminate();\n\
	return 0;\n\
}\n\
";

	std::ofstream out ( Pname + "/load.h" );
	out << loadn;
	out.close ();
	out.open ( Pname + "/main.cpp" );
	out << mainn;
	out.close ( );
	return 0;
}