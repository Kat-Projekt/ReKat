#define DIAGNOSTIC
#include <engine.hpp>
#include "Scripts/Player.h"
#include "Scripts/Camera_Controll.h"
#include "Scripts/Enemies/Master_Mind.h"
#include "Scripts/Life_display.h"
#include "Scripts/Dungeon/Dungeon.h"

int load ( ) {
	int result = 0;
	result += Manager::Shader_Load ( "sprite", "Shaders\\sprite.vs", "Shaders\\sprite.fs" );
	result += Manager::Shader_Load ( "font", "Shaders/font.vs", "Shaders/font.fs" );
	result += Manager::Shader_Load ( "tilemap", "Shaders/tilemap.vs", "Shaders/tilemap.fs" );
	result += Manager::Texture_Load ( "icon", "Sprites/icon.png" );
	result += Manager::Texture_Load ( "attack_mode", "Sprites/UI/attack_mode.png" );
	result += Manager::Texture_Load ( "heart", "Sprites/UI/heart.png" );
	result += Manager::Texture_Load ( "soldato", "Sprites/Player/soldato.png" );
	result += Manager::Texture_Load ( "bean", "Sprites/Spells/bean.png" );
	result += Manager::Texture_Load ( "dart", "Sprites/Spells/dart.png" );
	result += Manager::Texture_Load ( "wall", "Sprites/Spells/wall.png" );
	result += Manager::Texture_Load ( "weel", "Sprites/Spells/weel.png" );
	result += Manager::Texture_Load ( "bastone", "Sprites/Weapons/bastone.png" );
	result += Manager::Texture_Load ( "spada", "Sprites/Weapons/spada.png" );
	result += Manager::Texture_Load ( "gobelini", "Sprites/Enemies/gobelini.png" );
	result += Manager::Texture_Load ( "gobelini_dark", "Sprites/Enemies/gobelini_dark.png" );
	result += Manager::Texture_Load ( "gobeloni", "Sprites/Enemies/gobeloni.png" );
	result += Manager::Texture_Load ( "gobeloni_dark", "Sprites/Enemies/gobeloni_dark.png" );
	result += Manager::Texture_Load ( "gobelino", "Sprites/Enemies/gobelino.png" );
	result += Manager::Texture_Load ( "yoda", "Sprites/Enemies/yoda.png" );
	result += Manager::Texture_Load ( "tilemap", "Sprites/Tilemaps/tilemap.png" );
	result += Manager::Font_Load ( "death_record", "Fonts/death_record.ttf", 100 );
	if ( result != 0 ) { throw; }
	// creating objekts 
	// creating Scenes
	auto dungeon = new Objekt ( "Dungeon" );
	auto death = new Objekt ( "Death" );

	// creating Objekts
	auto UI_Container = new Objekt ( "UI" );	
	auto camera = new Objekt ( "Camera" );
	auto map = new Objekt ( "map" );
	auto mode_indicator = new Objekt ( "mode indicator", {-500,-450,0} );
	auto life_indicator = new Objekt ( "life indicator", {-500,480,0} );
	auto name_indicator = new Objekt ( "name indicator", {500,450,0} );
	auto player = new Objekt ( "Player" );
	auto sword = new Objekt ( "weapon" );
	auto enemy = new Objekt ( "enemy", {100,100,0} );

	auto death_text = new Objekt ( "death massage" );

	// creating Objekts Hierarchy
	dungeon->Add_Child ( UI_Container );
	dungeon->Add_Child ( camera );
	dungeon->Add_Child ( player );
	dungeon->Add_Child ( enemy );
	dungeon->Add_Child ( map );
	UI_Container->Add_Child ( mode_indicator );
	UI_Container->Add_Child ( name_indicator );
	UI_Container->Add_Child ( life_indicator );
	player->Add_Child ( sword );

	death->Add_Child ( death_text );

	// adding Components
	dungeon->Add_Component < Phisiks > ( );
	//dungeon->Add_Component < Fps > ( );
	camera->Add_Component < Camera > ( );
	camera->Add_Component < Camera_Controll > ( );
	map->Add_Component < Dungeon > ( )->_Start();
	mode_indicator->Add_Component < Sprite > ( );
	name_indicator->Add_Component < Text > ( );
	// life_indicator->Add_Component < Life_Indicator > ( );
	player->Add_Component < Player > ( );
	player->Add_Component < Sprite > ( );
	player->Add_Component < Rigidbody > ( );
	player->Add_Component < Sfere_Collider > ( ) -> Set_Size ( 60 );
	sword->Add_Component < Sprite > ( );
	sword->Add_Component < Weapon > ( );
	sword->Add_Component < Sfere_Collider > ( )->Set_Trigger ( true );
	auto mm = enemy->Add_Component < Master_Mind > ( );

	death_text->Add_Component < Text > ( );

	// configuring Components
	auto cam = camera->Get_Component < Camera > ( );
	camera->Get_Component < Camera_Controll > ( ) -> target = player;
	map->Get_Component < Tilemap > ( ) -> Set
	( "Maps/Dungeon.csv", Manager::Texture_Get( "tilemap" ), Manager::Shader_Get ( "tilemap" ), cam, {32,32} );
	mode_indicator->Get_Component < Sprite > ( ) -> Set 
	( Manager::Texture_Get ( "attack_mode" ), Manager::Shader_Get ( "sprite" ), cam, {2,1} )->Set ( true );
	name_indicator->Get_Component < Text > ( )->Set ( Manager::Font_Get ( "death_record" ), Manager::Shader_Get ( "font" ), cam, {0.5,0.6,0.1,1} );
	name_indicator->Get_Component < Text > ( )->Set ( "pier" );
	// life_indicator->Get_Component < Life_Indicator > ( )->cam = cam;
	// life_indicator->Get_Component < Life_Indicator > ( )->life = &player->Get_Component < Player > ( )->life;
	player->Get_Component < Sprite > ( ) -> Set 
	( Manager::Texture_Get ( "soldato" ), Manager::Shader_Get ( "sprite" ), cam );
	sword->Set_Rot_Pivot ( {0,-1,0} );
	sword->Get_Component < Sprite > ( ) -> Set 
	( Manager::Texture_Get ( "spada" ), Manager::Shader_Get ( "sprite" ), cam );
	mm->cam = cam;
	mm->player = player;

	death_text->Get_Component < Text > ( )->Set ( Manager::Font_Get ( "death_record" ), Manager::Shader_Get ( "font" ), cam, {0.5,0.6,0.1,1} );
	death_text->Get_Component < Text > ( )->Set ( "yo mama stupid" );
	// adding entry point
	Manager::Set_Active_Scene ( dungeon );
	return 0;
}