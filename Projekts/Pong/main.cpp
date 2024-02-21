#include "load.h"

int main ( int argc, char const *argv[] ) {
    ReKat::grapik::Start ( "Pong", 800, 600, false, false, true );
	ReKat::synth::Start ( );
	if ( Load () != 0 ) { return 1; }

	std::cout << "-------------- Start Main Loop -----------------\n";
	while ( ReKat::grapik::IsEnd ( ) ) {
		glClearColor(0.0, 0.0, 0.0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		Scene_Manager::Update();
		ReKat::grapik::Update();
		std::cout << '\n';
	}
	std::cout << "\n-------------- Ended Main Loop -----------------\n";

	ReKat::synth::End();
	ReKat::grapik::Terminate();
	return 0;
}
