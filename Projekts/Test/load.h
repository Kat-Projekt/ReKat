#define OPENGL
#define OPENAL
#define ALL_MODULES
#include <ReKat.hpp>

class Hello : public Behaviour {
	public:
	void Start ( ) { std::cout << "Obj: " << obj->Get_Name() << " says: hello world\n"; }
	void Update ( ) { }
};

using namespace ReKat::grapik::Input;

class Player : public Behaviour {
private:
    float speed = 200.0f;
    float camera_speed = 4.0f;
    bool camera_centered = true;
    int gobleino = 1;
    glm::vec2 m_ = {0,0};
	float progression = 10;
	float attack_duration = 0.15;
	float attack_angle;
	float attack_spread = 1.5;
	int Killed_gobelins = 0;
public:

	void Kill ( ) { Killed_gobelins ++; }

    void Update ( ) {
        // controls
        glm::vec2 dpos = {0,0};
        if ( Key_Pressed( "A" ) ) { dpos += glm::vec2{-1,0}; }
        if ( Key_Pressed( "W" ) ) { dpos += glm::vec2{0,1};  }
        if ( Key_Pressed( "D" ) ) { dpos += glm::vec2{1,0};  }
        if ( Key_Pressed( "S" ) ) { dpos += glm::vec2{0,-1}; }
        obj->Inc_Pos ( vec3(Normalize(dpos) * speed * Timer::delta_time, 0) );
		
		if ( Key_Pressed( "A" ) && Key_Pressed( "W" ) && Key_Pressed( "D" ) && Key_Pressed( "S" ) ) {
			obj->Set_Active(false);
		}

        /*if ( Key_Down ( "Mouse1" ) && progression >= attack_duration ) {
			progression = 0;
			attack_angle = angle( mouse_pos );
            Manager::Object_Get("Spada")->Set_Active(true);
        }
		if ( progression <= attack_duration ) {
			progression += Timer::delta_time;
			// spada rotation
			Manager::Object_Get("Player")->Get_Sub_Object("Spada")->Rotate( attack_angle - ( attack_spread * progression/attack_duration ) + attack_spread * 0.5f );
		} else { Manager::Object_Get("Spada")->Set_Active(false);  }

		if ( Key_Down ( "E" ) ) {
			std::cout << "E";
			Manager::Object_Get("S")->Get_component<Dialog>()->Start();
		}*/

		/*if ( Key_Down ("P") ) {
			std::cout << "Play\n";
			ReKat::synth::Play ( "Player", "Player" );
		}*/

        // if ( Key_Down ( "P" ) ) { Manager::Object_Load("gobelino_" + std::to_string(gobleino++), "gobelino", {100,100},{100,100} )->Add_component<gobelino>()->name = "gobelino_"+ std::to_string(gobleino);  }
    }
};

static void __FreamBufferResize ( GLFWwindow* window, int width, int height ) {
    glViewport ( 0, 0, ReKat::grapik::Internal::SCR_WIDTH = width, ReKat::grapik::Internal::SCR_HEIGTH = height );
}

int Load ( ) {
	_FreamBufferResize = __FreamBufferResize;

	int result = 0;
	result += Manager::Shader_Load  ( "sprite", "sprite.vs", "sprite.fs");
	result += Manager::Texture_Load ( "sprite", "sprite.png" );

	Objekt main ( "main" );
	Objekt pier ( "pier" );
	Objekt gio  ( "gio" );
	
	main.Add_Child ( &gio );
	main.Add_Child ( &pier );

	auto cam = pier.Add_component < Camera > ( );
	auto sprite = gio.Add_component < Sprite > ( );
	// gio.Add_component < Player > ( );
	main.Add_component < Fps > ( );

	sprite->Set ( Manager::Texture_Get( "sprite" ), Manager::Shader_Get( "sprite" ), cam, {2,2} );

	Scene_Manager::Set_Active_Scene ( &main );

	while ( ReKat::grapik::IsEnd ( ) ) {
		glClearColor(0.6, 0.4, 0.5, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		Scene_Manager::Update ( );

		std::cout << '\n';
		ReKat::grapik::Update ( );
    }

	std::cout << "\nresources loaded\n";
	return result;
}