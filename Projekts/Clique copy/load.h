#include "com.h"

#include "player.h"
#include "speels.h"
#include "camera_controll.h"
#include "gobleino.h"

static void __FreamBufferResize ( GLFWwindow* window, int width, int height ) {
    glViewport ( 0, 0, ReKat::grapik::Internal::SCR_WIDTH = width, ReKat::grapik::Internal::SCR_HEIGTH = height );
}

int Load ( ) {
	ReKat::grapik::Input::_FreamBufferResize = __FreamBufferResize;

	int result = 0;
	result += Manager::Font_Load 	( "font", "death_record.ttf" );

	result += Manager::Shader_Load  ( "sprite", "Shaders/sprite.vs", "Shaders/sprite.fs");
	result += Manager::Shader_Load  ( "tilemap", "Shaders/tilemap.vs", "Shaders/tilemap.fs");

	result += Manager::Texture_Load ( "sprite", "Collider_Shape.png" );
	result += Manager::Texture_Load ( "tilemap", "tile1.png" );
	result += Manager::Texture_Load ( "Bean", "Spells/Beans.png" );
	result += Manager::Texture_Load ( "Lasur", "Spells/Lasur.png" );
	result += Manager::Texture_Load ( "Wall", "Spells/LE wall.png" );
	result += Manager::Texture_Load ( "Spells", "Spells/spells.png" );
	if ( result != 0 ) { return 1; }

	std::cout << "Resources Loaded\n";

	Objekt main ( "main" );
	Objekt pier ( "pier", {300,-100,0.4}, {100,100,100});
	Objekt gio ( "gio",{300,0,0.5}, {50,50,50} );
	Objekt speel ( "spell" );
	Objekt WALL ( "spell", {0,200,0} );
	Objekt Map ( "map", {0,100,0},{100,100,10} );
	
	main.Add_Child ( &camera );
	main.Add_Child ( &pier );
	main.Add_Child ( &gio );
	main.Add_Child ( &speel );
	main.Add_Child ( &WALL );
	main.Add_Child ( &Map );

	std::cout << "Objects Linked\n";

	main.Add_component < Fps > ( )->MAX_FPS = -1;
	main.Add_component < Phisiks > ( );
	camera.Add_component < Camera > ( );
	camera.Add_component < Camera_Controll > ( )->_target = &pier;
	pier.Add_component < Sprite > ( );
	pier.Add_component < Rigidbody > ( );
	pier.Add_component < Player > ( );
	pier.Add_component < Box_Collider > ( )->Set_Size(100);
	gio.Add_component < Sprite > ( );
	gio.Add_component < Rigidbody > ( );
	gio.Add_component < Box_Collider > ( )->Set_Size(50);
	gio.Add_component < Gobleino > ( )->target = &pier;
	speel.Add_component < Spells > ( )->Player = &pier;

	Map.Add_component < Tilemap > ( );
	Map.Add_component < Tilemap_Collider > ( );

	std::cout << "Components Added\n";

	pier.Get_component < Sprite > ( ) ->
	Set ( Manager::Texture_Get( "sprite" ), Manager::Shader_Get( "sprite" ), camera.Get_component < Camera > ( ), {2,1} )->frame = 1;
	
	gio.Get_component < Sprite > ( ) ->
	Set ( Manager::Font_Get( "font" )->Get_Texture( ), Manager::Shader_Get( "sprite" ), camera.Get_component < Camera > ( ), {1,1} )->frame = 0;

	Map.Get_component < Tilemap > ( ) ->
	Set ( "--tilemap.csv", Manager::Texture_Get( "tilemap" ), Manager::Shader_Get( "tilemap" ), camera.Get_component < Camera > ( ), {32,32} );
	Map.Get_component < Tilemap_Collider > ( ) -> Set ( "Maps/Dungeon.c.csv" );

	std::cout << "Components Configured\n";

	Scene_Manager::Set_Active_Scene ( &main );

	std::cout << "Scene Configured\n";

	std::cout << "-------------- Start Main Loop -----------------\n";
	while ( ReKat::grapik::IsEnd ( ) ) {
		glClearColor(0.0, 0.0, 0.0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		Scene_Manager::Update();
		ReKat::grapik::Update();
		// std::cout << "\n--- Rendered: " << main.Get_Childrens ().size();
		// std::cout << "\n------------ End Frame\n";
	}
	std::cout << "\n-------------- Ended Main Loop -----------------\n";

	return result;
}