#include "engine.hpp"
#include "extensions/graphik/graphik.hpp"

int main ( )
{
	ReKat::Graphik::Start ( "Window Test", 800, 600 );

	while ( ReKat::Graphik::Is_End ( ) )
	{
		ReKat::Graphik::Clear_Screen ( 1.0f );
		ReKat::Graphik::Update ( );
	}
}