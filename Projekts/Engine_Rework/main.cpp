// #define DIAGNOSTIC
#include <Engine_Rework/engine.hpp>

#include <stdlib.h> 
int main(int argc, char const *argv[]) {
	ReKat::grapik::Start ( "Anna", 1500, 800 );
	Synth *Audio_Drivers = new Synth;
	Audio_Drivers->_Start ( );

	// resource allocation
	int result = 0;
	result += Manager::Source_Load ( "carlo", {0,0,0} );
	result += Manager::Buffer_Load ( "carlo", "a.wav" );
	std::cout << "Resources allocate wit exit code: " << result << '\n';

	Objekt nw ("carlos");
	Objekt ne ("gatoe");
	nw.Add_Child(&ne);
	nw.Add_Component < Fps > ( );
	nw.Add_Component < Synth > ( Audio_Drivers );
	ne.Add_Component < Audio_Source > ( )->Set ( "carlo", "carlo");
	int iterativo = 5000;

	color ( "\n------------ START ------------\n\n", FOREGROUND_GREEN | FOREGROUND_INTENSITY );
	nw.Start();
	color ( "\n------------ ADDITIONAL PREP ------------\n\n", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY );

	
	ne.Get_Component < Audio_Source > ( )->Play ( );
	while ( ReKat::grapik::IsEnd ( ) ) { 
		// std::cout << std::endl;
		// color ( ( std::string("------------ UPDATE ") + std::to_string( iterativo ) + std::string (" ------------")).c_str(), 
		// FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN );
		// std::cout << std::endl;
		nw.Update ( );
		ReKat::grapik::Update ( );
	}

	
	Manager::Free_Audio ( );
	Manager::Free ( );
	ReKat::grapik::End ( );
	color ( "\n------------ END ------------\n\n", FOREGROUND_RED | FOREGROUND_INTENSITY );
	return 0;
}
