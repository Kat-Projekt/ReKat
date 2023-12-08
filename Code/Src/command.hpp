#include "global.hpp"
using namespace ReKat;

long long command ( std::string command ) {
    // tokenize
    // Vector of string to save tokens
    std::vector <std::string> tokens;
     
    // stringstream class check1
    std::stringstream check1(command);
    
    std::string intermediate;
     
    // Tokenizing w.r.t. space ' '
    while ( getline ( check1, intermediate, ' ' ) ) { tokens.push_back(intermediate); }

    // command set ---------------------
    // ---------------------------------
    if ( tokens.size( ) == 0 ) { return NO_COMMAND; }

    // args: name, ip, <port>
    if ( tokens[0] == "connect" ) {
        int p = -999;
        if ( tokens.size ( ) == 3 ) { p = online::Connect ( tokens[1], tokens[2] ); }
        if ( tokens.size ( ) == 4 ) { p = online::Connect ( tokens[1], tokens[2], tokens[3].c_str( ) ); }
        
        switch ( p ) {
            case -999: return FAULTY_COMMAND; break;
            case online::SUCCESS: output << "connected to: " << tokens[1] << '\n'; return SUCCESS;        
            default: return p; break;
        }
    }

    // args: node, msg
    if ( tokens[0] == "msg" ) {
        if ( tokens.size ( ) >= 3 ) {
            // check for multiple nodes
            std::vector < std::string > nodes;
            std::stringstream nodes_S (tokens[1]);
            while ( getline ( nodes_S, intermediate, ',' ) ) { nodes.push_back(intermediate); }
            
            for ( auto node : nodes ) {
                // check if connected to node
                bool C = false;
                for ( std::string s : online::Connected ( ) ) { if ( s == node ) { C = true; } }
                if ( C == false ) { output << node << " is not connected\n"; continue; }
                std::string R = "";
                for (size_t i = 2; i < tokens.size(); i++) { R += " " + tokens[i]; }
                R.erase (0,1);
                R += 't';
                output << "to " << node << ": " << R << '\n';
                online::Send ( R.c_str(), R.size(), node ); 
            }
            return SUCCESS;
        }
        output << "command: " << command << " size: " << tokens.size() << '\n';
        return FAULTY_COMMAND;
    }

    if ( tokens[0] == "connected" ) {
        for ( auto s : online::Connected () ) 
        { output << "connected to: " << s << '\n'; } 
        if ( online::Connected().size() == 0 ) { output << "nobody there\n"; }
        return SUCCESS;
    }

    if ( tokens[0] == "clear" ) {
        if ( tokens.size ( ) == 2 ) {
            if ( tokens[1] == "history" ) {
                history.clear( );
                history.push_back ( "" );
                current_command = 0;
                output << "history cleared, exit to apply to disk\n";
                return SUCCESS;
            }
        }
        output.str(""); output << Begin_string; return SUCCESS; 
    }

    if ( tokens[0] == "disconnect" ) {
        if ( tokens.size ( ) == 2 ) {
            if ( tokens[1] == "--all") {
                // disconnect all nodes
                for ( std::string s : online::Connected ( ) ) 
                { online::Close_sock ( s ); std::cout << s + " disconnected\n";}
                return SUCCESS;
            }

            bool C = false;
            for ( std::string s : online::Connected ( ) ) { if ( s == tokens[1] ) { C = true; } }
            if ( C == false ) { output << tokens[1] << " is not connected\n"; return FAULTY_COMMAND; }
            std::cout << tokens[1] + " disconnected\n";
            online::Close_sock ( tokens[1] );
            return SUCCESS;
        }
        return FAULTY_COMMAND;
    }

    return INCORRECT_COMMAND;
}

void execute_command ( ) {
    if (history[current_command] == "exit") { Main_shutdown = true; }
    // output << '\n' << name << "@localhost: " << history[current_command] << '\n'; 
    int r = command ( history[current_command] );
    if ( r != 0 ) {
        switch ( r ) {
            case -1: output << "NO_COMMAND\n"; break;
            case -2: output << "FAULTY_COMMAND\n"; break;
            case -3: output << "INCORRECT_COMMAND\n"; break;
        }
    }
    text_mode = false;
    std::cout << "exiting text mode\n";
    current_command = history.size ( );
    if ( history[history.size()-1] == "" ) { current_command--; return; }
    history.push_back ("");
}

void Send_positon ( ) {
    // send
    char *_buf = add_ending (Manager::Object_Get("Player")->packpos(), sizeof(glm::vec2), 'v' );
    std::cout << "sending pos\n";
    // to every connected node
    for ( auto s : online::Connected() ) {
        std::cout << "sending pos to " << s;
        online::Send ( _buf, sizeof(glm::vec2)+1, s );
    }
}
