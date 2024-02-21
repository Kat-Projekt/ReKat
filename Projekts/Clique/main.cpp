#include "load.h"

int main ( int argc, char const *argv[] ) {
    ReKat::grapik::Start ( "Clique", 800, 600, false, false, true );
	ReKat::synth::Start ( );
	if ( Load () != 0 ) { return 1; }

	while ( ReKat::grapik::IsEnd ( ) ) {
		glClearColor(0.0, 0.0, 0.0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		Scene_Manager::Update();
		ReKat::grapik::Update();
    }

	ReKat::synth::End();
	ReKat::grapik::Terminate();
	return 0;
}
