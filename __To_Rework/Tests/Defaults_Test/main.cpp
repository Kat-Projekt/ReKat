#define DIAGNOSTIC
#include <engine.hpp>

int main ( ) {
	ReKat::grapik::Start ( "Defaults", 800, 600, false, false, true );

	if ( Manager::Defaults_Load ( ) != 0 ) { DEBUG ( 1, "failed to load defaults shaders"); }

	auto Entry_Obj = Manager::Objekt_Load ( "entry" );

	// renderers tests
	auto Text_Obj = Manager::Objekt_Load ( "text" );
	auto Frame_Obj = Manager::Objekt_Load ( "frame" );
	auto Sprite_Obj = Manager::Objekt_Load ( "sprite" );

	Text_Obj->Add_Component < Text > ( )->Set ( "Font.ttf", "", "" )->Set ( "grissin bon grissin bon");
	Sprite_Obj->Add_Component < Sprite > ( )->Set ( "Logo.png" );
	Frame_Obj->Add_Component < Framebuffer > ( )->Set ( "" );

	// UI elements tests
	auto Input_Obj = Manager::Objekt_Load ( "input", {0,100,0} );
	auto Button_Obj = Manager::Objekt_Load ( "button", {0,-100,0} );

	Input_Obj->Add_Component < Input_Box > ( );
	Button_Obj->Add_Component < Button > ( );

	// confiure suite for default shaders
	Entry_Obj->Add_Child ( Text_Obj );
	Entry_Obj->Add_Child ( Frame_Obj );
	Entry_Obj->Add_Child ( Sprite_Obj );

	// configure suite for UI elements
	// Entry_Obj->Add_Child ( Input_Obj );
	// Entry_Obj->Add_Child ( Button_Obj );

	Manager::Set_Active_Scene ( Entry_Obj );

	while ( ReKat::grapik::IsEnd ( ) ) {
		ReKat::grapik::Clear_Screen ( 0.0f, 0.5f,0.1f,1.0f );
		Manager::Update ( );
		ReKat::grapik::Update ( );
	}

	ReKat::grapik::Terminate ( );
}