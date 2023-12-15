#include "Engine/load.h"
#include "Engine/controlls.h"
#include "Online/load.h"

int main(int argc, char const *argv[]) {
    ReKat::grapik::Start ( "Kat Kave", 800, 600, false, false, true );
    std::cout << "icon set: " << ReKat::grapik::SetIcon ( "favicon.png" ) << '\n';

    ReKat::grapik::Input::_Keyboard = &__Keyboard;
    ReKat::grapik::Input::_Mouse_pos = &__Mouse_pos;
    ReKat::grapik::Input::_Mouse_key = &__Mouse_key;
    ReKat::grapik::Input::_ScrollWell = &__ScrollWell;
    ReKat::grapik::Input::_FreamBufferResize = &__FreamBufferResize;
    ReKat::grapik::Input::_Caracters = &__Caracters;

    Load_resources ( );
    Create_Objs ( );
    Create_Scenes ( );

    Start_Online ( argc, argv );
    
    // connect to server
    if ( argc != 3 ) {
        Manager::Set_Active_Scene ( "loading" );
        Manager::Start ( );
        Manager::Update ( );
        online::Connect ( "server", "localhost" );
    }

    Manager::Set_Active_Scene ( "main" );
    Manager::Start ( );

    while ( ReKat::grapik::IsEnd ( ) ) {
        Send_positon ( );
        Manager::Update ( );
    }
    std::cout << "End\n";
    End_Online ( );
    
    return 0;
}
