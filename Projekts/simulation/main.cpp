#include "load1.h"

int main ( int argc, char const *argv[] ) {
    ReKat::grapik::Start ( "clique", 800, 600, false, false, true );
	ReKat::synth::Start ( );
	if ( Load () != 0 ) { return 1; }

	ReKat::synth::End();
	ReKat::grapik::Terminate();	
	std::cout << "-------------------- END -----------------------\n";
	return 0;
}
