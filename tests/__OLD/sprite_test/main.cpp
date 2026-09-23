#include "engine.hpp"
#include "extensions/graphik/graphik.hpp"
#include "extensions/graphik/manager.hpp"

#include "extensions/graphik/components/sprite.h"
#include "extensions/graphik/components/framebuffer.h"

class Move : public Behaviour
{
	void Update ( )
	{
		if ( ReKat::Graphik::_current_window->input.Key_Pressed ( "W" ) )
		{
			obj->Inc_Pos ( {0,10,0} );
		}

		if ( ReKat::Graphik::_current_window->input.Key_Pressed ( "S" ) )
		{
			obj->Inc_Pos ( {0,-10,0} );
		}
	}
};

int main ( )
{
	ReKat::Graphik::Start ( "Sprite Test", 800, 600, true );
	if ( Manager::Defaults_Load ( ) != 0 )
	{ DEBUG ( 1, "failed to load defaults shaders"); }

	Manager::Make < Texture > ( "coconut", "coconut.jpg" );

	auto giorgio = Manager::Objekt_Load ( "Giorgio" );

	giorgio->Add_Component < Camera > ( );
	giorgio->Add_Component < Move > ( );
	giorgio->Add_Component < Sprite > ( )->Set ( "coconut" ).Set ( true );

	Manager::Set_Active_Scene ( "Giorgio" );

	while ( ReKat::Graphik::Is_End ( ) )
	{
		// ReKat::Graphik::Clear_Screen ( 1.0f );
		Manager::Update ( );
		ReKat::Graphik::Update ( );
	}

	Manager::Free ( );
	Manager::Free_Objekt ( "Giorgio" );
}