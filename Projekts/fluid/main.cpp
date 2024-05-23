#include "load.h"
 
int main ( int argc, char const *argv [ ] ) {
	ReKat::grapik::Start ( "fluid", 500, 500, false, false, true );

	load ( );
	Manager::Start ( );
	while ( ReKat::grapik::IsEnd() ) {
		Manager::Update ( );
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        ReKat::grapik::Pool ();
	}
	Manager::Free ( );
	ReKat::grapik::Terminate ( );
}