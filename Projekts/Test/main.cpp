#include "load.h"

int main ( int argc, char const *argv[] ) {
    ReKat::grapik::Start ( "Test", 800, 600, false, false, true );
	ReKat::synth::Start ( );
	if ( Load () != 0 ) { return 1; }
	
	ReKat::synth::End();
	ReKat::grapik::Terminate();
	return 0;
}
