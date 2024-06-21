#include "load.h"
 
int main ( int argc, char const *argv [ ] ) {
	ReKat::grapik::Start ( "Clique", 600, 400, false, false, true );
	load ( );
	Manager::Start ( );
	while ( ReKat::grapik::IsEnd ( ) ) {
		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		Manager::Update ( );
		ReKat::grapik::Update ( );
	}
	Manager::Free ( );
	ReKat::grapik::Terminate ( );
}