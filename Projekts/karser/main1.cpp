/** This is the scene parser */
/** converts the .kat file in the scene tree */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Resource {
	std::string type;
	std::string name;
	std::vector < std::string > attributes; 
};

struct Objekt {
	std::string name;
	std::vector < std::string > attributes;
	std::vector < std::string > components;
};


int main ( int argc, char const *argv[] ) {
	if ( argc < 2 ) { std::cout << "can't parse an immaginary file\n"; return 1; }
	
	// open input file
	std::ifstream in ( argv[1], std::ios_base::in );

	// first clean empty lines and comments
	std::vector < std::string > lines;
	std::string line;
	while ( std::getline ( in, line ) ) {
		// std::cout << "line: " << line;
		std::string new_line = "";
		bool primo_carattere_commento = false;
		for (size_t i = 0; i < line.size ( ); i++) {
			if (line[i] == '/' ) { 
				if ( primo_carattere_commento ) { new_line.pop_back (); break; }
				primo_carattere_commento = true; 
			}
			else { primo_carattere_commento = false; }
			new_line += line[i];
		}
		if ( ! ( new_line == "" || new_line == "\n" || new_line.size() == 0 ) ) { lines.push_back ( new_line ); }
		
	}

	// for ( auto line : lines ) {	std::cout << "line: " << line << "_size" << line.size() <<'\n'; }

	std::vector < Resource > resources = { {"Texture", "pippo", {"\"Giacomo.png\""}} };
	std::vector < std::string > components;
	std::vector < Objekt > objekts = { {"kat", {"{0,0,0}"}, {"balles"}} };
	int fase = 0;

	for ( auto line : lines ) {
		if ( line == ".resources") { fase = 0; continue; }
		if ( line == ".components") { fase = 1; continue; }
		if ( line == ".objekts") { fase = 2; continue; }
		switch ( fase ) {
			case 0: // resources
				std::cout << "resources: " << line << '\n';
			break;
			case 1: { // components // eatch line is a file
				std::string clean_include = "";
				for ( size_t i = 0; i < line.size ( ); i++ ) {
					if ( ( line[i] == ' ' || line[i] == '\t' || line[i] == '\"' ) && clean_include == "" ) { continue; }
					if ( line[i] == '\"' && clean_include != "" ) { break; }
					clean_include += line[i];
				}
				components.push_back ( clean_include );				
				std::cout << "components: " << clean_include << '\n';
			} break;
			case 2: // objekts
				std::cout << "objekts: " << line << '\n';
			break;
		}
	}

	// create main and load files and then link them
	auto main = std::ofstream ( "main.cpp" );
	auto load = std::ofstream ( "load.hpp" );

	// include components
	for ( auto inc : components ) {
		load << "#include \"" << inc << "\"\n";
	}

	// adding head
	main << "#include \"load.h\"\n \nint main ( int argc, char const *argv [ ] ) {\n";
	load << "\nint load ( ) {\n\tint result = 0;\n";

	for ( auto res : resources ) {
		load << '\t' << "Manager::" << res.type << "_Load ( \"" << res.name << "\"";
		for ( size_t i = 0; i < res.attributes.size( ); i++ ) {
			load << ", " << res.attributes[i];
		}
		load << " );\n";
	}

	for ( auto obj : objekts ) {
		load << "\tauto " << obj.name << " = Manager::Objekt_Load ( " << obj.name;
		for ( auto att : obj.attributes ) { load << ", " << att; }
		load << " );\n";

		// configuring components
		for ( auto com : obj.components ) {
			load << '\t' << obj.name << ".Add_Component < " << com << " > ( );\n";
		}
	}

	// close
	main << "}\n\n";
	main.close ( );
	load << "}\n\n";
	load.close ( );

	in.close ( );
	return 0;
}
