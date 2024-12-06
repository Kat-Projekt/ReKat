#ifndef ID_GENERATOR
#define ID_GENERATOR

#include <string>
#include <hash.hpp>

size_t generate_id ( std::string name, std::string pass ) {
    std::hash <std::string> hasher;
    size_t ID = hasher ( name + "_" + pass );
    return ID;
}

#endif