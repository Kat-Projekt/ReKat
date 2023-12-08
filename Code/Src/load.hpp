#pragma once
#include "controlls.hpp"
#include "Enemies/enemy.h"

int Load_resources () {
    int result = 0;
    // Load Shaders
    result += Manager::Shader_Load ( "text", "Shaders/text.vs", "Shaders/text.fs" );
    result += Manager::Shader_Load ( "sprite_1x1",  "Shaders/sprite_animation.vs", "Shaders/sprite_animation.fs" );
    result += Manager::Shader_Load ( "sprite_2x2",  "Shaders/sprite_animation.vs", "Shaders/sprite_animation.fs" );
    result += Manager::Shader_Load ( "empty_sprite",  "Shaders/sprite_animation.vs", "Shaders/sprite_animation.fs" );
    result += Manager::Shader_Load ( "tilemap", "Shaders/sprite_animation.vs", "Shaders/sprite_animation.fs" );
    result += Manager::Shader_Load ( "UI", "Shaders/sprite_animation.vs", "Shaders/sprite_animation.fs" );
    // Load Shader Result 
    if ( result != 0 ) 
    { std::cout << "Shaders Make Error: " << result <<'\n'; return 1; } std::cout << "Shaders  Load Correctly\n";

    // Load Textures
    result += Manager::Texture_Load ( "Player", "Sprites/soldato.png" );
    result += Manager::Texture_Load ( "Spada", "Sprites/SPADA.png" );
    result += Manager::Texture_Load ( "Buttons", "Sprites/UI_menu.png" );
    result += Manager::Texture_Load ( "empty_sprite", "Data/empty.png" );
    result += Manager::Texture_Load ( "tileset", "Data/tile1.png" );
    // enemy texture
    result += Manager::Texture_Load ( "gobelino", "Sprites/yoda.png" );
    // Load Textures Result 
    if ( result != 0 ) 
    { std::cout << "Textures Make Error: " << result <<'\n'; return 1; } std::cout << "Textures Load Correctly\n";
    
    // Load Text
    result = Manager::Text_Load ( "death_record", "death_record.ttf", "text" );
    // Load Text Result 
    if ( result != 0 ) 
    { std::cout << "Texts Make Error: " << result <<'\n'; return 1; } std::cout << "Texts    Load Correctly\n";
    
    // configure resources
    Manager::Shader_Get("text")->setMat4("projection", glm::ortho ( 0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT) ));
    Manager::Shader_Get("sprite_1x1")->setInt ("image", 0);
    Manager::Shader_Get("sprite_1x1")->setMat4("projection", cam.GetWiew());
    Manager::Shader_Get("sprite_2x2")->setInt ("image", 0);
    Manager::Shader_Get("sprite_2x2")->setMat4("projection", cam.GetWiew());
    Manager::Shader_Get("UI")->setInt ("image", 0);
    Manager::Shader_Get("UI")->setMat4("projection", cam.GetWiew());
    Manager::Shader_Get("empty_sprite")->setInt ("image", 0);
    Manager::Shader_Get("empty_sprite")->setMat4("projection", cam.GetWiew());
    Manager::Shader_Get("tilemap")->setInt ("image", 0 );
    Manager::Shader_Get("tilemap")->setMat4("projection", cam.GetWiew());

    // configure renderers
    Manager::Sprite_Load ( "Player", "sprite_1x1", "Player", {1,1} );
    Manager::Sprite_Load ( "Spada", "sprite_1x1", "Spada", {1,1} );
    Manager::Sprite_Load ( "gobelino", "sprite_2x2", "gobelino", {2,3} );
    // Manager::Sprite_Load ( "sprite", "sprite", "sprite", {1,1} );
    Manager::Sprite_Load ( "UI", "UI", "Buttons", {3,4} );
    Manager::Sprite_Load ( "empty_sprite", "empty_sprite", "empty_sprite" );
    Manager::Sprite_Load ( "tilemap", "tilemap", "tileset", {32,32} );

    // Load Tilemaps
    result = Manager::Tilemap_Load ( "tilemap", "tilemap", "Data/Tilemaps/Layer2.csv" );
    // Load Tilemaps Result
    if ( result != 0 ) 
    { std::cout << "Tilemaps Make Error: " << result <<'\n'; return 1; } std::cout << "Tilemaps Load Correctly\n";
    
    Enemy_start();
    return result;
}