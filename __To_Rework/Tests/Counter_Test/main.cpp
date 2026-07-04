#define DIAGNOSTIC
#include <engine.hpp>

int counter = 164;

void inc ( ) {
	counter++;
	Manager::Objekt_Get ( "Text" )->Get_Component < Text > ( )->Set ( "scrivi: " + std::to_string ( counter ) );
}

int main ( ) {
	ReKat::grapik::Start ( "Input_Box Test", 800,600, false, false, true );

	int result = 0;
	result += Manager::Shader_Load ( "text" , "text.vs", "text.fs" ); // maybe use defauts
	result += Manager::Font_Load ( "text", "font.ttf", 90, 10 );

	auto text = Manager::Objekt_Load ( "Text" );
	auto display = text->Add_Component < Text > ( )->Set ( "text", "text" )->Set ( "Ciao" );

	text->Add_Component < Button > ( )->OnClick ( inc );

	Manager::Set_Active_Scene ( text );

	while ( ReKat::grapik::IsEnd ( ) ) {
		glClearColor(1.0, 0.0, 0.0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		if ( Key_Down (" ") ) { inc (); }
		Manager::Update ( );
		ReKat::grapik::Update ( );
	}

	Manager::Free ( );
	Manager::Free_Objekt ( "Text" );
	ReKat::grapik::Terminate ( );
}
