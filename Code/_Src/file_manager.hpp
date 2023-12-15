#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include "global.hpp"

int generate_id ( size_t hash ) {
    int D = time (0);
    std::hash <size_t> hasher;
    return hasher ( D + hash );
}

std::string setup ( std::string name, std::string pass, size_t * ID ) {
    std::hash <std::string> hasher;
    size_t path_hash      = hasher ( name + pass );
    size_t generator_hash = hasher ( name + "_" + pass );

    std::string path = std::to_string ( path_hash );

    if ( std::filesystem::exists ( path ) ) { 
        // read ID from file
        std::ifstream ID_file(path+"/ID");
        ID_file.read((char*)ID, sizeof(size_t));
        // load history
        std::ifstream HI_file(path+"/history");
        
        std::string line;
        while ( std::getline(HI_file, line) ) {
            history.push_back ( line );
            current_command++;
        }

        return path; 
    }

    std::filesystem::create_directory ( path );
    // create ID file
    *ID = generate_id ( path_hash );
    std::ofstream ID_file(path+"/ID");
    ID_file.write((char*)ID,sizeof(size_t));

    return path;
}

void dump_history ( std::string path ) {
    std::ofstream HI_file ( path+"/history" );
    std::string last = "";
    for ( auto l : history ) { 
        if ( l != "" ) { 
            if ( l != last ) 
            { HI_file << l << '\n'; last = l; }
        } 
    }
}