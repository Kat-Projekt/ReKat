#define DIAGNOSTIC
// #define EXPANCE
#include <engine.hpp>

float speed = 104 * 7;
float actual_speed = speed;

class Change_Skin : public Behaviour {
private:
	Sprite* _sprite;
	std::string _change_char = "C";
public:
	void Start ( ) 
	{ _sprite = obj->Get_Component < Sprite > ( ); }

	void Update ( ) {
		if ( Key_Down ( _change_char ) ) 
		{ Inc ( ); }
	}

	void Inc ( ) 
	{ _sprite->frame ++; }

	void Set ( Sprite* sprite ) 
	{ _sprite = sprite; }

	void Set ( std::string change_char ) 
	{ _change_char = change_char; }
};

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

		if ( ReKat::sygnal::GetSignal ( "SCORE" ).bdata == true ) 
		{ obj->Set_Pos ( { obj->Get_Pos().x,0,0 } ); }
	}
};

class Player_Controller : public Behaviour {
private:
public:
	void Update ( ) {
		if ( Key_Pressed ( "W" ) ) { obj->Inc_Pos ( {0,speed * Timer::delta_time,0} ); }
		if ( Key_Pressed ( "S" ) ) { obj->Inc_Pos ( {0,-speed* Timer::delta_time,0} ); }
		// std::cout << Timer::delta_time;

		// stop outof border:
		auto offset = obj->Get_Size ( ).y * 0.5f;
		if ( obj->Get_Pos ( ).y > 500 - offset ) 
		{ obj->Inc_Pos ( {0,-speed * Timer::delta_time,0} ); }
		
		if ( obj->Get_Pos ( ).y < - 500 + offset ) 
		{ obj->Inc_Pos ( {0,speed * Timer::delta_time,0} ); }

		if ( ReKat::sygnal::GetSignal ( "SCORE" ).bdata == true ) 
		{ obj->Set_Pos ( { obj->Get_Pos().x,0,0 } ); }
	}
};

class Ball_Controller : public Behaviour {
	Text *_punteggio = nullptr;
	int _larghezza_campo = 0;
	Audio_Source *_bip;
	int punti_plater1 = 0;
	int punti_plater2 = 0;
	bool started = false;
	double start_time = 0;

	void Start_Game ( ) {
		obj->Set_Pos ( {0,0,0} );
		if ( punti_plater1 < punti_plater2 ) {
			obj->Get_Component < Rigidbody > ( )->velocity = { speed * 0.7, speed * 0.4, 0 };
		} else {
			obj->Get_Component < Rigidbody > ( )->velocity = { -speed * 0.7, -speed * 0.4, 0 };
		}
		_bip->Play (1);
		start_time = Timer::current_time;
	}

	void Update_Punteggio ( ) {
		if ( punti_plater1 == 3 ) { _punteggio->Set ( "V Wins", Text::LEFT ); obj->Get_Component < Rigidbody > ( )->velocity = {0,0,0}; _bip->Play (2); return; }
		if ( punti_plater2 == 3 ) { _punteggio->Set ( "E 2 Wins", Text::RIGHT ); obj->Get_Component < Rigidbody > ( )->velocity = {0,0,0}; _bip->Play (2); return; }

		_punteggio->Set ( std::to_string ( punti_plater1 ) + std::string ( " - " ) + std::to_string ( punti_plater2 ), Text::CENTER );
		ReKat::sygnal::SetSignal ( "SCORE", {S_BOOL,true} );
	}

	void Update ( ) {
		// reset score signal
		ReKat::sygnal::SetSignal ( "SCORE", {S_BOOL,false} );
	
		auto rigi = obj->Get_Component < Rigidbody > ( );

		DEBUG (3, "signal ",ReKat::sygnal::GetSignal ( "START" ).bdata );
		if ( ReKat::sygnal::GetSignal ( "START" ).bdata == true && !started ) {
			rigi->velocity = { speed * 0.7, speed * 0.4, 0 };
			started = true;
		}
		auto offset = obj->Get_Size ( ).x;
		if ( obj->Get_Pos ( ).y > 500 - offset || obj->Get_Pos ( ).y < - 500 + offset ) { // map limits
			auto vel = rigi->velocity;
			rigi->velocity = { vel.x, -vel.y, 0 };
		}

		// segna giocatore 1
		if ( obj->Get_Pos ( ).x > _larghezza_campo ) { punti_plater1 ++; Start_Game ( ); Update_Punteggio ( ); }

		// segna giocatore 2
		if ( obj->Get_Pos ( ).x < - _larghezza_campo ) { punti_plater2 ++; Start_Game ( ); Update_Punteggio ( ); }

		if ( punti_plater1 == 3 || punti_plater2 == 3 ) { // uno dei due vince
			if ( Key_Down ( "T" ) ) {
				punti_plater1 = 0;
				punti_plater2 = 0;
				Start_Game ( );
			}
		}

	}

	public: 
	void Set ( Text *Punteggio, int largezza_campo, Audio_Source *bip ) {
		_punteggio = Punteggio;
		_larghezza_campo = largezza_campo / 2;
		_bip = bip;
	}

	
    void Collision ( Objekt* _obj ) {
		float speed_gain = 0.03;
		actual_speed = speed * ( 1 + speed_gain * ( Timer::current_time - start_time ) );
		auto an = obj->Get_Pos( ) - _obj->Get_Pos( );
		vec3 norm = normalize ( vec3{an.x, an.y*0.7,an.x} );
		// limit angle at 45 degree
		if ( abs(norm.y) > 0.7071067811865475 ) {
			norm.x = ( norm.x > 0 ? 0.7071067811865475 : -0.7071067811865475 );
			norm.y = ( norm.y > 0 ? 0.7071067811865475 : -0.7071067811865475 );
		}
		obj->Get_Component < Rigidbody > ( )->velocity = { norm * actual_speed };
		_bip->Play (0);
	}
};

class Start_Timer : public Behaviour {
private:
	Text* _text;
	float start_time;
public:
	void Start ( ) {
		start_time = Timer::Get_Time ( );
		_text = obj->Get_Component < Text > ( );
	}

	void Update ( ) {
		std::string count = "READY??";
		// count to 3
		float C_time = Timer::Get_Time ( );
		if ( C_time > start_time + 1 ) { count = "1"; }
		if ( C_time > start_time + 1.5 ) { count = "2"; }
		if ( C_time > start_time + 2 ) { count = "3"; }
		if ( C_time > start_time + 2.5 ) { count = "START!!"; ReKat::sygnal::SetSignal ( "START",{S_BOOL,1} ); }

		if ( C_time > start_time + 3 ) { return; }

		_text->Set ( count );
	}
};

// classic pong game with local multi player
int main ( ) {
	ReKat::phisiks::Start ( 120 );
	ReKat::grapik::Start ( "Pong", 800, 600,false,false,false );
	ReKat::synth::Start ( );

	int largezza_campo = 1300;
	int distanza_palette = 1200;

	auto PostProcessor = Manager::Objekt_Load ( "post", {0,0,0}, {1600,1000,10} );

	auto Scene = Manager::Objekt_Load ( "scene" );
	auto Punteggio = Manager::Objekt_Load ( "Punteggio", { 0,300,0 } );
	auto Player1 = Manager::Objekt_Load ( "player1", {distanza_palette/2,0,0}, {20,100,100} );
	auto Player2 = Manager::Objekt_Load ( "player2", {-distanza_palette/2,0,0}, {20,100,100} );
	auto Ball = Manager::Objekt_Load ( "BALS", {0,0,0}, {20,20,20} );

	Scene->Add_Child ( Punteggio );
	Scene->Add_Child ( Player1 );
	Scene->Add_Child ( Player2 );
	Scene->Add_Child ( Ball );

    Manager::Texture_Load ( "sprite", "Sprites.png" );
    Manager::Texture_Load ( "skin1", "skin1.png" );
    Manager::Texture_Load ( "skin2", "skin2.png" );
    Manager::Texture_Load ( "skin1p", "skin1p.png" );
    Manager::Font_Load ( "font", "Font.ttf",90 );
    Manager::Shader_Load ( "postprocesor", "framebuffers.vs", "framebuffers.fs" );
    Manager::Shader_Load ( "sprite", "sprite.vs", "sprite.fs" );
    Manager::Shader_Load ( "text", "text.vs", "text.fs" );
	Manager::Buffer_Load ( "bip", "bip.wav" );
	Manager::Buffer_Load ( "bup", "bup.wav" );
	Manager::Buffer_Load ( "badun", "badun.wav" );
	Manager::Source_Load ( "bip" );

	DEBUG ( 3, "LOADED" );

	PostProcessor->Add_Component < Framebuffer > ( )->Set(1600,900)->Set(Scene)->Set("postprocesor");

	Manager::Camera_Load ( "cam", Scene );
	auto bip = Scene->Add_Component < Audio_Source > ( )->Set ( "bip", "bip" )->Set("bup")->Set("badun");
	auto pun = Punteggio->Add_Component < Text > ( )->Set ( "font", "text", "cam", {0,1,1,1} );
	Punteggio->Add_Component < Start_Timer > ( );

	Scene->Add_Component < Fps > ( )->MAX_FPS = 120;

	Player1->Add_Component < Sprite > ( )->Set ( "skin1", "sprite", "cam", {2,1}, 1 ); // square sprite
	Player1->Add_Component < Box_Collider > ( )->Set_Size ( Player1->Get_Size ( ) );
	Player1->Add_Component < AI_Controller > ( );
	Player2->Add_Component < Sprite > ( )->Set ( "skin2", "sprite", "cam", {5,1}, 0 );
	Player2->Add_Component < Box_Collider > ( )->Set_Size ( Player2->Get_Size ( ) );
	Player2->Add_Component < Player_Controller > ( );
	Player2->Add_Component < Change_Skin > ( );

	Ball->Add_Component < Sprite > ( )->Set ( "skin1p", "sprite", "cam" ); // circle sprite
	Ball->Add_Component < Box_Collider > ( )->Set_Size ( Ball->Get_Size().x );
	Ball->Add_Component < Rigidbody > ( )->time_scale = 1;
	Ball->Add_Component < Ball_Controller > ( )->Set ( pun, largezza_campo, bip );


	Manager::Set_Active_Scene ( "scene" );
	ReKat::phisiks::Set_Active ( "scene" );

	Manager::Check_Resource_Integrity ( );

	Manager::Start ( );

	// setting for the first time the score signal
	ReKat::sygnal::SetSignal ( "SCORE", {S_BOOL,false} );


	while ( ReKat::grapik::IsEnd ( ) ) {
		glClearColor(0.0, 0.0, 0.0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		Manager::Update ( );
		Manager::Get_Active_Scene ( )->Print_Tree ( "" );
		ReKat::grapik::Update ( );
		ReKat::phisiks::Update ( );
		DEBUG ( 4, "Rendered Frame" );
	}

	// Manager::Free ( );

	ReKat::grapik::End ( );
	ReKat::synth::End ( );
	return 0;
}
