/** This is the scene parser */
/** converts the .kat file in the scene tree */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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
		std::cout << "Start Reader\n";
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
		std::cout << "Done  Reader\n";
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
		std::cout << "Start Lexer\n";
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

		std::cout << reformatted_file;

		std::string token = "";

		bool skip = false;
		for ( size_t i = 0; i < reformatted_file.size(); i++ ) {
			if ( skip ) {
				tokens.push_back ( token );
				token = "";
				skip = false;
			}
			char c = reformatted_file[i];
			// if c is a divider
			if ( c == '[' || c == '{' || c == '}' || c == ']' || c == ';' || c == ':' || c == ',' || c == '=' ) {
				tokens.push_back ( token );
				token = "";
				skip = true;
			}
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

		std::cout << "tokens:\n"; for ( auto token : tokens )  { std::cout << token << " ' "; } std::cout << tokens.size ( );

		std::cout << "Done  Lexer\n";
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
	Projekt pj;
	Lexer lexer;
public:
	struct node {
		std::string token;
		int child_type = 0; // 0 = '{' ; 1 = '['
		std::vector < node > childrens;
	};

	void print_node ( node n, std::string indentation = "" ) {
		std::cout << indentation << n.token;
		if ( n.childrens.size ( ) == 0 ) { return; }
		switch ( n.child_type ) {
		case 0: std::cout << " : { n: " << n.childrens.size ( ); break;
		case 1: std::cout << " : [ n: " << n.childrens.size ( ); break;
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
		std::cout << "\nfinding pointer staring with: " << token;
		std::string next_token = lexer.next ( );
		std::cout << "\n\tnext: " << next_token << " ;";

		bool is_aglomerate = false;
		while ( ( next_token == "{" || next_token == "[" ) || ( is_aglomerate && next_token == ",") ) {
			if ( !is_aglomerate ) {
				if ( next_token == "[" ) { new_node.child_type = 1; }
				else { new_node.child_type = 0; }
			}
			
			std::cout << "\nadding child to " << token;
			lexer.next ( );
			new_node.childrens.push_back ( Find_node ( ) );

			std::cout << "\nfind child of " << token;
			next_token = lexer.peek ( );
			std::cout << "\n out token " << next_token ;
			is_aglomerate = true;
		}
		
		new_node.token = token;
		if ( is_aglomerate ) { lexer.next (); return new_node; }

		if ( next_token == "}" || next_token == "]" ) { return new_node; }

		
		std::cout <<  "\n\ttoken " << token;
		std::cout <<  "\n\tn_token " << next_token;
		while ( !( next_token == "," || next_token == ";" || next_token == "" || next_token == "}" || next_token == "]"  ) ) {
			std::cout << "\n\t adding token " << next_token;
			token += " " + next_token;
			next_token = lexer.next ( );
		}
		
		new_node.token = token;

		if ( next_token == ";" || next_token == "" ) { lexer.next ( ); }

		return new_node;
	}
	
	Parser ( Lexer _lexer ) {
		lexer = _lexer;
		std::cout << "Start Parser\n\n";
		int temp = -1;

		// find interface data // mandatory
		// temp = lexer.find_token ( "projekt" );
		// if ( temp == -1 ) { Error ( "Projekt name not set\n\tAdd \"projekt = <name>\" in your file\n" );}

		std::string current_token = lexer.peek ( );
		std::vector < node > root;

		while ( !lexer.Eof ( ) ) 
		{ root.push_back ( Find_node ( ) ); }
		
		for ( auto n : root ) {
			std::cout << '\n';
			print_node ( n );
		}

		std::cout << "\n\nDone  Parser\n";
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
		std::vector < _Argument > Arguments;
		std::vector < _Component > Components;
		std::vector < _Objekt > childs;
	};

	std::string name = "";
	_Interface Interface;
	std::vector < _Objekt > Objekts;
	std::vector < _Resource > Resources;
	std::vector < _Additional_scripts > Additional_scripts;
	
	_Objekt Projekt_Entry_Point;
};

class Compiler {
public:
	/// @brief this creates the main.cpp and load.h files that nead to be compiled
	/// @brief can also pack everything neaded to compile in a single folder 
	/// @param p 
	Compiler ( Parser p, bool pack = false ) {
		std::cout << "Start Compiler\n";
		

		std::cout << "Done  Compiler\n";
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
