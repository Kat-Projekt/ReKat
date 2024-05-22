#define DIAGNOSTIC
#include "load.h"
 
int main ( int argc, char const *argv [ ] ) {
	ReKat::grapik::Start ( "Clique", 600, 400 );
	load ( );
	Manager::Start ( );
	while ( ReKat::grapik::IsEnd ( ) ) {
		Manager::Update ( );
		ReKat::grapik::Update ( );
	}
	Manager::Free ( );
	ReKat::grapik::Terminate ( );
}