#ifndef TILEMAP_H
#define TILEMAP_H

#include "sprite.h"
#include <vector>
#include <iostream>

// only one tileset
// can load multiple layers
class Tilemap {
private:
    Sprite   *s_renerer;
    struct Layer {
        int Width, Heigth;
        std::vector<int> Data;
    };
    int Width, Heigth;
    int N_layers = 1;
    Layer *layers;
public:
    enum Status {
        SUCCESS,
        FAILED_OPEN_TILEMAP
    };
    Tilemap ( Sprite *render )
    { s_renerer = render; }
    Tilemap ( Sprite *render, std::string path )
    { s_renerer = render; Make ( path ); }

    // pass a path to a csv file
    int Make ( std::string path ) {
        std::vector < std::string > Data;
        std::ifstream Data_stream;
        // ensure ifstream objects can throw exceptions:
        Data_stream.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try {
            Data_stream.open ( path );
            std::string intermediate;
            std::stringstream sData_stream;
            sData_stream << Data_stream.rdbuf();
            Data_stream.close ();
            // Tokenizing 
            int H = 0;
            while ( getline ( sData_stream, intermediate, '\n' ) ) { H++; Data.push_back(intermediate); }
            Heigth = H;
        }
        catch(const std::exception& e) { std::cerr << e.what() << '\n'; return FAILED_OPEN_TILEMAP; }
        
        std::vector <int> values;
        for ( std::string s : Data ) {
            std::string intermediate;
            std::stringstream sData_stream;
            sData_stream << s;
            // Tokenizing 
            int W = 0;
            while ( getline ( sData_stream, intermediate, ',' ) ) { W++; values.push_back( std::stoi(intermediate)); }
            Width = W;
        }

        // std::cout << "loaded image with: W=" << Width << " H=" << Heigth << '\n';

        layers = (Layer*) calloc (N_layers,sizeof(Layer));
        for ( size_t i = 0; i < N_layers; i++ ) {
            layers[i].Heigth = Heigth;
            layers[i].Width = Width;
            layers[i].Data = values;
        }
        return SUCCESS;
    }

    // render:
    // 1,2,3,4
    // 5,6,7,8
    // .......
    void Draw ( glm::vec2 pos, glm::vec2 tile_size ) {
        for (size_t _layer = 0; _layer < N_layers; _layer++) {
            std::vector<int> data = layers[_layer].Data;
            for (size_t y = 0; y < Heigth; y++) { 
            for (size_t x = 0; x < Width; x++) {
                int location = y * Width + x;
                if ( data[location] >= 0 ) 
                { s_renerer->Draw_frame ( data[location], pos + glm::vec2(x,y) * tile_size, tile_size, 0 ); }
            } }
        }
    }
};

#endif