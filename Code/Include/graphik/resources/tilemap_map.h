#ifndef TILEMAP_MAP_H
#define TILEMAP_MAP_H

#include "../../resource.hpp"
#include "../../utility/list.h"

class Tilemap_Map : public Resource {
private:
    unsigned int width = 0;
    unsigned int heigth = 0;
    unsigned int intances = 0;

    struct TileSet {
        glm::ivec2 tiles;
        std::string texture;        
    };

    struct Layer {
        bool Collider = false;
        bool Nav_Mesh = false;
        int * data = nullptr;
        std::string name = "";
    };

    List < Layer > Layers;

    std::string path;
public:
    Tilemap_Map ( ) { }
    Tilemap_Map ( const char * path ) 
    { Make ( path ); };
    
    int Make ( const char * base_path ) {
        // get every layer path name

        DEBUG ( 4, "Successfully loaded tilemap" );
    }
    void End ( void ) { }
    void Use ( void ) { }
    void Use ( float ) { }
};

#endif