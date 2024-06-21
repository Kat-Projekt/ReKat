/** This is the scene parser */
/** converts the .kat file in the scene tree */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

template < typename T, typename T1 >
bool in_map ( std::unordered_map < T, T1 > map, T element ) {
	for ( auto pair : map ) 
	{ if ( pair.first == element ) { return true; } }
	return false;
}

#ifndef COLOR
#define COLOR
#if (defined (LINUX) || defined (__linux__) || defined (__APPLE__)) // linux implementation
	#include <stdio.h>
	#include <string>
	void color ( std::string string, int color ) {
		printf( ( std::string ( "\033[" ) + std::to_string ( color ) + std::string ( "m" ) + string + std::string ( "\033[0m" ) ).c_str() );
	}
#elif (defined (_WIN32) || defined (_WIN64)) // windows implementaion
	#include <stdio.h>
	#include <windows.h>
	void color ( const char* string, WORD color ) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, color);
		printf( string );
		SetConsoleTextAttribute( hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY );
	}
#endif
#endif

void Error ( std::string error ) { color ( error.c_str ( ), FOREGROUND_RED | FOREGROUND_INTENSITY ); throw; }

class Reader {
private:
	std::string caracters;
	size_t position_pointer = 0;
	size_t size;
public:
	// removes comments
	Reader ( std::string file ) {
		// std::cout << "Start Reader\n";
		// open files
		std::ifstream in ( file, std::ios_base::in );

		std::string line;
		while ( std::getline ( in, line ) ) {
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
			if ( ! ( new_line == "" || new_line == "\n" || new_line.size() == 0 ) ) {
				caracters += new_line + "\n"; 
			}
		}

		// std::cout << "Readed file:\n" << caracters << '\n';
		size = caracters.size ( );
		// std::cout << size;
		// std::cout << "Done  Reader\n";
	}

	char peek ( int k = 0 ) 
	{ return caracters [ position_pointer + k ]; }

	char next ( ) {
		position_pointer ++;
		return caracters [ position_pointer ];
	}

	bool Eof ( ) 
	{ return ( position_pointer + 1 >= size ); }
};

class Lexer {
private:
	std::vector < std::string > tokens;
	size_t current_token = 0;
	size_t size;
public:
	Lexer ( ) { }
	Lexer ( Reader reader ) {
		// std::cout << "Start Lexer\n";
		// remove every non in string space or new line
		std::string reformatted_file = "";
		bool Quotation_Mark = false; // chech for " or '
		bool Apostrophe = false;
		char current = reader.peek ( );
		while ( !reader.Eof ( ) ) {
			if ( current == '\"' && !Apostrophe ) // activate if not in an aphostrope string
			{ Quotation_Mark = !Quotation_Mark; }
			if ( current == '\'' && !Quotation_Mark ) // activate if not in an qutoation string
			{ Apostrophe = !Apostrophe; }

			if ( ( current == ' ' || current == '\t' || current == '\n' ) && ! ( Apostrophe || Quotation_Mark ) ) 
			{ current = reader.next ( ); continue; }
		
			reformatted_file += current;
			current = reader.next ( );
		}

		// std::cout << reformatted_file;

		std::string token = "";

		bool skip = false;
		bool graph_into_square = false; // when a {} is icnaposusated in [] used for postioning attributes [ {0,0,0}, "thing" ]
		bool into_square = false;
		for ( size_t i = 0; i < reformatted_file.size(); i++ ) {
			if ( skip ) {
				tokens.push_back ( token );
				token = "";
				skip = false;
			}
			
			char c = reformatted_file[i];
			if ( c == '[' ) { into_square = true; }
			if ( into_square && c == '{' ) { graph_into_square = true; }

			// if c is a divider
			if ( !graph_into_square && ( c == '[' || c == '{' || c == '}' || c == ']' || c == ';' || c == ',' || c == '=' ) ) {
				tokens.push_back ( token );
				token = "";
				skip = true;
			}

			if ( into_square && c == '}' ) { graph_into_square = false; }
			if ( c == ']' ) { into_square = false; }


			token += reformatted_file[i];
		}
		tokens.push_back ( token );

		std::vector < std::string > tokens_whiout_blaks;
		for ( auto token : tokens ) { 
			if ( token == "" ) { continue; } 
			tokens_whiout_blaks.push_back ( token );
		}

		tokens = tokens_whiout_blaks;
		size = tokens.size ( );

		// std::cout << "tokens:\n"; for ( auto token : tokens )  { std::cout << token << " ' "; } std::cout << tokens.size ( );

		// std::cout << "Done  Lexer\n";
	}

	std::string peek ( int k = 0 ) 
	{ return tokens [ current_token + k ]; }

	std::string next ( ) {
		current_token ++;
		if ( current_token >= size ) { return ""; }
		return tokens [ current_token ];
	}

	int find_token ( std::string token ) {
		for ( size_t i = 0; i < tokens.size ( ); i++ )
		{ if ( token == tokens[i] ) { return i; } }
		return -1;
	}

	void set_pointer ( int p ) { p < size ? current_token = p : 0 ; }
	int get_pointer ( ) { return current_token; }

	bool Eof ( ) 
	{ return ( current_token + 1 > size ); }
};

class Parser {
private:
	struct node {
		std::string token;
		int child_type = -1; // -1 = "no indentation" ; 0 = '{' ; 1 = '[' ; 2 = '='
		std::vector < node > childrens;
	};

	Lexer lexer;
public:
	std::vector < node > root;

	void print_node ( node n, std::string indentation = "" ) {
		std::cout << indentation << n.token;
		if ( n.childrens.size ( ) == 0 ) { return; }
		switch ( n.child_type ) {
		case 0: std::cout << " : { n: " << n.childrens.size ( ); break;
		case 1: std::cout << " : [ n: " << n.childrens.size ( ); break;
		case 2: std::cout << " = "; print_node ( n.childrens[0]); std::cout << '\n'; return;
		}

		for ( auto nn : n.childrens ) {
			std::cout << "\n";
			print_node ( nn, indentation + "\t" ) ;
		}

		switch ( n.child_type ) {
		case 0: std::cout << "\n" << indentation << "}\n"; break;
		case 1: std::cout << "\n" << indentation << "]\n"; break;
		}
	}
	// the token of a node is every thing it contains and it is broken when ',' ';' '}' is found
	node Find_node ( ) {
		node new_node;
		std::string token = lexer.peek ( );
		// std::cout << "\nfinding pointer staring with: " << token;
		std::string next_token = lexer.next ( );
		// std::cout << "\n\tnext: " << next_token << " ;";

		if ( next_token == "=" ) {
			// std::cout << "\n finding = token \n";
			new_node.child_type = 2;
			new_node.token = token;
			lexer.next ( );
			new_node.childrens.push_back ( Find_node ( ) );
			// std::cout << "\nadded token\n";
			return new_node;
		}

		bool is_aglomerate = false;
		while ( ( next_token == "{" || next_token == "[" ) || ( is_aglomerate && next_token == ",") ) {
			if ( !is_aglomerate ) {
				if ( next_token == "[" ) { new_node.child_type = 1; }
				else { new_node.child_type = 0; }
			}
			
			// std::cout << "\nadding child to " << token;
			lexer.next ( );
			new_node.childrens.push_back ( Find_node ( ) );

			// std::cout << "\nfind child of " << token;
			next_token = lexer.peek ( );
			// std::cout << "\n out token " << next_token ;
			is_aglomerate = true;
		}
		
		new_node.token = token;
		if ( is_aglomerate ) { lexer.next (); return new_node; }

		if ( next_token == "}" || next_token == "]" ) { return new_node; }

		
		// std::cout <<  "\n\ttoken " << token;
		// std::cout <<  "\n\tn_token " << next_token;
		while ( !( next_token == "," || next_token == ";" || next_token == "" || next_token == "}" || next_token == "]"  ) ) {
			// std::cout << "\n\t adding token " << next_token;
			token += " " + next_token;
			next_token = lexer.next ( );
		}
		
		new_node.token = token;

		if ( next_token == ";" || next_token == "" ) { lexer.next ( ); }

		return new_node;
	}
	
	Parser ( Lexer _lexer ) {
		lexer = _lexer;
		// std::cout << "Start Parser\n\n";
		int temp = -1;

		// find interface data // mandatory
		// temp = lexer.find_token ( "projekt" );
		// if ( temp == -1 ) { Error ( "Projekt name not set\n\tAdd \"projekt = <name>\" in your file\n" );}

		std::string current_token = lexer.peek ( );
		
		while ( !lexer.Eof ( ) ) 
		{ root.push_back ( Find_node ( ) ); }
		
		// for ( auto n : root ) { std::cout << '\n'; print_node ( n ); }

		// std::cout << "\n\nDone  Parser\n";
	}
};

class Projekt {
	public:
	
	struct _Interface {
		bool graphik = false;
		bool synth = false;
		bool katwork = false;
	};

	struct _Argument {
		std::string value;
	};

	struct _Resource {
		std::string type;
		std::string name;
		std::vector < _Argument > Arguments;
	};

	struct _Component {
		std::string name;
		std::vector < _Argument > Arguments;
	};

	struct _Additional_scripts {
		std::string path;
	};

	struct _Objekt {
		std::string name;
		std::vector < _Argument > Attributes;
		std::vector < _Component > Components;
		std::vector < _Objekt > Childs;
	};

	std::string name = "";
	_Interface Interface;
	std::vector < _Objekt > Objekts;
	std::vector < _Resource > Resources;
	std::vector < _Additional_scripts > Additional_scripts;
	
	std::string Projekt_Entry_Point;
};

class Compiler {
	Projekt pj;
public:
	/// @brief this creates the main.cpp and load.h files that nead to be compiled
	/// @brief can also pack everything neaded to compile in a single folder
	/// @brief first asambles the Projekt class than compiles it in the files
	/// @param p 
	Compiler ( Parser p, bool pack = false ) {
		std::cout << "Start Compiler\n";
		// find root node projekt ( break point )
		for ( auto n : p.root ) {
			if ( n.token == "projekt" ) {
				pj.name = n.childrens[0].token;
				std::cout << "projekt name: " << pj.name << '\n';
			}
			if ( n.token == "entry_point" ) {
				pj.Projekt_Entry_Point = n.childrens[0].token;
				std::cout << n.token << " set: " << pj.Projekt_Entry_Point;
			}
		}
		if ( pj.name == "" ) { Error  ( "no name?"); }
		if ( pj.Projekt_Entry_Point == "" ) { Error  ( "no entry point?"); }

		// find modules node: graphik, synth, katwork,
		for ( auto n : p.root ) {
			if ( n.token == "graphik" ) {
				pj.Interface.graphik = ( n.childrens[0].token == "true" ? true : 
										 n.childrens[0].token == "false" ? false : 
										 ( Error ( "erroneus boolean type after token: " + n.token ), false ) );
				std::cout << n.token << " engine activated\n";
			}
			if ( n.token == "synth" ) {
				pj.Interface.synth = ( n.childrens[0].token == "true" ? true : 
										 n.childrens[0].token == "false" ? false : 
										 ( Error ( "erroneus boolean type after token: " + n.token ), false ) );
				std::cout << n.token << " engine activated\n";
			}
			if ( n.token == "katwork" ) {
				pj.Interface.katwork = ( n.childrens[0].token == "true" ? true : 
										 n.childrens[0].token == "false" ? false : 
										 ( Error ( "erroneus boolean type after token: " + n.token ), false ) );
				std::cout << n.token << " engine activated\n";
			}
		}

		// configure resources
		for ( auto n : p.root ) {
			if ( n.token == ".resources" ) {
				for ( auto risorce_typ : n.childrens ) {
					std::string resource_type = risorce_typ.token;
					for ( auto ris : risorce_typ.childrens ) {
						Projekt::_Resource resource;
						resource.type = resource_type;
						resource.name = ris.token;
						for ( auto arg : ris.childrens ) { resource.Arguments.push_back ( { arg.token } ); }
						pj.Resources.push_back ( resource );
					}
				}
			}
		}
		std::cout << "resources to load:\n";
		for ( auto ris : pj.Resources ) {
			std::cout << " type: " << ris.type << "\n\tname: " << ris.name << "\n\t\targs: ";
			for ( auto arg : ris.Arguments ) {
				std::cout << arg.value << " ";
			}
			std::cout << '\n';			
		}
		

		// configure additional include files
		for ( auto n : p.root ) {
			if ( n.token == ".components" ) {
				for ( auto additional_include : n.childrens ) {
					std::string include = additional_include.token;
					pj.Additional_scripts.push_back ( { include } );
				}
			}
		}
		std::cout << "additionals to include:\n";
		for ( auto add_scr : pj.Additional_scripts ) {
			std::cout << " " << add_scr.path;
			std::cout << '\n';			
		}

		// configure objekts
		for ( auto n : p.root ) {
			if ( n.token == ".objekts" ) {
				for ( auto obj_token : n.childrens ) {
					Projekt::_Objekt obj;
					obj.name = obj_token.token;
					for ( auto ris : obj_token.childrens ) {
						if ( ris.token == "attributes" ) {
							for ( auto att : ris.childrens ) {
								obj.Attributes.push_back ( { att.token } );
							}
						}

						if ( ris.token == "components" ) {
							for ( auto comp : ris.childrens ) {
								Projekt::_Component compo;
								compo.name = comp.token;
								for ( auto att : comp.childrens ) {
									compo.Arguments.push_back ( { att.token } );
								}
								obj.Components.push_back ( compo );
							}
						}

						if ( ris.token == "childs" ) {
							for ( auto childs : ris.childrens ) {
								obj.Childs.push_back ( { childs.token } );
							}
						}
					}
					pj.Objekts.push_back (obj);
				}
			}
		}
		std::cout << "objekts:\n";
		for ( auto obj : pj.Objekts ) {
			std::cout << " " << obj.name;
			if ( obj.Attributes.size ( ) != 0 ) {
				std::cout << "\n\tattributes: ";
				for ( auto att : obj.Attributes ) {
					std::cout << "\n\t\t" << att.value;
				}
			}
			if ( obj.Components.size ( ) != 0 ) {
				std::cout << "\n\tcomponents: ";
				for ( auto com : obj.Components ) {
					std::cout << "\n\t\t" << com.name;
					for ( auto att : com.Arguments ) {
						std::cout << " " << att.value;
					}
				}
			}
			if ( obj.Childs.size ( ) != 0 ) {
				std::cout << "\n\tchilds: ";
				for ( auto child : obj.Childs ) {
					std::cout << "\n\t\t" << child.name;
				}
			}
			std::cout << '\n';			
		}

		// creating main file
		auto main = std::ofstream ( "main.cpp" );
		auto load = std::ofstream ( "load.h" );

		// include components
		load << "#include <engine.hpp>\n";
		for ( auto inc : pj.Additional_scripts ) {
			load << "#include " << inc.path << "\n";
		}

		// adding head
		main << "#include \"load.h\"\n \nint main ( int argc, char const *argv [ ] ) {\n";
		load << "\nint load ( ) {\n\tint result = 0;\n";

		// start engines
		if ( pj.Interface.graphik ) {
			main << "\tReKat::grapik::Start ( \"" + pj.name + "\", 600, 400 );\n";
		}
		if ( pj.Interface.graphik ) {
			// main << "\tReKat::synth::Start ( );\n";
		}
		if ( pj.Interface.graphik ) {
			// main << "\tReKat::online::Start ( " + pj.name + " );\n";
		}

		// creating main loop
		main << "\tload ( );\n";
		main << "\tManager::Start ( );\n";

		if ( pj.Interface.graphik ) {
			main << "\twhile ( ReKat::grapik::IsEnd ( ) ) {\n";
		} else {
			main << "\tbool main_shutdown = false;\n";
			main << "\twhile ( ! main_shutdown ) {\n";
		}

		main << "\t\tManager::Update ( );\n";

		if ( pj.Interface.graphik ) 
		{ main << "\t\tReKat::grapik::Update ( );\n"; }

		main << "\t}\n";

		// engine ending code
		if ( pj.Interface.graphik ) {
			main << "\tManager::Free ( );\n\tReKat::grapik::Terminate ( );\n";
		}
		if ( pj.Interface.graphik ) {
			// main << "\tReKat::synth::Terminate ( );\n";
		}
		if ( pj.Interface.graphik ) {
			// main << "\tReKat::online::Terminate;\n";
		}

		// load resources
		for ( auto ris : pj.Resources ) {
			load << "\tresult += Manager::" << ris.type << "_Load ( " << ris.name;
			for ( auto arg : ris.Arguments ) {
				load << ", " << arg.value;
			}
			load << " );\n";
		}
		load << "\tif ( result != 0 ) { throw; }\n";

		// create objects
		load << "\t// creating objekts \n";
		int obj_count = 0;
		std::unordered_map < std::string, std::string > objekts_list;

		for ( auto obj : pj.Objekts ) {
			if ( in_map ( objekts_list, obj.name ) ) { Error ( "already defined objekt\n"); }
			load << "\tauto obj_" << obj_count << " = Manager::Objekt_Load ( " << obj.name;
			for ( size_t i = 0; i < obj.Attributes.size( ) ; i++ ) {
				if ( i != 0 ) { load << ", "; } else { load << ' ';}
				load << obj.Attributes[i].value;
			}
			load << " );\n";
			objekts_list.insert ( { obj.name, "obj_" + std::to_string(obj_count) } );
			obj_count++;
		}

		// link objekts
		load << "\t// linking objekts \n";
		// creating hieratchy
		for ( auto obj : pj.Objekts ) {
			for ( auto child : obj.Childs ) {
				if ( ! in_map ( objekts_list, child.name ) ) 
				{ Error ( "no such named child: " + child.name );}
				load << '\t' << objekts_list[obj.name] << "->Add_Child ( " << objekts_list[child.name] << " );\n";
			}
		}

		// add components to objekts 
		// // first add
		// // second set
		load << "\t// adding components to objekts \n";
		for ( auto obj : pj.Objekts ) {
			for ( auto comp : obj.Components ) 
			{ load << '\t' << objekts_list[obj.name] << "->Add_Component < " << comp.name << " > ( );\n"; }
		}

		int additional_components = 0;
		for ( auto obj : pj.Objekts ) {
			for ( auto comp : obj.Components ) {
				if ( comp.Arguments.size ( ) != 0 ) { // there are arguments
					load << "\tauto com_" << additional_components << " = " << obj.name << " -> Get_Component < " << comp.name << " > ( );\n";

					load << "\tcom_" << additional_components << " -> Set (";
					for ( size_t i = 0; i < comp.Arguments.size( ) ; i++ ) {
						if ( i != 0 ) { load << ", "; } else { load << ' ';}
						load << comp.Arguments[i].value;
					}
					load << " );\n";
					additional_components ++;
					continue;
				}
			}
		}

		// add code entry point
		load << "\t// adding entry point\n";
		load << "\tManager::Set_Active_Scene ( " + pj.Projekt_Entry_Point + " );\n";

		main << "}";
		main.close ( );
		load << "\treturn 0;\n}";
		load.close ( );

		std::cout << "Done Compiler\n";
	}
};

int main ( int argc, char const *argv [ ] ) {
	if ( argc < 2 ) { std::cout << "can't parse an immaginary file\n"; return 1; }

	Reader r ( argv[1] );
	Lexer  l ( r );
	Parser p ( l );
	Compiler c ( p );
	return 0;
}
