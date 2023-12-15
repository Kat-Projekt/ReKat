#include "../global.h"
#include "connection.h"
using namespace ReKat;

std::thread conn_thread;
std::thread recv_thread;

int Start_Online ( int argc, char const *argv[] ) {
    // login infos
    std::string name = "";
    std::string pass = "";
    int port = DPORT;
    switch ( argc ) {
    case 3: std::cout << "command defined name/pass\n"; name = argv[1]; pass = argv[2]; port = DPORT; std::cout << "starting as server\n"; break;
    case 4: std::cout << "command defined name/pass\n"; name = argv[1]; pass = argv[2]; port = std::stoi(argv[3]); break; }

    Manager::Set_Active_Scene ( "info" );
    Manager::Start ( );

    // info loop
    bool Infos_get = false;
    while ( grapik::IsEnd( ) && !Infos_get ) {
        // setting output

        Manager::Update ( );

        if ( name == "") {
            Manager::UI_Object_Get("output")->Set_text("Login\nInsert name: ");
            if ( grapik::Input::Key_Down("Enter") ) { 
                name = Manager::UI_Object_Get("input")->Get_text();
                Manager::UI_Object_Get("input")->Set_text(""); 
            }
        }
        else if ( pass == "" ) {
            Manager::UI_Object_Get("output")->Set_text("Login\nInsert password:");
            if ( grapik::Input::Key_Down("Enter") ) { 
                pass = Manager::UI_Object_Get("input")->Get_text();
                Manager::UI_Object_Get("input")->Set_text(""); 
            }
        } else {
            Infos_get = true;
        }
    }

    if ( Infos_get == false ) {
        std::cout << "cannot get infos\n";
        return online::FAILURE;
    }

    // starting ONLINE
    std::cout << (online::Start ( name, 0, port ) == 0 ? "SUCCES\n" : "FAILED\n");

    // ONLINE threads
    conn_thread = std::thread ( Connections );
    recv_thread = std::thread ( Check_connections );

    return 0;
}

int End_Online ( ) {
    Main_Shutdown = true;
    // terminate threads
    if ( recv_thread.joinable() ) { recv_thread.join ( ); }
    std::cout << "----------------------- ENDED RECIVE ---------------------\n";

    if ( conn_thread.joinable() ) {
        TerminateThread ((HANDLE)conn_thread.native_handle(),1);
        conn_thread.detach ( );
    }
    std::cout << "--------------------------- END --------------------------\n";
    return 0;
}
