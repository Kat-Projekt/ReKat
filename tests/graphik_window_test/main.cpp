#include "engine.hpp"
#include "extensions/graphik/graphik.hpp"

int main ( )
{
	ReKat::grapik::Start ( "Window Test", 800, 600 );

	while ( ReKat::grapik::Is_End ( ) )
	{
		ReKat::grapik::Clear_Screen ( 1.0f );
		ReKat::grapik::Update ( );
	}
}