#define DIAGNOSTIC
#include <engine.hpp>

class AI_Controller : public Behaviour {
	float speed = 10;
	Objekt* _ball;
public:
	void Update ( ) {
		// follow perfectly ball
		obj->Set_Pos ( 
			lerp ( 
				obj->Get_Pos ( ), 
				_ball->Get_Pos ( ), 
				speed * Timer::delta_time ) );
	}

	void Set ( Objekt* Ball ) { _ball = Ball; }
};

class Player_Controller : public Behaviour {
private:
	float speed = 10;
public:
	void Update ( ) {
		if ( Key_Pressed ( "W" ) ) { obj->Inc_Pos ( {0,speed * Timer::delta_time,0} ); }
		if ( Key_Pressed ( "S" ) ) { obj->Inc_Pos ( {0,-speed* Timer::delta_time,0} ); }	
	}
};

// classic pong game with local multi player
int main ( ) {
	// ReKat::phisiks::Start ( 60 );
	ReKat::grapik::Start ( "Pong", 800, 600 );
	// ReKat::synth::Start ( );

	Objekt Scene ( "scene" );
	Objekt Walls ( "walls" );
	Objekt Player1 ( "player1", {300,0,0}, {50,300,100} );
	Objekt Player2 ( "player2", {-300,0,0}, {50,300,100} );
	Objekt Ball ( "BALS" );

	Scene.Add_Child ( &Walls );
	Scene.Add_Child ( &Player1 );
	Scene.Add_Child ( &Player2 );
	Scene.Add_Child ( &Ball );

    Manager::Texture_Load ( "sprite", "Sprites.png" );
    // Manager::Font_Load ( "font", "Font.ttf",80 );
    Manager::Shader_Load ( "sprite", "sprite.vs", "sprite.fs" );
    Manager::Shader_Load ( "text", "text.vs", "text.fs" );
	Camera* cam = new Camera;
	DEBUG ( 3, "LOADED" );

	Scene.Add_Component ( cam );
	Player1.Add_Component < Sprite > ( )->Set ( "sprite", "sprite", cam, {2,1}, 1 ); // square sprite
	Player1.Add_Component < Box_Collider > ( );
	Player1.Add_Component < Player_Controller > ( );
	Player2.Add_Component < Sprite > ( )->Set ( "sprite", "sprite", cam, {2,1}, 1 );
	Player2.Add_Component < Box_Collider > ( );
	Player2.Add_Component < AI_Controller > ( );
	Ball.Add_Component < Sprite > ( )->Set ( "sprite", "sprite", cam, {2,1}, 0 ); // circle sprite
	Ball.Add_Component < Sfere_Collider > ( );


	Manager::Set_Active_Scene ( &Scene );

	while ( ReKat::grapik::IsEnd ( ) ) {
		glClearColor(0.0, 0.0, 0.0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		Timer::Update ( );
		Manager::Update ( );
		ReKat::grapik::Update ( );
	}
	

	Manager::Free_Video ( );
	// Manager::Free_Audio ( );
	
	ReKat::grapik::End ( );
	// ReKat::synth::End ( );
	return 0;
}