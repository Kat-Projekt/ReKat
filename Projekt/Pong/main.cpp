// #define DIAGNOSTIC
// #define EXPANCE
#include <engine.hpp>

float speed = 104 * 7;
float actual_speed = speed;

enum STATE {
	START,
	PLAYING,
	PAUSED,
	ENDED,
	CREDITS
};

enum GAME_MODE {
	NORMAL,
	HOKEY,
	SOLO
};

STATE current_state = START;
GAME_MODE mode = HOKEY;

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
			Manager::Set_Active_Scene ( "post processor" );
			ReKat::phisiks::Set_Active ( "scene0" );
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
		if ( mode == SOLO && P == PLAYER2 ) { obj->Set_Active ( false ); return; }
		
		obj->Add_Component < Box_Collider > ( )->Set_Size ( obj->Get_Size ( ) );
		obj->Add_Component < Sprite > ( )->Set ( "sprite", "sprite", "cam" );
		
		float dist = obj->Get_Pos ( ).y;
		obj->Set_Pos ( {obj->Get_Pos().x,0,0} );

		if ( mode == HOKEY ) {
			auto second = Manager::Objekt_Load ( "second paddle", 
			{ - obj->Get_Pos ( ).x - sign (obj->Get_Pos ( ).x) * dist , 0, 0 }, obj->Get_Size ( ) );

			second->Add_Component < Box_Collider > ( )->Set_Size ( obj->Get_Size ( ) );
			second->Add_Component < Sprite > ( )->Set ( "sprite", "sprite", "cam" );

			obj->Add_Child ( second );
		}
	}

	void Update ( ) {
		if ( current_state == PAUSED ) { return; }

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

std::string to_time ( int frame ) {
	std::string str = "";
	int h = 0, m = 0, s = 0;
	s = frame % 60;
	m = frame / 60;
	h = frame / 3600;

	if ( h != 0 ) {
		str += std::to_string ( h ) + "h";
		if ( m != 0 || s != 0 ) 
		{ str += ":"; }
	}
	if ( m != 0 ) {
		str += std::to_string ( m ) + "m";
		if ( s != 0 ) 
		{ str += ":"; }
	}
	if ( s != 0 ) 
	{ str += std::to_string ( s ) + "s"; }

	if ( str == "" ) { str == "so friking fast"; }

	return str;
}

class Points : public Behaviour {
	Text* _points1;
	Text* _points2;
	Text* info;

	float start_time;
	float pause_time = 0;

	unsigned int p1 = 0;
	unsigned int p2 = 0;

	unsigned int Win = 11;

	void Start ( ) {
		auto __info = Manager::Objekt_Load ( "INFOS" );
		info = __info->Add_Component < Text > ( )->Set ( "font", "text", "cam" );
		obj->Add_Child ( __info );	
	}

	void Update ( ) {
		if ( current_state == PAUSED ) { pause_time += Timer::delta_time; }
		_points1->Set ( std::to_string( p1 ) );

		if ( mode == SOLO ) { _points2->Set ( "" ); }
		else { _points2->Set ( std::to_string( p2 ) ); }

		std::string msg;
		switch ( current_state ) {
		case START:
			switch ( mode ) {
				case SOLO: msg = "SOLO\n( I & K )\n( Space )"; break;
				case NORMAL: msg = "CLASSIC\n( W & S ) or ( I & K )\n( Space )"; break;
				case HOKEY: msg = "HOKEY\n( W & S ) or ( I & K )\n( Space )"; break;
			}
			info->Set ( msg )->Set_Active ( true );
			break;
		case PLAYING:
			info->Set_Active ( false );
			break;
		case ENDED:
			info->Set ( std::string ( p1>p2 ? "Player 1 won in:\n" : "Player 2 won in:\n" )
						+ to_time ( (int)( Timer::current_time -  start_time - pause_time ) )
						+ std::string("\n( Space )") )->Set_Active ( true );
			if ( mode == SOLO ) 
			{ info->Set ( std::string ( "you lasted for:\n" ) + to_time ( (int)( Timer::current_time -  start_time ) ) ); }
			current_state = CREDITS;
			break;
		}
	}

public:
	Points * Set ( Text* point1, Text* point2 ) 
	{ _points1 = point1; _points2 = point2; return this; }

	void Reset ( ) { p1 = 0; p2 = 0; start_time = Timer::current_time; pause_time = 0; }

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
		auto rigi = obj->Get_Component < Rigidbody > ( );
		
		if ( current_state == PAUSED ) { rigi->time_scale = 0; } else { rigi->time_scale = 1; }

		// solo mode
		if ( mode == SOLO && obj->Get_Pos ( ).x < - _larghezza_campo + 10 ) {
			auto vel = rigi->velocity;
			auto pos = obj->Get_Pos ( );
			obj->Set_Pos ( { -_larghezza_campo + 10, pos.y, 0 } );
			rigi->velocity = { -vel.x, vel.y, 0 };
			return;
		}
		// segna giocatore 1
		if ( obj->Get_Pos ( ).x > _larghezza_campo ) { _points->Score ( PLAYER1 ); Begin ( ); }
		// segna giocatore 2
		if ( obj->Get_Pos ( ).x < - _larghezza_campo ) { _points->Score ( PLAYER2 ); Begin ( ); }

		// reset score signal
		ReKat::sygnal::SetSignal ( "SCORE", {S_BOOL,false} );

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

void __FreamBufferResize (GLFWwindow* window, int width, int heigth ) {
	Manager::Objekt_Get ( "post processor" )->Get_Component < Framebuffer > ( )->Set ( width, heigth );
}

class Inizilize_Scene : public Behaviour {

	int largezza_campo = 1300;
	int distanza_palette = 1200;
	int distanza_hokey = 350;

public:
	Framebuffer * post;

	void Start ( ) {
		auto Punteggio1	= new Objekt ( "Punteggio2", { -200,400,0 } );
		auto Punteggio2	= new Objekt ( "Punteggio2", {  200,400,0 } );
		auto Player1	= new Objekt ( "player1", {distanza_palette/2,distanza_hokey,0}, {10,100,100} );
		auto Player2	= new Objekt ( "player2", {-distanza_palette/2,distanza_hokey,0}, {10,100,100} );
		auto Ball		= new Objekt ( "BALS", {0,0,0}, {15,15,15} );

		auto Edge1		= new Objekt ( "EdgeT", {0,496,0}, {largezza_campo,2,15} );
		auto Edge2		= new Objekt ( "EdgeB", {0,-496,0}, {largezza_campo,2,15} );

		Edge1->Add_Component < Sprite > ( )->Set ( "sprite", "sprite", "cam" );
		Edge2->Add_Component < Sprite > ( )->Set ( "sprite", "sprite", "cam" );

		int dots = 40;
		for ( int y = 0; y < dots / 2; y++ ) {
			auto O = new Objekt ( "dot" + std::to_string ( y ), { 0, -(1000/dots) * y, 0 }, ( 100.0f / dots ) * vec3{ 1,4,1 } );
			O->Add_Component < Sprite > ( )->Set ( "sprite", "sprite", "cam" );
			obj->Add_Child ( O );
			
			auto B = new Objekt ( "dot" + std::to_string ( y ), { 0, (1000/dots) * y, 0 }, ( 100.0f / dots ) * vec3{ 1,4,1 } );
			B->Add_Component < Sprite > ( )->Set ( "sprite", "sprite", "cam" );
			obj->Add_Child ( B );
		}

		obj->Add_Child ( Ball );
		obj->Add_Child ( Punteggio1 );
		obj->Add_Child ( Punteggio2 );
		obj->Add_Child ( Player1 );
		obj->Add_Child ( Player2 );
		obj->Add_Child ( Edge1 );
		obj->Add_Child ( Edge2 );

		Manager::Camera_Load ( "cam", obj, post );
		 
		Player1->Add_Component < Controller > ( )->P = PLAYER1;
		Player2->Add_Component < Controller > ( )->P = PLAYER2;

		Ball->Add_Component < Ball_Controller > ( )->Set ( largezza_campo, 
			obj->Add_Component < Audio_Source > ( )->Set ( "bip", "bip" )->Set ( "bup" )->Set ( "badun" ),
			obj->Add_Component < Points > ( )->Set ( 
				Punteggio1->Add_Component < Text > ( )->Set ( "font", "text", "cam", {1,1,1,1} ), 
				Punteggio2->Add_Component < Text > ( )->Set ( "font", "text", "cam", {1,1,1,1} )
			)
		)->Set ( Player1, Player2 );
	}
};

int load ( std::string base_dir = "" ) {
	int loaded = 0;

    loaded += Manager::Texture_Load ( "sprite", ( base_dir + ( "Data/Textures/empty.png" ) ).c_str ( ) );
    loaded += Manager::Texture_Load ( "noise", ( base_dir + ( "Data/Textures/noise.png" ) ).c_str ( ), 1 );

    loaded += Manager::Font_Load ( "font", ( base_dir + "Data/Font.ttf" ).c_str( ), 90, 13 );

    loaded += Manager::Shader_Load ( "sprite", ( base_dir + "Data/Shaders/sprite.vs" ).c_str( ), ( base_dir + "Data/Shaders/sprite.fs" ).c_str( ) );
    // loaded += Manager::Shader_Load ( "framebuffer", ( base_dir + "Data/Shaders/PostProcessing/framebuffer.vs" ).c_str( ), ( base_dir + "Data/Shaders/PostProcessing/framebuffer.fs" ).c_str( ) );
    loaded += Manager::Shader_Load ( "default", ( base_dir + "Data/Shaders/PostProcessing/default.vs" ).c_str( ), ( base_dir + "Data/Shaders/PostProcessing/default.fs" ).c_str( ) );
    loaded += Manager::Shader_Load ( "crt_effect", ( base_dir + "Data/Shaders/PostProcessing/crt_effect.vs" ).c_str( ), ( base_dir + "Data/Shaders/PostProcessing/crt_effect.fs" ).c_str( ) );
    loaded += Manager::Shader_Load ( "trace", ( base_dir + "Data/Shaders/PostProcessing/trace.vs" ).c_str( ), ( base_dir + "Data/Shaders/PostProcessing/trace.fs" ).c_str( ) );
    loaded += Manager::Shader_Load ( "glitch", ( base_dir + "Data/Shaders/PostProcessing/glitch.vs" ).c_str( ), ( base_dir + "Data/Shaders/PostProcessing/glitch.fs" ).c_str( ) );
    loaded += Manager::Shader_Load ( "text", ( base_dir + "Data/Shaders/text.vs" ).c_str( ), ( base_dir + "Data/Shaders/text.fs" ).c_str( ) );

	loaded += Manager::Source_Load ( "bip" );

	loaded += Manager::Buffer_Load ( "bip", ( base_dir + "Data/Sounds/bip.wav" ).c_str( ) );
	loaded += Manager::Buffer_Load ( "bup", ( base_dir + "Data/Sounds/bup.wav" ).c_str( ) );
	loaded += Manager::Buffer_Load ( "badun", ( base_dir + "Data/Sounds/badun.wav" ).c_str( ) );

	return loaded;
}

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif

// classic pong game with local multi player
int main ( ) {
	std::string custom_path = "ReKat/Arcade/Projekt/Pong/";
	custom_path = "";

#ifdef __APPLE__
	CFURLRef appUrlRef = CFBundleCopyResourceURL(CFBundleGetMainBundle(), CFSTR("Font"), CFSTR("ttf"), CFSTR("Data"));
	CFStringRef filePathRef = CFURLCopyPath(appUrlRef);
	const char* filePath = CFStringGetCStringPtr(filePathRef, kCFStringEncodingUTF8);
	custom_path = filePath;

	for ( int i = 0; i < 13; i++) 
	{ custom_path.pop_back ( ); }
	custom_path.push_back ( '/' );

	// Release references
	CFRelease(filePathRef);
	CFRelease(appUrlRef);
#endif
	
	ReKat::phisiks::Start ( 120 );
	ReKat::grapik::Start ( "Pong", 800, 600,false,false,true, ( custom_path + "Data/Textures/favicon.png" ).c_str ( ) );
	Bound_Input_Handler->_FreamBufferResize = __FreamBufferResize;
	ReKat::synth::Start ( );

	auto Splash			= Manager::Objekt_Load ( "splash" );
	auto PostProcessor	= Manager::Objekt_Load ( "post processor", {0,0,0}, { 1333, 1000, 100 } );
	auto Scene			= Manager::Objekt_Load ( "scene0" );
	
	Splash->Add_Component < Splash_Screen > ( );
	auto processor = PostProcessor->Add_Component < Framebuffer > ( )->Set ( Scene )->Set ( "default" )->Set ( 800, 600 );
	Scene->Add_Component < Inizilize_Scene > ( )->post = processor;

	if ( load ( custom_path ) != 0 ) { DEBUG ( 1, "RESOURCES NOT LOADED" ); }

	Manager::Camera_Load ( "cam", Scene, PostProcessor->Get_Component < Framebuffer > ( ) );
	Manager::Set_Active_Scene ( "splash" );
	ReKat::phisiks::Set_Active ( "scene0" );

	// add noise texture to framebuffer shader
	Manager::Shader_Get ( "crt_effect" )->setInt ( "screenTexture", 0 );
	Manager::Shader_Get ( "crt_effect" )->setInt ( "noiseTexture", 1 );
	Manager::Shader_Get ( "crt_effect" )->setFloat ( "time", 0 );
	Manager::Shader_Get ( "glitch" )->setInt ( "noiseTexture", 1 );
	Manager::Shader_Get ( "glitch" )->setFloat ( "time", 0 );

	STATE old_state = current_state;
	int scenes = 0;
	int shader = 0;

	while ( ReKat::grapik::IsEnd ( ) ) {
		glClearColor(0.0, 0.0, 0.0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

	Manager::Texture_Get ( "noise" )->Use ( );
	Manager::Shader_Get ( "crt_effect" )->setFloat ( "time", Timer::current_time );
	Manager::Shader_Get ( "glitch" )->setFloat ( "time", Timer::current_time );

		if ( Key_Down ( "P" ) ) {
			if ( current_state == PAUSED ) 
			{ current_state = old_state; }
			else {
				old_state = current_state;
				current_state = PAUSED;
			}
		}

		if ( Key_Down ( "G" ) ) {
			Manager::Free_Objekt ( "scene" + std::to_string ( scenes ) );
			scenes++;
			current_state = START;
			switch ( scenes % 3) {
				case 0: mode = SOLO; break;
				case 1: mode = NORMAL; break;
				case 2: mode = HOKEY; break;
			}
			auto Gion = Manager::Objekt_Load ( "scene" + std::to_string ( scenes ) );
			Gion->Add_Component < Inizilize_Scene > ( )->post = processor;
			processor->Set ( Gion );
			Gion->Start ( );

			ReKat::phisiks::Set_Active ( "scene" + std::to_string ( scenes ) );
		}

		if ( Key_Down ( "E" ) ) {
			shader++;
			switch ( shader % 3 ) {
				case 0: processor->Set ( "default" ); break;
				case 1: processor->Set ( "crt_effect" ); break;
				case 2: processor->Set ( "glitch" ); break;
				case 3: processor->Set ( "trace" ); break;
			}
		}

		if ( Key_Down ( "X" ) ) { ReKat::grapik::End ( ); }

		Manager::Update ( );
		ReKat::grapik::Update ( );
		ReKat::phisiks::Update ( );
	}

	Manager::Free ( );
	ReKat::grapik::End ( );
	ReKat::synth::End ( );
	return 0;
}
