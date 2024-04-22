#include "load.h"

int main ( int argc, char const *argv[] ) {
	if ( Load () != 0 ) { return 1; }
	Manager::Start ( );
	while ( ReKat::grapik::IsEnd ( ) ) {
		Manager::Update ( );
		ReKat::grapik::Update ( );
    }
	Manager::Free_Audio ( );
	Manager::Free ( );
	ReKat::grapik::Terminate();
	return 0;
}
