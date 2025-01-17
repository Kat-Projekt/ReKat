#ifndef PARSER
#define PARSER

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#define DIAGNOSTIC
#include <debugger.hpp>

/* SINTAX
-----------

Every command must be divided by one separator
form the list { ';', '{', '[', '=' }

comments are begined with // and are ignored
"""
// this is a comment
"""
	--------------
	-- VARABLES --
	--------------

variables types there are 5 supported types:
1. 'strings' delimentated by '"'
2. 'integers' just the number
3. 'floats' / doubles represented using a .
4. 'booleans' just true or false
5. 'vectors' rappresented by { <value>, <value>, ... <value> } the <value>s have to have the varable type

Examples
"""
"this is a string"
1234 // this is an integer aka. int
1234.5678 // 1234 is the integer part and 5678 is the fractionary part
true // this is a boolean
false // this two
{1,3,5,6} // this is a vector with 4 integers
{123,"hello",12.56} // is not a valid vector
"""

from now on i will refer to varibles as <type> and vectors as <vector<type>> like cpp templates
whe you find that simbol substitute it with a varible of that type

	-------------
	-- HEADERS --
	-------------

each header shoud be unique and will throw an error if duplicated

there are 6 heades: 
1. 'projekt' is used to set the projekt name 
	'''projekt = <string>'''
2. 'graphik' is used to set up the graphik renderer
	'''graphik = [ <int> ]'''
3. 'phisiks'
4. 'synth'
5. 'katwork'
6. 'entry_point'

	----------------
	-- PARAGRAPHS --
	----------------



*/

class Reader {
private:
	std::string caracters;
	size_t position_pointer = 0;
	size_t size;
public:
	Reader ( std::string file ) {
        DEBUG ( 4, "Reading file" );
		std::ifstream in ( file, std::ios_base::in );
		// do sintax check

		std::string line;
		while ( std::getline ( in, line ) ) {
			std::string new_line = "";
			bool primo_carattere_commento = false;
			for (size_t i = 0; i < line.size ( ); i++) {
	            // check for comments
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

		size = caracters.size ( );
        DEBUG ( 4, "Done Reading" );
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
        DEBUG ( 4, "Starting Lexer" );
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

		DEBUG ( 4, "Done  Lexer\n" );
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
	// Abstract tree
	struct node {
		std::string token;
		//-1: "no indentation"
		// 0: '{'
		// 1: '['
		// 2: '='
		int child_type = -1; 
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
        DEBUG ( 4, "Starting Parser" );
		lexer = _lexer;
		int temp = -1;

		std::string current_token = lexer.peek ( );
		
		while ( !lexer.Eof ( ) ) 
		{ root.push_back ( Find_node ( ) ); }
		
        DEBUG ( 4, "Done Parser" );
	}
};

class Projekt {
	public:

	struct _Graphik_Interface {
		bool active = false;
		unsigned int width = 0;
		unsigned int heigth = 0;
		bool resizable = false;
		bool fullscreen = false;
		bool transaprent = false;
	};

	struct _Phisiks_Interface {
		bool active = false;
		unsigned int frames = 60;
	};
	
	struct _Interface {
		_Graphik_Interface graphik;
		_Phisiks_Interface phisiks;
		bool synth = false;
		bool katwork = false;
	};

	struct _Argument {
		std::string value;
	};

	friend std::ostream& operator << ( std::ostream& os, const _Argument a ) {
		os << a.value;
		return os;
	}

	struct _Resource {
		std::string type;
		std::string name;
		std::vector < _Argument > Arguments;
	};

	struct _Component {
		std::string name;
		std::vector < _Argument > Arguments;
	};

	friend std::ostream& operator << ( std::ostream& os, const _Component c ) {
		os << c.name;
		return os;
	}

	struct _Objekt {
		std::string name;
		std::vector < _Argument > Attributes;
		std::vector < _Component > Components;
		std::vector < _Objekt > Childs;
	};

	friend std::ostream& operator << ( std::ostream& os, const _Objekt o ) {
		os << o.name;
		return os;
	}

	std::string name = "";
	_Interface Interface;
	std::vector < _Objekt > Objekts;
	std::vector < _Resource > Resources;
	
	std::string Projekt_Entry_Point;

    Projekt ( ) { }

    Projekt ( Parser p ) {
        DEBUG ( 3, "CREATING PROJEKT" );
		// find root node projekt ( break point )
		for ( auto n : p.root ) {
			if ( n.token == "projekt" ) {
				name = n.childrens[0].token;
				DEBUG ( 5, "projekt name: ", name );
			}
			if ( n.token == "entry_point" ) {
				Projekt_Entry_Point = n.childrens[0].token;
				DEBUG ( 5, n.token, " set: ", Projekt_Entry_Point );
			}
		}
		if ( name == "" ) { DEBUG ( 1, "no name set" ); }
		if ( Projekt_Entry_Point == "" ) { DEBUG ( 1, "no entry point?" ); }

		// find modules node: graphik, synth, katwork,
		for ( auto n : p.root ) {
            if ( n.token == "graphik" || n.token == "synth" || n.token == "katwork" ) {
				if ( n.childrens.size ( ) == 0 ) { DEBUG ( 1, "Interface sub value not defined" ); }

                if ( n.token == "graphik" ) { Interface.graphik.active = true; continue; }
                if ( n.token == "phisiks" ) { Interface.phisiks.active = true; 
											  Interface.phisiks.frames = std::stoi ( n.childrens [0].token ); continue; }

                bool valore = false;
                if ( n.childrens[0].token == "true" ) { valore = true; }
                else if ( n.childrens[0].token == "false" ) { valore = false; }
                else { DEBUG ( 1, "erroneus boolean type after token: ", n.token ); }

                if ( n.token == "synth"   ) { Interface.synth = valore;   }
                if ( n.token == "katwork" ) { Interface.katwork = valore; }
                DEBUG ( 5, n.token, " engine activated" );
            }
		}

		// configure resources
		DEBUG ( 4, "Resources" );
		for ( auto n : p.root ) {
			if ( n.token == ".resources" ) {
				for ( auto risorce_typ : n.childrens ) {
					std::string resource_type = risorce_typ.token;
					for ( auto ris : risorce_typ.childrens ) {
						Projekt::_Resource resource;
						resource.type = resource_type;
						resource.name = ris.token;
						for ( auto arg : ris.childrens ) { resource.Arguments.push_back ( { arg.token } ); }
						Resources.push_back ( resource );
						DEBUG ( 5, resource.type, " : ", resource.name, " { ", resource.Arguments , " }" );
					}
				}
			}
		}

		// configure objekts
		DEBUG ( 4, "Objkets" );
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
					Objekts.push_back (obj);
					DEBUG ( 5, obj.name, " ( ", obj.Attributes, " ) : ", obj.Components , " : { ", obj.Childs, " }" );
				}
			}
		}
    }
};

inline Projekt parse ( std::string file ) {
    return Projekt ( Parser ( Lexer ( Reader ( file ) ) ) );
}

#endif