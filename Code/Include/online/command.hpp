#ifndef ONLINE_COMMAND
#define ONLINE_COMMAND
// this scripts is the interface between the user and the netowrk it is used to run commands that are coing to influence the network
enum command_type {
    CONNECT,
    DISCONNECT,
    SEND,
    FIND
};

#include "online.hpp"

struct parsed_command {
    command_type type;
    
};

int command ( std::string comm ) {
    // parse the command
    
}

#endif