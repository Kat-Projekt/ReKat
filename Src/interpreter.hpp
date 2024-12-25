#include <engine.hpp>
#include "parser.hpp"

Projekt _pj;

int load ( Projekt pj ) {
    _pj = pj;
    for (auto &&resource : _pj.Resources ) {
        if ( resource.type == "texture" ) 
        { Manager::Texture_Load ( resource.name, resource.Arguments[0].value.c_str ( ) ); continue; }
        if ( resource.type == "texture" ) {
            switch ( resource.Arguments.size ( ) ) {
            case 2: Manager::Shader_Load ( resource.name, resource.Arguments[0].value.c_str ( ), resource.Arguments[1].value.c_str ( ) ); break;
            case 3: Manager::Shader_Load ( resource.name, resource.Arguments[0].value.c_str ( ), resource.Arguments[1].value.c_str ( ), resource.Arguments[2].value.c_str ( ) ); break;
            case 4: Manager::Shader_Load ( resource.name, resource.Arguments[0].value.c_str ( ), resource.Arguments[1].value.c_str ( ), resource.Arguments[2].value.c_str ( ), resource.Arguments[3].value.c_str ( ) ); break;
            case 5: Manager::Shader_Load ( resource.name, resource.Arguments[0].value.c_str ( ), resource.Arguments[1].value.c_str ( ), resource.Arguments[2].value.c_str ( ), resource.Arguments[3].value.c_str ( ), resource.Arguments[4].value.c_str ( ) ); break;
            
            default: break; }
            continue;            
        }
        
    }
    
    return 0;
}

int start ( ) {

    return 0;
}

int update ( ) {

    return 0;
}

int end ( ) {

    return 0;
}
