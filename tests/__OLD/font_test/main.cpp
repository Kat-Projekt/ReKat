#include "engine.hpp"

int main ( )
{
	ReKat::Graphik::Start ( "Font Test", 800, 600 );
	Manager::Defaults_Load ( );
	DEBUG ( 3, "Started" );
	Manager::Make < Font > ( "Aovel", "AovelSansRounded.ttf", 90 );

	Manager::Objekt_Load ( "Testo", {0,0,0} )
		->Add_Component < Text > ( )
			->Set ( "Aovel", "" )
			->Set ( "Hello\nWorld", Text::CENTER );

	Manager::Set_Active_Scene ( "Testo" );

	while ( ReKat::Graphik::Is_End ( ) )
	{
		ReKat::Graphik::Clear_Screen ( 1.0f );
		Manager::Update ( );
		ReKat::Graphik::Update ( );
	}

	Manager::Free ( );
	Manager::Free_Objekt ( "Testo" );
}