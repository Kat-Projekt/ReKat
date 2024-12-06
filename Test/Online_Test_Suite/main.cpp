#define DIAGNOSTIC
#include <thread>
#include <string>
#include <online/components.hpp>

int main ( int argc, char const *argv[] ) {
    // get credentials
    std::string name = "";
    std::string pass = "";
    int port = -1;
    switch ( argc ) {
    case 3: DEBUG ( 4, "command defined name/pass\n" ); name = argv[1]; pass = argv[2]; port = DPORT; break;
    case 4: DEBUG ( 4, "command defined name/pass\n" ); name = argv[1]; pass = argv[2]; port = std::stoi(argv[3]); break; }
    if ( name == "" ) { DEBUG ( 2, "no username or password" ); }

    // start server
    auto ID = generate_id ( name, pass );
    auto R = ReKat::online::Start ( name, ID, port );

    // setup threads
    std::thread connection_thread;
    std::thread recive_thread;

    // wait for commands
    while ( true ) {
        /* code */
    }
    
    // terminate connections
    ReKat::online::End ();
}