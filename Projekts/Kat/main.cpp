#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#define OPENGL
#define OPENAL
#define ALL_MODULES
#include <ReKat.hpp>
using namespace ReKat;
#include "divide.hpp"

class Music : public Behaviour {
	float color = 0;
	virtual void Update ( ) {
		if ( grapik::Input::Key_Down ( name ) ) { 
			color = 1;
			synth::Play( "buf" + name );
		}

		if ( color > 0 ) {
			Manager::Shader_Get("sin")->setVec2 ("freq[" + name + "]", 440.0 * pow ( 2, (std::stoi(name)) / 4.0 ), color);
			Manager::UI_Object_Get(name)->Set_Color( {color,0,0,1} );
			color -= Timer::delta_time;
		} else {
			Manager::UI_Object_Get(name)->Set_Color( {0,0,0,1} );
			Manager::Shader_Get("sin")->setVec2 ("freq[" + name + "]", 0.0f, color);
		}
	}
};

class AA : public Behaviour {
	void Update ( ) { 
		if ( grapik::Input::Key_Down ( "Enter" ) ) {
			std::cout << "AAAAAAAAAAAAAAAAAAAAAAAA\n";
			synth::Play("AA");
		}
	}
};

float Sample_Shape ( float i, int duration, int crescita ) {
	double Derivata_C;
    if ( i >= crescita && i <= duration - crescita ) {
        Derivata_C = 1;
    } else if (i < crescita ) {
        Derivata_C = (double)i / (double)crescita;
    } else {
        Derivata_C = (double)( crescita - ( i - (duration - crescita) ) ) / (double)crescita;
    }
	return Derivata_C;
}

void Load_Samples ( void ) {
	// generate 10 tones
	float sample = 2 * M_PI / 44100;
	int len = 44100;

	for (float i = 0; i < 10; i++) {
		short * buf = ( short* ) calloc ( len, sizeof(short) );
		float Freq = 440.0 * pow ( 2, (i) / 12.0 );
		for (size_t i = 0; i < len; i++) 
		{ buf[i] = 10000.0 * sin ( Freq * i * sample ); }
		
		// make the begin and end tend to 0
		for (size_t i = 0; i < len; i++) 
		// { buf[i] *= Sample_Shape ( i, len - 1, len/5 ); }
		{ buf[i] *= sin ( i * sample ); }


		synth::Create_Buffer ( "buf" + std::to_string ((int)(i)), buf, len );
		synth::Create_Source ( "src" + std::to_string ((int)(i)), {0,0,0}, true );
	}
}

int load ( ) {
	int result = 0 << '\n';
	result += Manager::Shader_Load ( "empty_sprite",  "Shaders/sprite_animation.vs", "Shaders/sprite_animation.fs" );
	result += Manager::Shader_Load ( "sin",  "Shaders/show_graph.vs", "Shaders/show_graph.fs" );
	result += Manager::Shader_Load ( "text", "Shaders/text.vs", "Shaders/text.fs" );
	Manager::Shader_Get("empty_sprite")->setInt ("image", 0);
	// Manager::Shader_Get("text")->setInt ("text", 0);

	result += Manager::Texture_Load ( "empty_sprite", "Data/empty.png" );

	result += Manager::Sprite_Load ( "key", "empty_sprite", "empty_sprite" );
	result += Manager::Sprite_Load ( "sin", "sin", "empty_sprite" );
	
	result += Manager::Text_Load ( "text", "death_record.ttf", "text" );
	
	Manager::Scene_Load ( "piano" )
			->Add_UI_Shader ( Manager::Shader_Get("sin") )
			->Add_UI_Shader( Manager::Shader_Get ("text") )
			->Add_UI_Shader( Manager::Shader_Get("empty_sprite") )
			->Add_UI_Object ( Manager::UI_Object_Load ( "cok", "", "sin", "", {0,-150}, {1200,100}, nullptr ) )
			->Add_UI_Object ( Manager::UI_Object_Load ( "teticoli", "dd", "sin", "text", {0,0}, {200,400}, nullptr ) );
	Manager::UI_Object_Get ( "cok" )->Add_component < AA > ();
	Manager::UI_Object_Get ( "teticoli" )->Set_Color({0.5,0.5,.05,1});
	for (int i = 1; i < 11; i++) {
		Manager::UI_Object_Load ( std::to_string ( ( i == 10 ? 0 : i ) ), "", "key", "", {100*i - 550 ,400},{80,300}, nullptr )->Add_component<Music>();
		Manager::Scene_Get( "piano")->Add_UI_Object ( Manager::UI_Object_Get ( std::to_string ( ( i == 10 ? 0 : i ) ) ) );
	}

	return result;
}

static void __FreamBufferResize ( GLFWwindow* window, int width, int height ) {
    glViewport ( 0, 0, ReKat::grapik::Internal::SCR_WIDTH = width, ReKat::grapik::Internal::SCR_HEIGTH = height );
}

void Char ( GLFWwindow *, unsigned int scan ) {
	synth::Play ( std::string(1,(char)scan));
}

#include <Synth/FFT/test.h>

int main ( int argc, char const *argv[] ) {
	grapik::Start ( "Anna", 800, 600, false, false, true );
	synth::Start ( );
	ReKat::grapik::Input::_Caracters = Char;
    ReKat::grapik::Input::_FreamBufferResize = &__FreamBufferResize;
	Load_Samples ( );
	
	std::vector < std::string > alfabeto = {"a","a1","b","c","ch","d","e","e1","f","g","gh","i","i1","j","k","l","m","n","o","o1","p","q","r","s","t","u","u1","v","w","x","y","z"};
	std::cout << "taglia: " << Taglia ( "Voce.wav", alfabeto ) << '\n';

	for ( auto l : alfabeto ) 
	{ std::cout << "lettera " + l + ": " << synth::Create_Buffer ( l, l + ".wav" ) << '\n'; }
	
	test ( );

	/// synth::Create_Buffer ( "AA", _buf, _buf.size() );
	
	Manager::Empty();
	std::cout << "load:result: " << load() << '\n';
	Manager::Set_Active_Scene ( "piano" );

	int Fps = 0;
	int t = time (0);
	while ( grapik::IsEnd ( ) ) {
		Manager::Shader_Get("sin")->setFloat ("time", Timer::Get_Time());
		Fps ++;
		if ( t + 1 == time(0) ) { 
			std::cout << "\n-------------- Fps: " << Fps << '\n'; 
			Fps = 0; t = time (0); 
		}

		Manager::Update ( );
		grapik::Pool ( );
	}
	
	grapik::End ( );
	synth::End ( );
	return 0;
}