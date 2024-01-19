#include "../global.h"
#include "dialog.h"
#include "player.h"
#include "camera.h"
#include "Enemies/enemy.h"
#include "dungeon.h"
#include "dungeon_special.h"
#include "mouseponter.h"
#include "speels.h"

int Load_resources ( ) {
	int result = 0;
	// Load Shaders
	result += Manager::Shader_Load ( "text", "Shaders/text.vs", "Shaders/text.fs" );
	result += Manager::Shader_Load ( "sprite_1x1",  "Shaders/sprite_animation.vs", "Shaders/sprite_animation.fs" );
	result += Manager::Shader_Load ( "sprite_2x1",  "Shaders/sprite_animation.vs", "Shaders/sprite_animation.fs" );
	result += Manager::Shader_Load ( "sprite_2x2",  "Shaders/sprite_animation.vs", "Shaders/sprite_animation.fs" );
	result += Manager::Shader_Load ( "Gobelin",  "Shaders/sprite_animation.vs", "Shaders/sprite_animation.fs" );
	result += Manager::Shader_Load ( "Beans",  "Shaders/sprite_animation.vs", "Shaders/sprite_animation.fs" );
	result += Manager::Shader_Load ( "empty_sprite",  "Shaders/sprite_animation.vs", "Shaders/sprite_animation.fs" );
	result += Manager::Shader_Load ( "empty_sprite_UI",  "Shaders/sprite_animation.vs", "Shaders/sprite_animation.fs" );
	result += Manager::Shader_Load ( "tilemap", "Shaders/sprite_animation.vs", "Shaders/sprite_animation.fs" );
	result += Manager::Shader_Load ( "UI", "Shaders/sprite_animation.vs", "Shaders/sprite_animation.fs" );
	result += Manager::Shader_Load ( "Frame", "Shaders/framebuffer.vs", "Shaders/framebuffer.fs" );
	// Load Shader Result 
	if ( result != 0 )
	{ std::cout << "Shaders  Make Error: " << result <<'\n'; return 1; } std::cout << "Shaders  Load Correctly\n";

	// Load Textures
	result += Manager::Texture_Load ( "Player", "Sprites/soldato.png" );
	result += Manager::Texture_Load ( "Spada", "Sprites/Bastone.png" );
	result += Manager::Texture_Load ( "Buttons", "Sprites/UI_menu.png" );
	result += Manager::Texture_Load ( "empty_sprite", "Data/empty.png" );
	result += Manager::Texture_Load ( "tileset", "Data/tile1.png" );
	result += Manager::Texture_Load ( "Dialog_Player", "Sprites/Dialog/soldato.png" );
	result += Manager::Texture_Load ( "Dialog_Spada", "Sprites/Dialog/spada.png" );
	result += Manager::Texture_Load ( "cursor", "cursor.png" );
	result += Manager::Texture_Load ( "Bean", "Sprites/Spells/Beans.png" );
	result += Manager::Texture_Load ( "Lasur", "Sprites/Spells/Lasur.png" );
	result += Manager::Texture_Load ( "Wall", "Sprites/Spells/LE wall.png" );
	result += Manager::Texture_Load ( "Spells", "Sprites/Spells/spells.png" );
	// enemy texture
	result += Manager::Texture_Load ( "gobelino", "Sprites/yoda.png" );
	// Load Textures Result 
	if ( result != 0 ) 
	{ std::cout << "Textures Make Error: " << result <<'\n'; return 1; } std::cout << "Textures Load Correctly\n";
		
	// Load Text
	result = Manager::Text_Load ( "death_record", "death_record.ttf", "text" );
	// Load Text Result 
	if ( result != 0 ) 
	{ std::cout << "Texts	 Make Error: " << result <<'\n'; return 1; } std::cout << "Texts	 Load Correctly\n";
		
	// configure resources
	Manager::Shader_Get("sprite_1x1")->setInt ("image", 0);
	Manager::Shader_Get("sprite_2x2")->setInt ("image", 0);
	Manager::Shader_Get("UI")->setInt ("image", 0);
	Manager::Shader_Get("empty_sprite")->setInt ("image", 0);
	Manager::Shader_Get("tilemap")->setInt ("image", 0 );
	Manager::Shader_Get("tilemap")->setInt ("screenTexture", 0 );

	// configure renderers
	Manager::Sprite_Load ( "Player", "sprite_1x1", "Player", {1,1} );
	Manager::Sprite_Load ( "Bean", "Beans", "Bean", {2,2} );
	Manager::Sprite_Load ( "Lasur", "sprite_2x1", "Lasur", {2,1} );
	Manager::Sprite_Load ( "Wall", "sprite_2x1", "Wall", {2,1} );
	Manager::Sprite_Load ( "Spada", "sprite_1x1", "Spada", {1,1} );
	Manager::Sprite_Load ( "Dialog_Spada", "empty_sprite_UI", "Dialog_Spada", {1,1} );
	Manager::Sprite_Load ( "Dialog_Player", "empty_sprite_UI", "Dialog_Player", {1,1} );
	Manager::Sprite_Load ( "cursor", "empty_sprite_UI", "cursor", {1,1} );
	Manager::Sprite_Load ( "gobelino", "Gobelin", "gobelino", {2,3} );
	Manager::Sprite_Load ( "Spells", "sprite_2x2", "Spells", {2,4} );
	// Manager::Sprite_Load ( "sprite", "sprite", "sprite", {1,1} );
	Manager::Sprite_Load ( "UI", "UI", "Buttons", {3,4} );
	Manager::Sprite_Load ( "empty_sprite", "empty_sprite", "empty_sprite" );
	Manager::Sprite_Load ( "empty_sprite_UI", "empty_sprite_UI", "empty_sprite" );
	Manager::Sprite_Load ( "tilemap", "tilemap", "tileset", {32,32} );

	// Load Tilemaps
	result = Manager::Tilemap_Load ( "tilemap", "tilemap", "Data/Tilemaps/Layer2.csv" );
	// Load Tilemaps Result
	if ( result != 0 ) 
	{ std::cout << "Tilemaps Make Error: " << result <<'\n'; return 1; } std::cout << "Tilemaps Load Correctly\n";
		
	// Create animation
	Manager::Animator_Load ( "walk" );
	Manager::Animator_Get  ( "walk" )->Create_Animation("walk")->Add_Frames({{0,0.2},{1,0.2},{2,0.2},{3,0.2}});

	return result;
}

void UI_Object_callback ( ) { ReKat::grapik::End(); }

void Create_Objs ( ) {
	// configure UI objects
	Manager::UI_Object_Load ( "start", "", "UI", "death_record", {-600,450}, {100,100}, UI_Object_callback, 6 );
	Manager::UI_Object_Load ( "output", "output", "empty_sprite", "death_record", {0,-200}, {1000,300}, nullptr, 6 )->Set_Color({0,1,0,1});
	Manager::UI_Object_Load ( "input", "", "empty_sprite", "death_record", {0,200}, {1000,300}, nullptr, 6 )->Set_Color({1,0,0,1});
	Manager::UI_Object_Load ( "Connecting", "Connecting", "empty_sprite", "death_record", {0,0}, {1000,1000}, nullptr, 6 )->Set_Color({1,1,1,1});

	// dialog box
	Manager::UI_Object_Load ( "Dialog", "Ciao, Signor Moro Aldo", "empty_sprite_UI", "death_record", {0,-350}, {1000,300}, nullptr, 6 )->Set_Color({1,1,1,1});
	Manager::UI_Object_Load ( "Dialog_Player", "", "Dialog_Player", "death_record", {-600,-250}, {500,500}, nullptr, 6 )->Set_Color({1,1,1,1});
	Manager::UI_Object_Load ( "Dialog_Char", "", "Dialog_Spada", "death_record", {600,-250}, {500,500}, nullptr, 6 )->Set_Color({1,1,1,1});

	Manager::UI_Object_Load ( "Mouse", "", "cursor", "", {0,0}, {30,30}, nullptr, 0 )->Add_component<Pointer>();
	
	Manager::UI_Object_Get ( "Dialog" )->Set_Active ( false );
	Manager::UI_Object_Get ( "Dialog" )->Add_Sub_Object( "Player", Manager::UI_Object_Get ( "Dialog_Player" ) );
	Manager::UI_Object_Get ( "Dialog" )->Add_Sub_Object( "Char", Manager::UI_Object_Get ( "Dialog_Char" ) );

	// configure player
	Manager::Object_Load ( "Player", "Player", {600,600}, {100,100} );
	Manager::Object_Load ( "G", "empty_sprite", {100,300}, {100,100} )->Set_Color ({1,0,0,1});
	Manager::Object_Load ( "S", "empty_sprite", {300,100}, {100,100} )->Set_Color ({0,1,0,1});
	Manager::Object_Load ( "Spada", "Spada",{0,0}, {100,100} );

	Manager::Object_Get("Spada")->Set_Rotation_Pivot({0.5,0});
	Manager::Object_Get("Player")->Add_Sub_Object ( "Spada", Manager::Object_Get("Spada") );
	Manager::Object_Get("Player")->Add_component <Player> ( );
	Manager::Object_Get("Player")->Add_component <_Camera> ( )->Pointer = Manager::Object_Get("Player");
	Manager::Object_Get("Player")->Set_Altitude ( 1 );

	Manager::Object_Get("S")->Add_component<Dialog> ();

	Manager::Add_Collider ("Player") ->Set_size ( {50,70} );
	Manager::Add_Collider ("G") ->Set_size ( {100,100} )->Set_movable(false);
	Manager::Add_Collider ("S") ->Set_size ( {100,100} )->Set_trigger(true);
	Manager::Add_Collider ("Spada") ->Set_size ( { 50,50 } )->Set_trigger(true);

	// congigure objects
	Manager::Object_Load ( "Map", "empty_sprite", {100,100}, {100,100} )->Set_Color({0,0,0,0})->Set_Altitude(-100);
	Manager::Object_Get  ("Map")->Add_component<dungeon>();

	Manager::Object_Load ( "Enemy", "", {700,700}, {100,100} )->Add_component < Enemy > ();
	Manager::Object_Get ( "Enemy" )->Set_Altitude (0);

	Manager::UI_Object_Load ( "Spells", "", "", "",  {0,0}, {0,0}, nullptr )->Add_component < Spells >();

	// std::cout << "Framebuffer " << Manager::Framebuffer_Load ( "test", 100, 100 ) << '\n';
	std::cout << "Objects created\n";

	ReKat::grapik::Input::Configure();
}

void Create_Scenes ( ) {
	Manager::Scene_Load ( "info" );
	Manager::Scene_Get ( "info" )->Add_UI_Shader ( Manager::Shader_Get("empty_sprite") );
	Manager::Scene_Get ( "info" )->Add_UI_Shader ( Manager::Shader_Get("text") );
	Manager::Scene_Get ( "info" )->Add_UI_Object ( Manager::UI_Object_Get( "output" ) );
	Manager::Scene_Get ( "info" )->Add_UI_Object ( Manager::UI_Object_Get( "input" ) );

	Manager::Scene_Load ( "loading" );
	Manager::Scene_Get ( "loading" )->Add_UI_Shader ( Manager::Shader_Get("text") );
	Manager::Scene_Get ( "loading" )->Add_UI_Object ( Manager::UI_Object_Get( "Connecting" ) );

	Manager::Scene_Load ( "main" );
	Manager::Scene_Get ( "main" )->Add_Shader ( Manager::Shader_Get("sprite_1x1") );
	Manager::Scene_Get ( "main" )->Add_Shader ( Manager::Shader_Get("sprite_2x1") );
	Manager::Scene_Get ( "main" )->Add_Shader ( Manager::Shader_Get("Beans") );
	Manager::Scene_Get ( "main" )->Add_Shader ( Manager::Shader_Get("Gobelin") );
	Manager::Scene_Get ( "main" )->Add_UI_Shader ( Manager::Shader_Get("sprite_2x2") );
	Manager::Scene_Get ( "main" )->Add_Shader ( Manager::Shader_Get("empty_sprite") );
	Manager::Scene_Get ( "main" )->Add_UI_Shader ( Manager::Shader_Get("UI") );
	Manager::Scene_Get ( "main" )->Add_UI_Shader ( Manager::Shader_Get("text") );
	Manager::Scene_Get ( "main" )->Add_UI_Shader ( Manager::Shader_Get("empty_sprite_UI") );
	//Manager::Scene_Get ( "main" )->Add_Object ( Manager::Object_Get("Map") );
	Manager::Scene_Get ( "main" )->Add_Object ( Manager::Object_Get("Player") );
	Manager::Scene_Get ( "main" )->Add_Object ( Manager::Object_Get("G") );
	Manager::Scene_Get ( "main" )->Add_Object ( Manager::Object_Get("S") );
	Manager::Scene_Get ( "main" )->Add_Object ( Manager::Object_Get("Enemy") );
	Manager::Scene_Get ( "main" )->Add_UI_Object ( Manager::UI_Object_Get("Spells") );
	Manager::Scene_Get ( "main" )->Add_UI_Object ( Manager::UI_Object_Get( "start" ) );
	Manager::Scene_Get ( "main" )->Add_UI_Object ( Manager::UI_Object_Get( "Dialog" ) );
	Manager::Scene_Get ( "main" )->Add_UI_Object ( Manager::UI_Object_Get( "Mouse" ) );

	std::cout << "Scenes  created\n";
}

int Load_Audio ( ) {
	int result = 0;
	// Load buffers
	int _len = 44100;
	short* _buf = ( short* ) calloc ( _len, sizeof(short));
	for (size_t i = 0; i < _len; i++) { _buf[i] = sin ( i * 0.01 ) * 32000; }
	result += ReKat::synth::Create_Buffer ( "Player", _buf, _len );
	result += ReKat::synth::Create_Buffer ( "Bals", "bones.wav" );
	// Load Buffers Result 
	if ( result != 0 ) 
	{ std::cout << "Buffers  Make Error: " << result <<'\n'; return 1; } std::cout << "Buffers  Load Correctly\n";

	// Load sources
	result += ReKat::synth::Create_Source ( "Player", {0,0,0} );
	result += ReKat::synth::Create_Source ( "Bals", {0,0,0} );
	// Load Sources Result 
	if ( result != 0 ) 
	{ std::cout << "Sources  Make Error: " << result <<'\n'; return 1; } std::cout << "Sources  Load Correctly\n";

	return result;
}