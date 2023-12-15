#pragma once

#define OPENGL
#define ALL_MODULES
#define ONLINE_PEER
#include <ReKat.hpp>

#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <filesystem>
#include <chrono>

bool Main_shutdown = false;
std::string Begin_string = "ReKat: ";

int current_command = 0;
std::vector < std::string > history;
std::stringstream output;

char* add_ending ( char* os, size_t s, char end ) {
    char* p = (char*) calloc ( s+1, sizeof(char) );
    for (size_t i = 0; i < s; i++) 
    { p[i] = os[i]; }
    p[s] = end;
    return p;
}

template < typename T >
T remove_ending ( char* os ) {
    char* p = (char*) calloc (sizeof(T),sizeof(char));
    for (size_t i = 0; i < sizeof(T); i++) 
    { p[i] = os[i]; }
    return *((T*)p);
}

uint64_t Get_Time ( ) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}


enum COMMAND_STATUS {
    SUCCESS,
    NO_COMMAND = -1,
    FAULTY_COMMAND = -2,
    INCORRECT_COMMAND = -3,
    TERMINATE = -999
};

bool ctrl = false;
bool shift = false;
bool text_mode = false;

float BGR, BGG, BGB;

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
