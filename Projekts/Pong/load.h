#define OPENGL
#define OPENAL
#define ONLINE_PEER
#define ALL_MODULES
#include <ReKat.hpp>
using namespace ReKat::grapik::Input;

class Player : public Behaviour {
	public:
	bool P1 = true;
	float speed = 200;
	void Update ( ) {
		if ( Key_Pressed ("Shift") ) { speed = 800; } else { speed = 200; }
		if ( P1 ) {
			if ( Key_Pressed ( "W" ) ) { obj->Get_component < Rigidbody > ( )->velocity = {0, speed,0}; return; }
			if ( Key_Pressed ( "S" ) ) { obj->Get_component < Rigidbody > ( )->velocity = {0,-speed,0}; return; }
		} else {
			if ( Key_Pressed ( "I" ) ) { obj->Get_component < Rigidbody > ( )->velocity = {0, speed,0}; return; }
			if ( Key_Pressed ( "K" ) ) { obj->Get_component < Rigidbody > ( )->velocity = {0,-speed,0}; return; }
		}
		obj->Get_component < Rigidbody > ( )->velocity = {0,0,0};
	}
};

class Ball : public Behaviour {
	public:
	int P1 = 0;
	int P2 = 0;
	Shader* S;

	void Start ( ) {
		S = Manager::Shader_Get ( "point" );
	}

	void Fixed_Update ( ) {
		if ( obj->Get_Pos ( ).x < -700 ) { P1 ++; obj->Set_Pos({400,0,0}); obj->Get_component < Rigidbody > ( )->velocity = {-400,90,0};}
		if ( obj->Get_Pos ( ).x > 700 ) { P2 ++; obj->Set_Pos({-400,0,0}); obj->Get_component < Rigidbody > ( )->velocity = {400,-90,0};}
		S->setInt ( "P1", P1 );
		S->setInt ( "P2", P2 );
	}
};

int Load ( ) {
	int result = 0;
	Manager::Shader_Load ( "bar", "bar.vs", "bar.fs" );
	Manager::Shader_Load ( "sprite", "sprite.vs", "sprite.fs" );
	Manager::Shader_Load ( "point", "sprite.vs", "point.fs" );
	Manager::Shader_Load ( "framebuffers", "framebuffers.vs", "framebuffers.fs" );
	
	result += Manager::Texture_Load ( "sprite", "sprite.png" );
	if ( result != 0 ) { return 1; }


	Objekt *main = new Objekt ( "main" );
	Objekt *camera = new Objekt ( "camera" );
	Objekt *points = new Objekt ( "points", {0,450,0}, {200,100,100} );
	Objekt *ball = new Objekt ( "ball", { 0,0,0 }, {50,50,50} );
	Objekt *walls = new Objekt ( "walls" );
	Objekt *wall_top = new Objekt ( "wall_T", {0,550,0}, {1200,100,0} );
	Objekt *wall_bot = new Objekt ( "wall_B", {0,-550,0}, {1200,100,0} );
	Objekt *pier = new Objekt ( "pier", {600,0,0}, {10,100,0} );
	Objekt *gio = new Objekt ( "gio", {-600,0,0}, {10,100,0}  );

	walls->Add_Child ( wall_top )->Add_Child ( wall_bot );
	main->Add_Child ( ball )->Add_Child ( walls )->Add_Child ( pier )->Add_Child( gio )->Add_Child ( points );

	main->Add_component < Phisiks > ( );
	main->Add_component < Fps > ( )->MAX_FPS = 104;
	camera->Add_component < Camera > ( );

	ball->Add_component < Sprite > ( )->
	Set ( Manager::Texture_Get( "sprite" ), Manager::Shader_Get( "sprite" ), camera->Get_component < Camera > ( ), {1,2} )->frame = 1;
	gio->Add_component < Sprite > ( )->
	Set ( Manager::Texture_Get( "sprite" ), Manager::Shader_Get( "bar" ), camera->Get_component < Camera > ( ) );
	pier->Add_component < Sprite > ( )->
	Set ( Manager::Texture_Get( "sprite" ), Manager::Shader_Get( "bar" ), camera->Get_component < Camera > ( ) );
	points->Add_component < Sprite > ( )->
	Set ( Manager::Texture_Get( "sprite" ), Manager::Shader_Get( "point" ), camera->Get_component < Camera > ( ) );
	

	ball->Add_component < Sfere_Collider > ( )->Set_Size(50);
	gio->Add_component < Box_Collider > ( )->Set_Size({10,100,0})->Set_Static ( true );
	pier->Add_component < Box_Collider > ( )->Set_Size({10,100,0})->Set_Static ( true );
	wall_top->Add_component < Box_Collider > ( )->Set_Size(wall_top->Get_Size( ));
	wall_bot->Add_component < Box_Collider > ( )->Set_Size(wall_bot->Get_Size( ));

	ball->Add_component < Rigidbody > ( )->bouncy = 1;
	ball->Get_component < Rigidbody > ( )->velocity = {400,400,0};
	gio->Add_component < Rigidbody > ( );
	pier->Add_component < Rigidbody > ( );

	ball->Add_component < Ball > ( );
	gio->Add_component < Player > ( );
	pier->Add_component < Player > ( )->P1 = false;


	Objekt *rays = new Objekt ( "ray" );
	rays->Add_component < Framebuffer > ( )->Set ( main, Manager::Shader_Get ( "framebuffers" ), rays->Add_component < Camera > ( ) );

	main->Start();
	Scene_Manager::Set_Active_Scene ( rays );

	gio->Get_component < Box_Collider > ( )->Set_Static ( true );
	pier->Get_component < Box_Collider > ( )->Set_Static ( true );

	return result;
}