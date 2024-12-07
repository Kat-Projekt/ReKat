// this is the interpeter for the scenes
#include "parser.hpp"
#include "interpreter.hpp"

int main ( int argc, char const *argv[] ) {
    // get scene file path from args
    if ( argc < 2 ) { DEBUG ( 1, "can't parse an immaginary file\n" ); }

    // parse the file
	auto pj = parse ( argv[1] );

    // run the interpreter
    load ( pj );

    start ( );

    if ( pj.Interface.graphik == true ) {
        while ( ReKat::grapik::IsEnd( ) ) 
        { Update ( ); }
    } else {
        while ( true ) 
        { Update ( ); }
    }

    return 0;
}

