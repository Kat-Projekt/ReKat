// #define DIAGNOSTIC
// #define EXPANCE
#include <engine.hpp>

float speed = 104 * 7;
float actual_speed = speed;

enum STATE {
	START,
	PLAYING,
	ENDED,
	CREDITS
};

STATE current_state = START;

using namespace ReKat::grapik::Input;

class Splash_Screen : public Behaviour {
	Text* titolo;
	Text* sottot;
public:
	void Start ( ) {
		obj->Set_Pos ( {0,60,0} );
		titolo = obj->Add_Component < Text > ( );
		titolo->Set ( "Pong", Text::CENTER );
		titolo->Set ( "font", "text" );

		auto Sottot = Manager::Objekt_Load ( "sotto titolo", {0,-80,0}, {30,30,100} );
		sottot = Sottot->Add_Component < Text > ( );
		sottot->Set ( "made with 4RealEngine5", Text::CENTER );
		sottot->Set ( "font", "text" );

		obj->Add_Child ( Sottot );

		// animation
		// 3 secondi Titolo
		// 2 secondi fade
		auto titolo_a = new Animation ( titolo->Expose_Color ( ), ONCE );
		auto sottot_a = new Animation ( sottot->Expose_Color ( ), ONCE );
		titolo_a->Add_Frame ( {1,1,1,1}, {1,1,1,1}, 3 );
		sottot_a->Add_Frame ( {1,1,1,1}, {1,1,1,1}, 3 );
		titolo_a->Add_Frame ( {1,1,1,1}, {1,1,1,0}, 1 );
		sottot_a->Add_Frame ( {1,1,1,1}, {1,1,1,0}, 1 );
		DEBUG ( 3, "animations: ", titolo_a, " ", sottot_a );
		Manager::Animation_Load ( "titolo", titolo_a );
		Manager::Animation_Load ( "sottot", sottot_a );
		obj->Add_Component < Animator > ( )
		->New_Node ( "splash" )
		->Change_Animation ( "splash" )
		->Add_Animation ( "splash", titolo_a )
		->Add_Animation ( "splash", sottot_a );
	}

	void Update ( ) {
		if ( Timer::current_time > 4.5 ) {
			Manager::Set_Active_Scene ( "scene" );
			ReKat::phisiks::Set_Active ( "scene" );
		} 
	}
};

enum Player {
	PLAYER1,
	PLAYER2
};

class Controller : public Behaviour {
public:
	Player P;

	void Start ( ) {
		obj->Add_Component < Box_Collider > ( )->Set_Size ( obj->Get_Size ( ) );
		obj->Add_Component < Sprite > ( )->Set ( "sprite", "sprite", "cam" );
	}

	void Update ( ) {
		switch ( P ) {
		case PLAYER1:
			if ( Key_Pressed ( "I" ) ) { obj->Inc_Pos ( {0,speed * Timer::delta_time,0} ); }
			if ( Key_Pressed ( "K" ) ) { obj->Inc_Pos ( {0,-speed* Timer::delta_time,0} ); }
		break;
		
		case PLAYER2:
			if ( Key_Pressed ( "W" ) ) { obj->Inc_Pos ( {0,speed * Timer::delta_time,0} ); }
			if ( Key_Pressed ( "S" ) ) { obj->Inc_Pos ( {0,-speed* Timer::delta_time,0} ); }
		break;
		}

		// stop out of border:
		const int edge = 490;

		auto offset = obj->Get_Size ( ).y * 0.5f;
		if ( obj->Get_Pos ( ).y > edge - offset ) 
		{ obj->Set_Pos ( {obj->Get_Pos ( ).x, edge - offset, 0} ); }
		
		if ( obj->Get_Pos ( ).y < - edge + offset ) 
		{ obj->Set_Pos ( {obj->Get_Pos ( ).x, -edge + offset, 0} ); }

	}
};

class Points : public Behaviour {
	Text* _points1;
	Text* _points2;
	Text* info;

	float start_time;

	unsigned int p1 = 0;
	unsigned int p2 = 0;

	unsigned int Win = 11;

	void Start ( ) {
		auto __info = Manager::Objekt_Load ( "INFOS" );
		info = __info->Add_Component < Text > ( )->Set ( "font", "text" );
		obj->Add_Child ( __info );	
	}

	void Update ( ) {
		_points1->Set ( std::to_string( p1 ) );
		_points2->Set ( std::to_string( p2 ) );

		switch ( current_state ) {
		case START:
			info->Set ( "Press Space to Start\nTo Move use:\nPlayer 1: W & S\nPlayer 2: I & K" )->Set_Active ( true );
			break;
		case PLAYING:
			info->Set_Active ( false );
			break;
		case ENDED:
			info->Set ( std::string ( p1>p2 ? "Player 1 wins" : "Player 2 wins" ) 
						+ "\nwith time: " 
						+ std::to_string ( Timer::current_time -  start_time )
						+ std::string("\nPress Space to remach") )->Set_Active ( true );
			current_state = CREDITS;
			break;
		}
	}

public:
	Points * Set ( Text* point1, Text* point2 ) 
	{ _points1 = point1; _points2 = point2; return this; }

	void Reset ( ) { p1 = 0; p2 = 0; start_time = Timer::current_time; }

	void Score ( Player P ) {
		switch ( P ) {
			case PLAYER1: p1++; break;
			case PLAYER2: p2++; break;
			default: break;
		}
		if ( Won ( ) ) 
		{ current_state = ENDED; }
	}

	inline bool Won ( ) { return ( p1 >= Win || p2 >= Win ) && ( p1 >= p2 + 2 || p2 >= p1 + 2); }
};

class Ball_Controller : public Behaviour {
	Points * _points;
	Objekt * Pl1, * Pl2;
	int starts = 0; // 2 for player 2; 0 for player 1

	float _larghezza_campo = 0;
	Audio_Source *_bip;
	
	bool started = false;
	double start_time = 0;

	void Start ( ) {
		obj->Add_Component < Rigidbody > ( );
		obj->Add_Component < Box_Collider > ( )->Set_Size ( obj->Get_Size ( ) );
		obj->Add_Component < Sprite > ( )->Set ( "sprite", "sprite", "cam" )->Set_Active ( false );

		srand ( time(0) );
		if ( rand ( ) % 2 ) 
		{ starts = 2; }
	}

	void Begin ( ) {
		switch ( rand ( ) % 2 + starts ) {
		case 0:
			obj->Set_Pos ( { - _larghezza_campo / 2, 400, 0 } );
			obj->Get_Component < Rigidbody > ( )->velocity = speed * vec3 { 0.6, -0.4, 0 };
		break;
		case 1:
			obj->Set_Pos ( { - _larghezza_campo / 2, - 400, 0 } );
			obj->Get_Component < Rigidbody > ( )->velocity = speed * vec3 { 0.6, 0.4, 0 };
		break;
		case 2:
			obj->Set_Pos ( { _larghezza_campo / 2, 400, 0 } );
			obj->Get_Component < Rigidbody > ( )->velocity = speed * vec3 { -0.6, -0.4, 0 };
		break;
		case 3:
			obj->Set_Pos ( { _larghezza_campo / 2, - 400, 0 } );
			obj->Get_Component < Rigidbody > ( )->velocity = speed * vec3 { -0.6, 0.4, 0 };
		break;
		}
		start_time = Timer::current_time;
		Pl1->Set_Active (true);
		Pl2->Set_Active (true);
	}

	void Update ( ) {
		// segna giocatore 1
		if ( obj->Get_Pos ( ).x > _larghezza_campo ) { _points->Score ( PLAYER1 ); Begin ( ); }
		// segna giocatore 2
		if ( obj->Get_Pos ( ).x < - _larghezza_campo ) { _points->Score ( PLAYER2 ); Begin ( ); }

		// reset score signal
		ReKat::sygnal::SetSignal ( "SCORE", {S_BOOL,false} );
	
		auto rigi = obj->Get_Component < Rigidbody > ( );

		// top bound
		auto offset = obj->Get_Size ( ).x - 2;
		if ( obj->Get_Pos ( ).y > 500 - offset || obj->Get_Pos ( ).y < - 500 + offset ) { // map limits
			auto vel = rigi->velocity;
			auto pos = obj->Get_Pos ( );
			obj->Set_Pos ( {pos.x, sign ( vel.y ) * ( 499 - offset ), 0 } );
			rigi->velocity = { vel.x, -vel.y, 0 };
		}

		if ( _points->Won ( ) || !started ) {
			Pl1->Set_Active (false);
			Pl2->Set_Active (false);

			// side bounds
			if ( obj->Get_Pos ( ).x > _larghezza_campo - offset || obj->Get_Pos ( ).x < - _larghezza_campo + offset ) { // map limits
				auto vel = rigi->velocity;
				auto pos = obj->Get_Pos ( );
				obj->Set_Pos ( {sign ( vel.x ) * ( _larghezza_campo - 1 - offset ), pos.y, 0 } );
				rigi->velocity = { -vel.x, vel.y, 0 };
			}
			
			if ( Key_Down ( " " ) ) {
				Begin ( );
				_points->Reset ( );
				started = true;
				current_state = PLAYING;
				obj->Get_Component < Sprite > ( )->Set_Active ( true );
			}
		}
	}

	public: 
	Ball_Controller * Set ( int largezza_campo, Audio_Source *bip, Points * points ) {
		_larghezza_campo = largezza_campo / 2;
		_bip = bip;
		_points = points;
		return this;
	}

	Ball_Controller * Set ( Objekt* _Pl1, Objekt* _Pl2 ) 
	{ Pl1 = _Pl1; Pl2 = _Pl2; return this; }

    void Collision ( Objekt* _obj ) {
		float speed_gain = 0.01;
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

// classic pong game with local multi player
int main ( ) {
	ReKat::phisiks::Start ( 120 );
	ReKat::grapik::Start ( "Pong", 800, 600,false,false,true );
	ReKat::grapik::SetIcon ( "Data/favicon.png" );
	ReKat::synth::Start ( );

	int largezza_campo = 1300;
	int distanza_palette = 1200;

	auto Splash		= Manager::Objekt_Load ( "splash" );

	Splash->Add_Component < Splash_Screen > ( );

	auto Scene		= Manager::Objekt_Load ( "scene" );
	auto Punteggio1	= Manager::Objekt_Load ( "Punteggio2", { -200,400,0 } );
	auto Punteggio2	= Manager::Objekt_Load ( "Punteggio2", {  200,400,0 } );
	auto Player1	= Manager::Objekt_Load ( "player1", {distanza_palette/2,0,0}, {10,100,100} );
	auto Player2	= Manager::Objekt_Load ( "player2", {-distanza_palette/2,0,0}, {10,100,100} );
	auto Ball		= Manager::Objekt_Load ( "BALS", {0,0,0}, {15,15,15} );

	auto Edge1		= Manager::Objekt_Load ( "EdgeT", {0,496,0}, {largezza_campo,2,15} );
	auto Edge2		= Manager::Objekt_Load ( "EdgeB", {0,-496,0}, {largezza_campo,2,15} );

	Edge1->Add_Component < Sprite > ( )->Set ( "sprite", "sprite", "cam" );
	Edge2->Add_Component < Sprite > ( )->Set ( "sprite", "sprite", "cam" );

	int dots = 40;
	for ( int y = 0; y < dots / 2; y++ ) {
		auto O = Manager::Objekt_Load ( "dot" + std::to_string ( y ), { 0, -(1000/dots) * y, 0 }, ( 100.0f / dots ) * vec3{ 1,4,1 } );
		O->Add_Component < Sprite > ( )->Set ( "sprite", "sprite", "cam" );
		Scene->Add_Child ( O );
		
		auto B = Manager::Objekt_Load ( "dot" + std::to_string ( y ), { 0, (1000/dots) * y, 0 }, ( 100.0f / dots ) * vec3{ 1,4,1 } );
		B->Add_Component < Sprite > ( )->Set ( "sprite", "sprite", "cam" );
		Scene->Add_Child ( B );
	}

	Scene->Add_Child ( Ball );

	Scene->Add_Child ( Punteggio1 );
	Scene->Add_Child ( Punteggio2 );
	Scene->Add_Child ( Player1 );
	Scene->Add_Child ( Player2 );
	Scene->Add_Child ( Edge1 );
	Scene->Add_Child ( Edge2 );

    Manager::Texture_Load ( "sprite", "Data/empty.png" );

    Manager::Font_Load ( "font", "Data/Font.ttf", 90, 13 );

    Manager::Shader_Load ( "sprite", "Data/sprite.vs", "Data/sprite.fs" );
    Manager::Shader_Load ( "text", "Data/text.vs", "Data/text.fs" );

	Manager::Source_Load ( "bip" );

	Manager::Buffer_Load ( "bip", "Data/bip.wav" );
	Manager::Buffer_Load ( "bup", "Data/bup.wav" );
	Manager::Buffer_Load ( "badun", "Data/badun.wav" );

	Manager::Camera_Load ( "cam", Scene );

	Player1->Add_Component < Controller > ( )->P = PLAYER1;
	Player2->Add_Component < Controller > ( )->P = PLAYER2;

	Ball->Add_Component < Ball_Controller > ( )->Set ( largezza_campo, 
		Scene->Add_Component < Audio_Source > ( )->Set ( "bip", "bip" )->Set ( "bup" )->Set ( "badun" ),
		Scene->Add_Component < Points > ( )->Set ( 
			Punteggio1->Add_Component < Text > ( )->Set ( "font", "text", "cam", {1,1,1,1} ), 
			Punteggio2->Add_Component < Text > ( )->Set ( "font", "text", "cam", {1,1,1,1} )
		)
	)->Set ( Player1, Player2 );

	Manager::Set_Active_Scene ( "splash" );
	ReKat::phisiks::Set_Active ( "splash" );

	while ( ReKat::grapik::IsEnd ( ) ) {
		glClearColor(0.0, 0.0, 0.0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		Manager::Update ( );
		ReKat::grapik::Update ( );
		ReKat::phisiks::Update ( );
	}

	Manager::Free ( );
	ReKat::grapik::End ( );
	ReKat::synth::End ( );
	return 0;
}
