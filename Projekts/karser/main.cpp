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

class Reader {
private:
	std::string caracters;
	size_t position_pointer = 0;
	size_t size;
public:
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

		std::string token = "";
		char current = reader.peek ( );
		bool Quotation_Mark_Apostrophe = false; // chech for " or ' 
		while ( !reader.Eof ( ) ) {
			// std::cout << "char: " << current << '\n';
			if ( ( current == '\"' || current == '\'' ) && Quotation_Mark_Apostrophe ) {
				token += current;
				current = reader.next ( );
				tokens.push_back ( token );
				token = "";
				Quotation_Mark_Apostrophe = false;
				continue; 
			}
			if ( current == '\"' || current == '\'' ) { Quotation_Mark_Apostrophe = true; }

			if ( ! Quotation_Mark_Apostrophe && ( current == ' ' || current == '\t' || current == '\n' ) ) {
				tokens.push_back ( token ); 
				token = ""; 
				current = reader.next ( );
				continue;
			}
			
			token += current;
			current = reader.next ( );
		}
		tokens.push_back ( token );

		// clear tokens
		std::vector < std::string > tokenns;
		std::string tokenn = "";
		for ( auto token : tokens ) {
			for ( auto c : token ) { 
				if ( !( c == ' ' || c == '\t' || c == '\n' ) ) {
					tokenn += c;
				}
			}
			if ( tokenn == "" ) { continue; }
			// check for : or ;
			// std::cout << "token: " << tokenn << " size: " << tokenn.size( ) << '\n';
			if ( tokenn == ":" || tokenn == ";" ) 
			{ tokenns.push_back ( tokenn ); tokenn = ""; continue; }
			if ( tokenn.back() == ':' || tokenn.back() == ';' || tokenn.back() == ',') {
				std::string p = "";
				p += tokenn.back( );
				tokenn.pop_back ( );
				tokenns.push_back ( tokenn );
				tokenns.push_back ( p );
				tokenn = "";
				continue;
			}

			tokenns.push_back ( tokenn );
			tokenn = "";
		}

		std::vector < std::string > tokennns;
		// meged tokens: .resource:texture -> .resource : texture
		for ( auto token : tokenns ) {
			if ( token == ";" || token == ":" || token == "," ) 
			{ tokennns.push_back ( token ); continue; }
			std::string temp_token = "";
			for ( auto c : token ) {
				if ( c == ';' || c == ':' || c == ',' ) { // split
					tokennns.push_back ( temp_token );
					std::string charc;
					charc += c;
					tokennns.push_back ( charc );
					temp_token = "";
					continue;
				}
				temp_token += c;
			}
			tokennns.push_back ( temp_token );
		}

		std::vector < std::string > toke4ns;
		for ( auto token : tokennns ) {
			if ( !( token == " " || token == "\t" || token == "\n" ) ) {
				toke4ns.push_back ( token );
			}
		}

		tokens = toke4ns;
		size = toke4ns.size ( );

		// std::cout << "tokens:\n"; for ( auto token : tokens )  { std::cout << token << ' '; } std::cout << tokens.size ( );
		
		std::cout << "Done  Lexer\n";
	}

	std::string peek ( int k = 0 ) 
	{ return tokens [ current_token + k ]; }

	std::string next ( ) {
		current_token ++;
		if ( current_token >= size ) { return 0; }
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

void Error ( std::string error ) {
	color ( error.c_str ( ), FOREGROUND_RED | FOREGROUND_INTENSITY );
	throw;
}

class Parser {
private:
	Projekt pj;
	Lexer lexer;
public:
	void Parse_Resources ( int inizio, int fine ) {
		std::cout << "parsing Resources\tstarting at:" << inizio << "\t ending at: " << fine <<'\n';
		// setup lexer
		lexer.set_pointer ( inizio );
		while ( lexer.get_pointer ( ) != fine )	{
			std::cout << "PR: " << lexer.next ( ) << '\n';
			/* code */
		}
	}

	void Parse_Objekts ( int inizio, int fine )  {
		std::cout << "parsing Objekts\tstarting at:" << inizio << "\t ending at: " << fine <<'\n';
		// setup lexer
		lexer.set_pointer ( inizio );
		while ( lexer.get_pointer ( ) != fine )	{
			std::cout << "PO: " << lexer.next ( ) << '\n';
			/* code */
		}
	}

	void Parse_Components ( int inizio, int fine ) {
		std::cout << "parsing Components\tstarting at:" << inizio << "\t ending at: " << fine <<'\n';
		// setup lexer
		lexer.set_pointer ( inizio );
		while ( lexer.get_pointer ( ) != fine )	{
			std::cout << "PC: " << lexer.next ( ) << '\n';
			/* code */
		}
	}
	
	Parser ( Lexer _lexer ) {
		lexer = _lexer;
		std::cout << "Start Parser\n\n";
		int temp = -1;

		// find interface data // mandatory
		// temp = lexer.find_token ( "projekt" );
		// if ( temp == -1 ) { Error ( "Projekt name not set\n\tAdd \"projekt = <name>\" in your file\n" );}

		std::string current_token = lexer.peek ( );

		while ( !lexer.Eof ( ) ) {

			if ( current_token == ".resources" ) {
				// check for begin
				if ( lexer.next ( ) != "{" ) { Error ( "unexpected token after .resources" ); }
				int open_brakets = 1;
				int begining_of_section = lexer.get_pointer ( );
				

				// find end -> count "{" and return when equals to "}"
				while ( !lexer.Eof ( ) ) {
					current_token = lexer.next ();
					if ( current_token == "{" ) { open_brakets ++; }
					if ( current_token == "}" ) { open_brakets --; }
					if ( open_brakets == 0 ) { break; }
				}

				if ( open_brakets == 0 ) { Parse_Resources ( begining_of_section, lexer.get_pointer ( ) -1 ); }
				else { Error ( "missing closing token" ); }
				current_token = lexer.next ( );
			}
			if ( current_token == ".components" ) {// check for begin
				if ( lexer.next ( ) != "{" ) { Error ( "unexpected token after .components" ); }
				int open_brakets = 1;
				int begining_of_section = lexer.get_pointer ( );
				

				// find end -> count "{" and return when equals to "}"
				while ( !lexer.Eof ( ) ) {
					current_token = lexer.next ();
					if ( current_token == "{" ) { open_brakets ++; }
					if ( current_token == "}" ) { open_brakets --; }
					if ( open_brakets == 0 ) { break; }
				}

				if ( open_brakets == 0 ) { Parse_Components ( begining_of_section, lexer.get_pointer ( ) -1 ); }
				else { Error ( "missing closing token" ); }
				current_token = lexer.next ( );
			}
			if ( current_token == ".objekts"    ) {
				if ( lexer.next ( ) != "{" ) { Error ( "unexpected token after .objekts" ); }
				int open_brakets = 1;
				int begining_of_section = lexer.get_pointer ( );
				

				// find end -> count "{" and return when equals to "}"
				while ( !lexer.Eof ( ) ) {
					current_token = lexer.next ();
					if ( current_token == "{" ) { open_brakets ++; }
					if ( current_token == "}" ) { open_brakets --; }
					if ( open_brakets == 0 ) { break; }
				}

				if ( open_brakets == 0 ) { Parse_Objekts ( begining_of_section, lexer.get_pointer ( ) -1 ); }
				else { Error ( "missing closing token" ); }
				current_token = lexer.next ( );
			}
			
			std::cout << current_token << ' ';

			current_token = lexer.next ();
		}
		

		std::cout << "\n\nDone  Parser\n";
	}
};

class Compiler {
public:
	Compiler ( Parser p ) {
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
