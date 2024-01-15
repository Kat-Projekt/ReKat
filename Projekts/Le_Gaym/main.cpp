#include "Engine/load.h"
#include "Engine/controlls.h"
#include "Online/load.h"

int main(int argc, char const *argv[]) {
    ReKat::grapik::Start ( "Kat Kave", 800, 600, false, false, true );
    std::cout << "icon set: " << ReKat::grapik::SetIcon ( "favicon.png" ) << '\n';;
	ReKat::synth::Start ( );

    ReKat::grapik::Input::_Keyboard = &__Keyboard;
    ReKat::grapik::Input::_Mouse_pos = &__Mouse_pos;
    ReKat::grapik::Input::_Mouse_key = &__Mouse_key;
    ReKat::grapik::Input::_ScrollWell = &__ScrollWell;
    ReKat::grapik::Input::_FreamBufferResize = &__FreamBufferResize;
    ReKat::grapik::Input::_Caracters = &__Caracters;

	int result = 0;
    result += Load_resources ( );
	result += Load_Audio ( );
	if ( result != 0 ) { return 0; }
    Create_Objs ( );
    Create_Scenes ( );

    Manager::Set_Active_Scene ( "main" );
	
	std::cout << "Calling Start\n";
    Manager::Start ( );
	std::cout << "Called Start\n";

    int Fps = 0;
	int t = time (0);

    while ( ReKat::grapik::IsEnd ( ) ) {
		Fps ++;
		if ( t + 1 == time(0) ) { 
			t = time (0); 
			std::cout << "\n-------------- Fps: " << Fps << '\n'; 
			Fps = 0; 
		}
        Send_positon ( );

        Manager::Update ( );

        ReKat::grapik::Pool ( );	
    }
    std::cout << "End\n";
    End_Online ( );
	ReKat::synth::End();
	ReKat::grapik::Terminate();

    return 0;
}