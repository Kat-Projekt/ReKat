#define OPENGL
#define OPENAL
#define ONLINE_PEER
#define ALL_MODULES
#include <ReKat.hpp>
#include "gravitate.h"
#include "camera_controll.h"

int Load ( ) {
	int result = 0;

	result += Manager::Shader_Load  ( "sprite", "sprite.vs", "sprite.fs");
	result += Manager::Shader_Load  ( "frame", "framebuffers.vs", "framebuffers.fs");

	result += Manager::Texture_Load ( "sprite", "sprite.png" );
	if ( result != 0 ) { return 1; }

	Objekt main ( "main" );

	Objekt Frame ( "FF" );
	Frame.Add_component < Framebuffer > ( )->Set( &main, Manager::Shader_Get( "frame" ), Frame.Add_component < Camera > ( ) );

	Objekt pier ( "pier",{-500,0,0}, {100,100,10} );
	Objekt gio ( "gio",{500,0,0}, {200,200,100} );
	Objekt camera ( "camera" );
	
	main.Add_Child ( &camera );
	main.Add_Child ( &pier );
	main.Add_Child ( &gio );

	main.Add_component < Fps > ( )->MAX_FPS = -1;
	main.Add_component < Phisiks > ( );
	camera.Add_component < Camera > ( );
	camera.Add_component < Camera_controll > ( );
	pier.Add_component < Sprite > ( );
	pier.Add_component < Rigidbody > ( )->velocity = {5,2,0};
	pier.Add_component < Sfere_Collider > ( )->Set_Size ( 100 );
	gio.Add_component < Sprite > ( );
	gio.Add_component < Rigidbody > ( )->mass = 100000000000000000;
	gio.Add_component < Sfere_Collider > ( )->Set_Size ( 200 );

	pier.Add_component < Gravitate > ( )->target = &gio;
	pier.Get_component < Gravitate > ( )->mass = LLONG_MAX;
	gio.Add_component < Gravitate > ( )->target = &pier;
	pier.Get_component < Gravitate > ( )->mass = 10;

	pier.Get_component < Sprite > ( ) ->
	Set ( Manager::Texture_Get( "sprite" ), Manager::Shader_Get( "sprite" ), camera.Get_component < Camera > ( ), {2,1}, {0,0,1,1} )->frame = 0;
	
	gio.Get_component < Sprite > ( ) ->
	Set ( Manager::Texture_Get( "sprite" ), Manager::Shader_Get( "sprite" ), camera.Get_component < Camera > ( ), {2,1}, {0,0,1,1} )->frame = 0;

	main.Start ( );
	Scene_Manager::Set_Active_Scene ( &Frame );
	gio.Get_component < Sfere_Collider > ( )->Set_Static ( true );
	std::cout << "-------------- Start Main Loop -----------------\n";
	// Main_Loop_FrameBuf ( );
	while ( ReKat::grapik::IsEnd ( ) ) {
		Scene_Manager::Update();
		ReKat::grapik::Update();
	}
	std::cout << "\n-------------- Ended Main Loop -----------------\n";

	return result;
}
