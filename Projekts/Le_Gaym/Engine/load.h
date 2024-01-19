#define OPENGL
#define OPENAL
#define ONLINE_PEER
#define ALL_MODULES
#include <ReKat.hpp>

int Load ( ) {
	int result = 0;

resources: {
	/* Shaders */
	result += Manager::Shader_Load ( "text", "Shaders/text.vs", "Shaders/text.fs" );
	result += Manager::Shader_Load ( "sprite",  "Shaders/sprite_animation.vs", "Shaders/sprite_animation.fs" );
	result += Manager::Shader_Load ( "UI_sprite",  "Shaders/sprite_animation.vs", "Shaders/sprite_animation.fs" );
	if ( result != 0 ) { std::cout << "Shaders  Make Error: " << result <<'\n'; return result; } 
	std::cout << "Shaders  Load Correctly\n";

	// configure resources
	Manager::Shader_Get("sprite")->setInt ("image", 0);
	Manager::Shader_Get("UI_sprite")->setInt ("image", 0);

	/* Textures */
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
	if ( result != 0 ) { std::cout << "Textures Make Error: " << result <<'\n'; return result; }
	std::cout << "Textures Load Correctly\n";

	/* Text */
	result = Manager::Text_Load ( "death_record", "death_record.ttf", "text" );
	// Load Text Result 
	if ( result != 0 ) { std::cout << "Texts    Make Error: " << result <<'\n'; return result; } 
	std::cout << "Texts    Load Correctly\n";

	// configure renderers
	Manager::Sprite_Load ( "Player", "sprite", "Player", {1,1} );
	Manager::Sprite_Load ( "Spada", "sprite", "Spada", {1,1} );
	Manager::Sprite_Load ( "Lasur", "sprite", "Lasur", {2,1} );
	Manager::Sprite_Load ( "Bean", "sprite", "Bean", {2,2} );
	Manager::Sprite_Load ( "Wall", "sprite", "Wall", {2,1} );
	Manager::Sprite_Load ( "Dialog_Spada", "empty_sprite_UI", "Dialog_Spada", {1,1} );
	Manager::Sprite_Load ( "Dialog_Player", "empty_sprite_UI", "Dialog_Player", {1,1} );
	Manager::Sprite_Load ( "cursor", "empty_sprite_UI", "cursor", {1,1} );
	Manager::Sprite_Load ( "gobelino", "sprite", "gobelino", {2,3} );
	Manager::Sprite_Load ( "Spells", "sprite_2x2", "Spells", {2,4} );
	Manager::Sprite_Load ( "UI", "UI", "Buttons", {3,4} );
	Manager::Sprite_Load ( "empty_sprite", "empty_sprite", "empty_sprite" );
	Manager::Sprite_Load ( "empty_sprite_UI", "empty_sprite_UI", "empty_sprite" );
	Manager::Sprite_Load ( "tilemap", "tilemap", "tileset", {32,32} );
}

objects: {

}

scenes: {

}

	return result;
}