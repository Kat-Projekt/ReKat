#define OPENGL
#define OPENAL
#define ALL_MODULES
#include <ReKat.hpp>

using namespace ReKat::grapik::Input;


int Load ( ) {
	int result = 0;
	result += Manager::Shader_Load  ( "sprite", "sprite.vs", "sprite.fs");
	result += Manager::Shader_Load  ( "font", "font.vs", "font.fs");
	result += Manager::Texture_Load ( "sprite", "sprite.png" );
	result += Manager::Font_Load 	( "font", "death_record.ttf", 100 );

	Objekt main ( "main" );
	Objekt cam ( "pier" );
	Objekt gio  ( "gio", {0,400,0}, {400,200,0} );
	Objekt pie  ( "pie", {0,0,0}, {100,100,10} );
	
	main.Add_Child ( &cam );
	main.Add_Child ( &gio );
	main.Add_Child ( &pie );

	main.Add_Component < Fps > ( );
	cam.Add_Component < Camera > ( );

	gio.Add_Component < Sprite > ( )-> Set ( Manager::Font_Get( "font" )->Get_Texture( ), 
				   Manager::Shader_Get( "sprite" ), cam.Get_Component < Camera >( ) );
	
	pie.Add_Component < Text > ( )->Set ( Manager::Font_Get ( "font" ), 
				   Manager::Shader_Get( "font" ), cam.Get_Component < Camera >( ), {0.5,0,0.6,1} );

	Scene_Manager::Set_Active_Scene ( &main );
	pie.Get_Component < Text > ( )->Set ( "Il Kat disse: [...]" );

	while ( ReKat::grapik::IsEnd ( ) ) {
		glClearColor(0.6, 0.4, 0.5, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		Scene_Manager::Update ( );
		ReKat::grapik::Update ( );
    }

	std::cout << "\nresources loaded\n";
	return result;
}