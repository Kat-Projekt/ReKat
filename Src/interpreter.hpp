#include <engine.hpp>
#include "parser.hpp"

Projekt _pj;

int Load ( Projekt pj ) {
    // starting up systems
    auto GI = pj.Interface.graphik;
    if ( GI.active ) 
    { ReKat::grapik::Start ( pj.name, GI.width, GI.heigth, GI.transaprent, GI.fullscreen, GI.resizable ); }
    if ( pj.Interface.synth ) 
    { ReKat::synth::Start ( ); }
    if ( pj.Interface.phisiks.active ) 
    { ReKat::phisiks::Start ( pj.Interface.phisiks.frames ); }

    _pj = pj;
    int resource_load_result = 0;
    for (auto &&resource : _pj.Resources ) {
        if ( resource.type == "texture" ) {
            if ( resource.Arguments.size ( ) != 1 ) 
            { DEBUG ( 1, "Incorrenct number of inizializers for texture" ); }
            Manager::Texture_Load ( resource.name, resource.Arguments[0].value.c_str ( ) );
            continue;
        } else if ( resource.type == "shader" ) {
            switch ( resource.Arguments.size ( ) ) {
            case 2: resource_load_result += Manager::Shader_Load ( resource.name, resource.Arguments[0].value.c_str ( ), resource.Arguments[1].value.c_str ( ) ); break;
            case 3: resource_load_result += Manager::Shader_Load ( resource.name, resource.Arguments[0].value.c_str ( ), resource.Arguments[1].value.c_str ( ), resource.Arguments[2].value.c_str ( ) ); break;
            case 4: resource_load_result += Manager::Shader_Load ( resource.name, resource.Arguments[0].value.c_str ( ), resource.Arguments[1].value.c_str ( ), resource.Arguments[2].value.c_str ( ), resource.Arguments[3].value.c_str ( ) ); break;
            case 5: resource_load_result += Manager::Shader_Load ( resource.name, resource.Arguments[0].value.c_str ( ), resource.Arguments[1].value.c_str ( ), resource.Arguments[2].value.c_str ( ), resource.Arguments[3].value.c_str ( ), resource.Arguments[4].value.c_str ( ) ); break;
            default: DEBUG ( 1, "Incorrenct number of inizializers for shader" ); break; }
            continue;            
        } else if ( resource.type == "font" ) {
            switch ( resource.Arguments.size ( ) ) {
            case 1: resource_load_result += Manager::Font_Load ( resource.name, resource.Arguments[0].value.c_str ( ) ); break;
            case 2: resource_load_result += Manager::Font_Load ( resource.name, resource.Arguments[0].value.c_str ( ), std::stoi ( resource.Arguments[1].value ) ); break;
            case 3: resource_load_result += Manager::Font_Load ( resource.name, resource.Arguments[0].value.c_str ( ), std::stoi ( resource.Arguments[1].value ), std::stoi ( resource.Arguments[2].value ) ); break;
            default: DEBUG ( 1, "Incorrenct number of inizializers for font" ); break; }
            continue;            
        } else if ( resource.type == "tilemap" ) {
            if ( resource.Arguments.size ( ) != 1 )
            { DEBUG ( 1, "Incorrenct number of inizializers for tilemap" ); }
            Manager::Tilemap_Load ( resource.name, resource.Arguments[0].value.c_str ( ) );
            continue;
        } else if ( resource.type == "audio_buffer" ) {
            if ( resource.Arguments.size ( ) != 1 ) 
            { DEBUG ( 1, "Incorrenct number of inizializers for audio_buffer" ); }
            Manager::Buffer_Load ( resource.name, resource.Arguments[0].value.c_str ( ) );
            continue;
        }

        DEBUG ( 1, "this resource type <" + resource.type + "> does not exist" );
    }

    
    
    return 0;
}

int Start ( ) {

    return 0;
}

int Update ( ) {

    return 0;
}

int End ( ) {

    return 0;
}
