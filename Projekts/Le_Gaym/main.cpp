#include "Engine/load.h"

int main ( int argc, char const *argv[] ) {
    ReKat::grapik::Start ( "Kat Kave", 800, 600, false, false, true );
	ReKat::synth::Start ( );
	if ( Load () != 0 ) { return 1; }
	Manager::Start ( );
	while ( ReKat::grapik::IsEnd ( ) ) {
		Manager::Update ( );
		ReKat::grapik::Pool ( );
    }
	ReKat::synth::End();
	ReKat::grapik::Terminate();
	return 0;
}