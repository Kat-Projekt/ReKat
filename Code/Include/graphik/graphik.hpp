#ifndef GRAPIK
#define GRAPIK

#include <iostream>
#include <string>
#include <chrono>
#include <map>
#include <glm/glm.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "graphik_debugger.hpp"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

#include "../resource.hpp"
enum Mode {
	NONE,
	PRESSED,
	HELD,
	RELEASED
};

struct Input {
	std::map < std::string, Mode > keys;
	std::string typed;
	glm::vec2 mouse_pos = {0,0};
	glm::vec2 old_mouse_pos;
	float screen_ration = 1;
	double scrool_pos = 0;

	void (*_Keyboard)  (GLFWwindow*, int, int, int, int) = nullptr;
	void (*_Mouse_pos) (GLFWwindow*, double, double ) = nullptr;
	void (*_Mouse_key) (GLFWwindow*, int, int, int ) = nullptr;
	void (*_Caracters) (GLFWwindow*, unsigned int ) = nullptr;
	void (*_ScrollWell)(GLFWwindow*, double, double ) = nullptr;
	void (*_FreamBufferResize)(GLFWwindow*, int, int ) = nullptr;

	void Keyboard ( GLFWwindow* window, int key, int scancode, int action, int mode );
	void Mouse_pos ( GLFWwindow* window, double xpos, double ypos );
	void Mouse_key ( GLFWwindow* window, int button, int action, int mode );
	void ScrollWell ( GLFWwindow* window, double xoffset, double yoffset );
	void FreamBufferResize ( GLFWwindow* window, int width, int height );
	void Caracters ( GLFWwindow* window, unsigned int codepoint );

	void Update ( ) {
		for ( auto k = keys.begin(); k != keys.end(); k++ ) {
			if ( k->second == PRESSED ) { k->second = HELD; }
			if ( k->second == RELEASED ) { k->second = NONE; }
		}
		typed = "";
	}
	bool Key_Down ( std::string key ) 
	{ return ( keys[key] == PRESSED ? true : false ); }
	bool Key_Pressed ( std::string key ) 
	{ return ( keys[key] != NONE ? true : false ); }
	bool Key_Up ( std::string key ) 
	{ return ( keys[key] == RELEASED ? true : false ); }
};

Input* Bound_Input_Handler = nullptr;

static void Keyboard ( GLFWwindow* window, int key, int scancode, int action, int mode ) {
	if ( Bound_Input_Handler != nullptr ) 
	{ Bound_Input_Handler->Keyboard ( window, key, scancode, action, mode ); }
}
static void Mouse_pos ( GLFWwindow* window, double xpos, double ypos ) {
	if ( Bound_Input_Handler != nullptr ) 
	{ Bound_Input_Handler->Mouse_pos ( window, xpos, ypos ); }
}
static void Mouse_key ( GLFWwindow* window, int button, int action, int mode ) {
	if ( Bound_Input_Handler != nullptr ) 
	{ Bound_Input_Handler->Mouse_key ( window, button, action, mode ); }
}
static void ScrollWell ( GLFWwindow* window, double xoffset, double yoffset ) {
	if ( Bound_Input_Handler != nullptr ) 
	{ Bound_Input_Handler->ScrollWell ( window, xoffset, yoffset ); }
}
static void FreamBufferResize ( GLFWwindow* window, int width, int height ) {
	if ( Bound_Input_Handler != nullptr ) 
	{ Bound_Input_Handler->FreamBufferResize ( window, width, height ); }
}
static void Caracters ( GLFWwindow* window, unsigned int codepoint ) {
	if ( Bound_Input_Handler != nullptr ) 
	{ Bound_Input_Handler->Caracters ( window, codepoint ); }
}

class Window : public Resource {
private:
	GLFWwindow* window;
public:
	std::string name;
	Input input; // input module
	unsigned int Width;
	unsigned int Heigth;	
	float Screen_Ratio = 0;

    int Make  ( std::string name, unsigned int SCR_WIDTH, unsigned int SCR_HEIGTH,
				std::string icon_path = "",
				bool transparent = false, bool fullscreen = false, bool resizable = false ) {
		this->name = name;
		Width = SCR_WIDTH;
		Heigth = SCR_HEIGTH;
		Screen_Ratio = (float) SCR_WIDTH / (float) SCR_HEIGTH;

		glfwInit ( );
		glfwWindowHint ( GLFW_CONTEXT_VERSION_MAJOR, 3 );
		glfwWindowHint ( GLFW_CONTEXT_VERSION_MINOR, 3 );
		glfwWindowHint ( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

		// anti aliasing
		// glfwWindowHint(GLFW_SAMPLES, 4);
		// glEnable(GL_MULTISAMPLE); 
	#ifdef __APPLE__
		glfwWindowHint ( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
	#endif

		if ( transparent ) {
			glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
			glfwWindowHint(GLFW_DECORATED, 0);
		}

		if ( !resizable ) { glfwWindowHint ( GLFW_RESIZABLE, resizable ); }

		if ( fullscreen ) {
			glfwWindowHint(GLFW_DECORATED, 0);
			GLFWmonitor* _monitor =  glfwGetPrimaryMonitor ( );
			window = glfwCreateWindow( Width, Heigth, name.c_str() , _monitor, nullptr );
		} else 
		{ window = glfwCreateWindow ( Width, Heigth, name.c_str(), nullptr, nullptr ); }

		if ( window == NULL )
		{ glfwTerminate(); return 1; }

		glfwMakeContextCurrent ( window );
		glfwSetKeyCallback ( window, Keyboard );
		glfwSetCursorPosCallback ( window, Mouse_pos );
		glfwSetMouseButtonCallback ( window, Mouse_key );
		glfwSetScrollCallback ( window, ScrollWell );
		glfwSetFramebufferSizeCallback ( window, FreamBufferResize );
		glfwSetCharCallback( window, Caracters );

		if ( !gladLoadGLLoader ( (GLADloadproc)glfwGetProcAddress ) ) 
		{ return 1; }

		#ifdef __APPLE__
			// count on the internal resize for macos
			Width *= 2;
			Heigth *= 2;
		#endif

		glViewport ( 0, 0, Width, Heigth );
		glEnable ( GL_CULL_FACE ); 
		glEnable ( GL_BLEND ); 
		glEnable ( GL_DEPTH_TEST );
		glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glFrontFace ( GL_CCW );

		Bound_Input_Handler = &input;

		// icon
		if ( icon_path != "" ) {
			GLFWimage images[1]; 
			images[0].pixels = stbi_load ( icon_path.c_str () , &images[0].width, &images[0].height, 0, 4 ); //rgba channels 
			glfwSetWindowIcon ( window, 1, images ); 
			stbi_image_free ( images[0].pixels );
		}
		
		DEBUG ( 3, "Inizialized Graphik Window" );
		return 0;
	}

	int IsEnd ( ) {
		// DEBUG ( 5, "Is End? ", name );
		Use ( );
		// DEBUG ( 5, !glfwWindowShouldClose( window ) ? "YES" : "NO" );
		GL_CHECK_ERROR;
		return !glfwWindowShouldClose( window );
	}

    void End ( ) {
		if ( IsEnd() ) 
		{ glfwSetWindowShouldClose( window, true ); }
	}
    
	void Use ( ) 
	{ glfwMakeContextCurrent ( window ); Bound_Input_Handler = &input; }

	void Pool ( ) {
		input.Update ( );
		glfwSwapBuffers ( window ); GL_CHECK_ERROR;
		glfwPollEvents ( ); GL_CHECK_ERROR;
	}
};

Window* Bound_Window_Handler = nullptr;

void Input::Keyboard ( GLFWwindow* window, int key, int scancode, int action, int mode ) {
	DEBUG ( 3, "Updating Keyboard" );
	if ( _Keyboard != nullptr ) 
	{ _Keyboard ( window, key, scancode, action, mode ); }
	// adding meta charaters
	if ( action == GLFW_PRESS ) {
		if ( key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL ) { keys["Ctrl"] = PRESSED; return; }
		if ( key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT ) { keys["Shift"] = PRESSED; return; }
		if ( key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT ) { keys["Alt"] = PRESSED; return; }
		if ( key == GLFW_KEY_DELETE || key == GLFW_KEY_BACKSPACE ) { keys["Del"] = PRESSED; return; }
		if ( key == GLFW_KEY_ENTER ) { keys["Enter"] = PRESSED; return; }
		if ( key == GLFW_KEY_ESCAPE ) { keys["Esc"] = PRESSED; return; }
	}
	if ( action == GLFW_RELEASE ) {
		if ( key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL ) { keys["Ctrl"] = RELEASED; return; }
		if ( key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT ) { keys["Shift"] = RELEASED; return; }
		if ( key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT ) { keys["Alt"] = RELEASED; return; }
		if ( key == GLFW_KEY_DELETE || key == GLFW_KEY_BACKSPACE ) { keys["Del"] = RELEASED; return; }
		if ( key == GLFW_KEY_ENTER ) { keys["Enter"] = RELEASED; return; }
		if ( key == GLFW_KEY_ESCAPE ) { keys["Esc"] = RELEASED; return; }	
	}

	if ( ( GLFW_KEY_A <= key && key <= GLFW_KEY_Z ) || 
			( key >= GLFW_KEY_0 && key <= GLFW_KEY_9 ) ||
			( key == GLFW_KEY_SPACE )) { 
		if ( action == GLFW_PRESS ) { keys[std::string(1,(char)key)] = PRESSED; }
		if ( action == GLFW_RELEASE ) { keys[std::string(1,(char)key)] = RELEASED; }
	}
}
void Input::Mouse_pos ( GLFWwindow* window, double xpos, double ypos ) {
	if ( Bound_Window_Handler == nullptr ) { return; }
	DEBUG ( 3, "Updating Mouse" );
	if ( _Mouse_pos != nullptr ) 
	{ _Mouse_pos ( window, xpos, ypos ); }
	old_mouse_pos = mouse_pos;
	mouse_pos = {xpos, ypos};
	// traslate mouse_pos
	mouse_pos.x -= Bound_Window_Handler->Width/2;
	mouse_pos.y = -mouse_pos.y + Bound_Window_Handler->Heigth/2;
	// resize mouse_pos
	mouse_pos = glm::vec2 ( mouse_pos.x / (Bound_Window_Handler->Width) * 1000 * 
						((float)Bound_Window_Handler->Width / (float)Bound_Window_Handler->Heigth)
						, mouse_pos.y / Bound_Window_Handler->Heigth * 1000 );
}
void Input::Mouse_key ( GLFWwindow* window, int button, int action, int mode ) {
	if ( _Mouse_key != nullptr ) 
	{ _Mouse_key ( window, button, action, mode ); }
	if ( action == GLFW_PRESS ) {
		if ( button == GLFW_MOUSE_BUTTON_LEFT ) { keys["Mouse1"] = PRESSED; return; }
		if ( button == GLFW_MOUSE_BUTTON_MIDDLE ) { keys["Mouse2"] = PRESSED; return; }
		if ( button == GLFW_MOUSE_BUTTON_RIGHT ) { keys["Mouse3"] = PRESSED; return; }
	}
	if ( action == GLFW_RELEASE ) {
		if ( button == GLFW_MOUSE_BUTTON_LEFT ) { keys["Mouse1"] = RELEASED; return; }
		if ( button == GLFW_MOUSE_BUTTON_MIDDLE ) { keys["Mouse2"] = RELEASED; return; }
		if ( button == GLFW_MOUSE_BUTTON_RIGHT ) { keys["Mouse3"] = RELEASED; return; }
	}
}
void Input::ScrollWell ( GLFWwindow* window, double xoffset, double yoffset ) {
	DEBUG ( 3, "Updating ScroolWell" );
	scrool_pos += yoffset;
	if ( _ScrollWell != nullptr ) 
	{ _ScrollWell ( window, xoffset, yoffset ); }
}
void Input::FreamBufferResize ( GLFWwindow* window, int width, int height ) {
	if ( Bound_Window_Handler == nullptr ) { return; }
	DEBUG ( 3, "Updating Framebuffer" );
	glViewport ( 0, 0, Bound_Window_Handler->Width = width, Bound_Window_Handler->Heigth = height );
	if ( _FreamBufferResize != nullptr ) 
	{ _FreamBufferResize ( window, width, height ); }

#ifdef __APPLE__
	// count on the internal resize for macos
	width /= 2;
	height /= 2;
#endif

	Bound_Window_Handler->Heigth = height;
	Bound_Window_Handler->Width = width;
	Bound_Window_Handler->Screen_Ratio = (float)Bound_Window_Handler->Width / (float)Bound_Window_Handler->Heigth;
}
void Input::Caracters ( GLFWwindow* window, unsigned int codepoint ) {
	typed.push_back ( (char)codepoint );
	if ( _Caracters != nullptr ) 
	{ _Caracters ( window, codepoint ); }
}

namespace ReKat {
namespace grapik {
// this namespace is ment to be used for defining Input function external to the main namespace
namespace Input {
	static void Update ( ) {
		if ( Bound_Input_Handler == nullptr ) { return; }
		Bound_Input_Handler->Update ( ); }
	static bool Key_Down ( std::string key ) {
		if ( Bound_Input_Handler == nullptr ) { return false; }
		return Bound_Input_Handler->Key_Down ( key ); }
	static bool Key_Pressed ( std::string key ) {
		if ( Bound_Input_Handler == nullptr ) { return false; }
		return Bound_Input_Handler->Key_Pressed ( key ); }
	static bool Key_Up ( std::string key ) {
		if ( Bound_Input_Handler == nullptr ) { return false; }
		return Bound_Input_Handler->Key_Up ( key ); }
}
	static std::unordered_map < std::string, Window* > _windows;

	// Basic Grapik functions
	static int Start ( std::string name, unsigned int SCR_WIDTH, unsigned int SCR_HEIGTH,
					   bool transparent = false, bool fullscreen = false, bool resizable = false, std::string icon_path = "" ) {

		Window *t = new Window;
        _windows.insert( { name , t } );
		Bound_Window_Handler = t;
        return (*t).Make( name, SCR_WIDTH, SCR_HEIGTH, icon_path, transparent, fullscreen, resizable );
	}
	static void Update ( ) {
		for ( auto W : _windows ) 
		{ W.second->Pool ( ); }
	}
	static void Bound_Window ( std::string name ) {
		Bound_Window_Handler = _windows [ name ];
		Bound_Window_Handler->Use ( );
	}
	static void End ( std::string window = "" ) 
	{ if ( window == "" ) { Bound_Window_Handler->End ( ); } }
	static int IsEnd ( std::string window = "" ) {
		// DEBUG ( 4, "Getting is End of window ",
		// ( window == "" ? "DEFAULT" : window ) );
		// DEBUG ( 4, Bound_Window_Handler->name );
		if ( window == "" ) { return Bound_Window_Handler->IsEnd ( ); }
		return true;
	}
	static void Terminate ( ) {
		for ( auto W : _windows ) 
		{ W.second->End ( ); }
	}
	static void Pool ( ) { Update ( ); }
	
} /* Grapik */ } // ReKat

// Basic Grapik function Implementaion

#endif