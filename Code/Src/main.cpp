#include "render.hpp"
#include "file_manager.hpp"

using namespace ReKat;

int main(int argc, char const *argv[]) {
    // login infos
    std::string name = "";
    std::string pass = "";
    int port = -1;
    switch ( argc ) {
    case 3: std::cout << "command defined name/pass\n"; name = argv[1]; pass = argv[2]; port = DPORT; break;
    case 4: std::cout << "command defined name/pass\n"; name = argv[1]; pass = argv[2]; port = std::stoi(argv[3]); break; }
    Grapik_Init ( );

    // info loop
    history.push_back ("");
    bool Infos_get = false;
    while ( grapik::IsEnd( ) && !Infos_get ) {
        // when a new command is added (\n pressed) get the last command as parameter        
        glClearColor(0.09412f, 0.5451f, 0.74118f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // command out;
        Manager::Text_Get("death_record")->RenderText ( output.str(), {25.0f,SCR_HEIGHT-75.0f}, { SCR_WIDTH-50.0f, 200.0f}, 1.0f, glm::vec4(0.21875f, 0, 0.21875f , 1));
        // command box
        Manager::Text_Get("death_record")->RenderText ( "$ " + history[current_command], {25.0f,100.0f},{SCR_WIDTH-50.0f,200.0f}, 0.75f, glm::vec4(0.3, 0.7f, 0.9f, 1));

        grapik::Pool();
        if ( name == "") {
            output.str("Login\nInsert name: "); if ( history.size() > 1 ) 
            { name = history [0]; }
        }
        else if ( pass == "" ) { 
            output.str("Login\nInsert password:"); if ( history.size() > 2 ) 
            { pass = history [1]; }
        }
        else if ( ! ( port == 0 || ( 1024 <= port && port <= 65535 )) ) {
            output.str("Node port\nInsert port: ( 0 for "+ std::to_string(DPORT) +" )"); if ( history.size() > 3 ) {
                // remover "$ "
                port = std::stoi ( history[2] );
                history.pop_back ();
                current_command--;
                Infos_get = true;
                if ( port == 0 ) { port = DPORT; }
            }
        } else {
            Infos_get = true;
        }
    }

    if ( Infos_get == false ) {
        std::cout << "cannot get infos\n";
        return online::FAILURE;
    }

    /*
    std::cout << "INFOS GET";
    std::cout << "\nname: " << name;
    std::cout << "\npass: " << pass;
    std::cout << "\nport: " << port << '\n';
    */
    output.str ("");
    history.clear ( );
    current_command = 0;

    Begin_string += name + '\n';
    output << Begin_string;

    // getting ID
    size_t ID;
    std::string path = setup ( name, pass, &ID );
    output << "path: " << path << "\nID: " << ID << '\n';

    history.push_back ( "" );
    
    // starting ONLINE
    long long start_result = online::Start ( name, 0, port ) << '\n';
    output << "start result: " << ( start_result == 0 ? "succes" : "failed: " + std::to_string ( start_result ) ) << '\n';

    // ONLINE threads
    std::thread conn_thread;
    conn_thread = std::thread ( Connections );

    std::thread recv_thread;
    recv_thread = std::thread ( Check_connections );

    // random bg color 
    srand ( (int)ID);
    BGR = 0.5f + ((float)(rand()%256))/512.0f, BGG = 0.5f + ((float)(rand()%256))/512.0f, BGB = 0.5f + ((float)(rand()%256))/512.0f;

    // render title
    spash_start = Get_Time();
    while ( grapik::IsEnd( ) && ( Render_splash ( ) != 1 )) { ReKat::grapik::Pool(); }
    // cross fade
    spash_start = Get_Time();
    while ( grapik::IsEnd( ) )  {
        Render();
        if ( Render_splash_fade ( ) == 1 ) 
        { break; }
        ReKat::grapik::Pool();
    }

    while ( !Main_shutdown && grapik::IsEnd( ) ) 
    { Render(); ReKat::grapik::Pool(); }
    Main_shutdown = true;
    grapik::Terminate ( );
    std::cout << "\n\n----------------------- ENDED GRAPIK ---------------------\n";

    dump_history (path);

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
