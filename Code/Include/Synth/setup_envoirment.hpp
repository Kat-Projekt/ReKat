#pragma once
#include "defines.h"

using namespace std::filesystem;

int setup ( ) {
    create_directory ( data_path );
    create_directory ( audio_libs_path );
    create_directory ( save_files_path );
}