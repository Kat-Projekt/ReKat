#define DIAGNOSTIC
#include <engine.hpp>

float speed = 104 * 7;

class AI_Controller : public Behaviour {
public:
	void Update ( ) {
		// follow perfectly ball
		if ( Key_Pressed ( "I" ) ) { obj->Inc_Pos ( {0,speed * Timer::delta_time,0} ); }
		if ( Key_Pressed ( "K" ) ) { obj->Inc_Pos ( {0,-speed* Timer::delta_time,0} ); }

		// stop outof border:
		auto offset = obj->Get_Size ( ).y * 0.5f;
		if ( obj->Get_Pos ( ).y > 500 - offset ) 
		{ obj->Inc_Pos ( {0,-speed * Timer::delta_time,0} ); }
		
		if ( obj->Get_Pos ( ).y < - 500 + offset ) 
		{ obj->Inc_Pos ( {0,speed * Timer::delta_time,0} ); }
	}
};

class Player_Controller : public Behaviour {
private:
public:
	void Update ( ) {
		if ( Key_Pressed ( "W" ) ) { obj->Inc_Pos ( {0,speed * Timer::delta_time,0} ); }
		if ( Key_Pressed ( "S" ) ) { obj->Inc_Pos ( {0,-speed* Timer::delta_time,0} ); }

		// stop outof border:
		auto offset = obj->Get_Size ( ).y * 0.5f;
		if ( obj->Get_Pos ( ).y > 500 - offset ) 
		{ obj->Inc_Pos ( {0,-speed * Timer::delta_time,0} ); }
		
		if ( obj->Get_Pos ( ).y < - 500 + offset ) 
		{ obj->Inc_Pos ( {0,speed * Timer::delta_time,0} ); }
	}
};

class Ball_Controller : public Behaviour {
	Text *_punteggio = nullptr;
	int _larghezza_campo = 0;
	Audio_Source *_bip;
	int punti_plater1 = 0;
	int punti_plater2 = 0;

	void Start_Game ( ) {
		obj->Set_Pos ( {0,0,0} );
		if ( punti_plater1 > punti_plater2 ) {
			obj->Get_Component < Rigidbody > ( )->velocity = { speed * 0.7, speed * 0.4, 0 };
		} else {
			obj->Get_Component < Rigidbody > ( )->velocity = { -speed * 0.7, -speed * 0.4, 0 };
		}
		_bip->Play (1);
	}

	void Update_Punteggio ( ) {
		if ( punti_plater1 == 3 ) { _punteggio->Set ( "Player 1 Wins", Text::LEFT ); obj->Get_Component < Rigidbody > ( )->velocity = {0,0,0}; _bip->Play (2); return; }
		if ( punti_plater2 == 3 ) { _punteggio->Set ( "Player 2 Wins", Text::RIGHT ); obj->Get_Component < Rigidbody > ( )->velocity = {0,0,0}; _bip->Play (2); return; }

		_punteggio->Set ( std::to_string ( punti_plater1 ) + std::string ( " - " ) + std::to_string ( punti_plater2 ), Text::CENTER );
	}

	void Update ( ) {
		auto rigi = obj->Get_Component < Rigidbody > ( );

		if ( Key_Down ( "T" ) ) { rigi->velocity = { speed * 0.7, speed * 0.4, 0 }; }
		auto offset = obj->Get_Size ( ).x;
		if ( obj->Get_Pos ( ).y > 500 - offset || obj->Get_Pos ( ).y < - 500 + offset ) { // map limits
			auto vel = rigi->velocity;
			rigi->velocity = { vel.x, -vel.y, 0 };
		}

		// segna giocatore 1
		if ( obj->Get_Pos ( ).x > _larghezza_campo ) { punti_plater1 ++; Start_Game ( ); }

		// segna giocatore 2
		if ( obj->Get_Pos ( ).x < - _larghezza_campo ) { punti_plater2 ++; Start_Game ( ); }

		// keeping x velocity constant and growing
		float speed_gain = 0.01;
		auto vel = rigi->velocity;
		if ( vel.x > 0 ) { vel.x = speed * ( 0.7 + speed_gain * Timer::current_time ); }
		else if ( vel.x < 0 ) { vel.x = - speed * ( 0.7 + speed_gain * Timer::current_time ); }
		rigi->velocity = vel;

		Update_Punteggio ( );
	}

	public: 
	void Set ( Text *Punteggio, int largezza_campo, Audio_Source *bip ) {
		_punteggio = Punteggio;
		_larghezza_campo = largezza_campo / 2;
		_bip = bip;
	}

	
    void Collision ( Objekt* _obj ) { _bip->Play (0); }
};

// classic pong game with local multi player
int main ( ) {
	ReKat::phisiks::Start ( 120 );
	ReKat::grapik::Start ( "Pong", 800, 600 );
	ReKat::synth::Start ( );

	int largezza_campo = 1000;
	int distanza_palette = 800;

	Objekt Scene ( "scene" );
	Objekt Punteggio ( "Punteggio", { 0,300,0 } );
	Objekt Player1 ( "player1", {distanza_palette/2,0,0}, {50,300,100} );
	Objekt Player2 ( "player2", {-distanza_palette/2,0,0}, {50,300,100} );
	Objekt Ball ( "BALS" );

	Scene.Add_Child ( &Punteggio );
	Scene.Add_Child ( &Player1 );
	Scene.Add_Child ( &Player2 );
	Scene.Add_Child ( &Ball );

    Manager::Texture_Load ( "sprite", "Sprites.png" );
    Manager::Font_Load ( "font", "Font.ttf",80 );
    Manager::Shader_Load ( "sprite", "sprite.vs", "sprite.fs" );
    Manager::Shader_Load ( "text", "text.vs", "text.fs" );
	Manager::Buffer_Load ( "bip", "bip.wav" );
	Manager::Buffer_Load ( "bup", "bup.wav" );
	Manager::Buffer_Load ( "badun", "badun.wav" );
	Manager::Source_Load ( "bip" );
	Camera* cam = new Camera;
	DEBUG ( 3, "LOADED" );

	Scene.Add_Component ( cam );
	auto bip = Scene.Add_Component < Audio_Source > ( )->Set ( "bip", "bip" )->Set("bup")->Set("badun");
	auto pun = Punteggio.Add_Component < Text > ( )->Set ( "font", "text", cam, {1,1,1,1} );

	Player1.Add_Component < Sprite > ( )->Set ( "sprite", "sprite", cam, {2,1}, 1 ); // square sprite
	Player1.Add_Component < Box_Collider > ( )->Set_Size ( Player1.Get_Size ( ) );
	Player1.Add_Component < AI_Controller > ( );
	Player2.Add_Component < Sprite > ( )->Set ( "sprite", "sprite", cam, {2,1}, 1 );
	Player2.Add_Component < Box_Collider > ( )->Set_Size ( Player2.Get_Size ( ) );
	Player2.Add_Component < Player_Controller > ( );

	Ball.Add_Component < Sprite > ( )->Set ( "sprite", "sprite", cam, {2,1}, 0 ); // circle sprite
	Ball.Add_Component < Box_Collider > ( )->Set_Size ( Ball.Get_Size().x );
	Ball.Add_Component < Rigidbody > ( );
	Ball.Add_Component < Ball_Controller > ( )->Set ( pun, largezza_campo, bip );

	Manager::Set_Active_Scene ( &Scene );

	while ( ReKat::grapik::IsEnd ( ) ) {
		glClearColor(0.0, 0.0, 0.0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		Timer::Update ( );
		Manager::Update ( );
		ReKat::grapik::Update ( );
		ReKat::phisiks::Update ( );
	}
	

	Manager::Free_Video ( );
	Manager::Free_Audio ( );
	
	ReKat::grapik::End ( );
	ReKat::synth::End ( );
	return 0;
}