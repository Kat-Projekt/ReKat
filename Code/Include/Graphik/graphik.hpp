#ifndef GRAPIK
#define GRAPIK

#include <iostream>
#include <string>
#include <map>
#include <glm/glm.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

namespace ReKat {
namespace grapik {
// error codes
enum Status {
	SUCCESS,
	// window
	FAILED_LOAD_GLAD,
	FAILED_CREATING_WINDOW,
	ALDREADY_CLOSED,
};

// Internal variables
namespace Internal {
	static GLFWwindow* window;
	static unsigned int SCR_WIDTH;
	static unsigned int SCR_HEIGTH;	
} /*Internal*/

// this namespace is ment to be used for defining Input function external to the main namespace
namespace Input {
	enum Mode {
		NONE,
		PRESSED,
		HELD,
		RELEASED
	};
	static std::map < std::string, Mode > keys;
	static glm::vec2 mouse_pos = {0,0};
	static glm::vec2 old_mouse_pos;
	static float screen_ration = 1;

	static void(*_Keyboard)  (GLFWwindow*, int, int, int, int) = nullptr;
	static void(*_Mouse_pos) (GLFWwindow*, double, double ) = nullptr;
	static void(*_Mouse_key) (GLFWwindow*, int, int, int ) = nullptr;
	static void(*_Caracters) (GLFWwindow*, unsigned int ) = nullptr;
	static void(*_ScrollWell)(GLFWwindow*, double, double ) = nullptr;
	static void(*_FreamBufferResize)(GLFWwindow*, int, int ) = nullptr;

	static void Keyboard ( GLFWwindow* window, int key, int scancode, int action, int mode ) {
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
		// adding caracter keys
		if ( GLFW_KEY_A <= key && key <= GLFW_KEY_Z ) { 
			if ( action == GLFW_PRESS ) { keys[std::string(1,(char)key)] = PRESSED; }
			if ( action == GLFW_RELEASE ) { keys[std::string(1,(char)key)] = RELEASED; }
		}
		// adding number keys
		if ( key >= GLFW_KEY_0 && key <= GLFW_KEY_9 ) {
			if ( action == GLFW_PRESS ) { keys[std::string(1,(char)key)] = PRESSED; }
			if ( action == GLFW_RELEASE ) { keys[std::string(1,(char)key)] = RELEASED; }
		}
	}
	static void Mouse_pos ( GLFWwindow* window, double xpos, double ypos ) {
		if ( _Mouse_pos != nullptr ) 
		{ _Mouse_pos ( window, xpos, ypos ); }
		old_mouse_pos = mouse_pos;
		mouse_pos = {xpos, ypos};
        // traslate mouse_pos
        mouse_pos.x -= ReKat::grapik::Internal::SCR_WIDTH/2;
		mouse_pos.y = -mouse_pos.y + ReKat::grapik::Internal::SCR_HEIGTH/2;
        // resize mouse_pos
        mouse_pos = glm::vec2 ( mouse_pos.x / (ReKat::grapik::Internal::SCR_WIDTH) * 1000 * 
                            ((float)ReKat::grapik::Internal::SCR_WIDTH / (float)ReKat::grapik::Internal::SCR_HEIGTH)
                            , mouse_pos.y / ReKat::grapik::Internal::SCR_HEIGTH * 1000 );
	}
	static void Mouse_key ( GLFWwindow* window, int button, int action, int mode ) {
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
	static void ScrollWell ( GLFWwindow* window, double xoffset, double yoffset ) {
		if ( _ScrollWell != nullptr ) 
		{ _ScrollWell ( window, xoffset, yoffset ); }
	}
	static void FreamBufferResize ( GLFWwindow* window, int width, int height ) {
		ReKat::grapik::Internal::SCR_HEIGTH = height;
		ReKat::grapik::Internal::SCR_WIDTH = width;
		screen_ration = (float)ReKat::grapik::Internal::SCR_WIDTH / (float)ReKat::grapik::Internal::SCR_HEIGTH;
		if ( _FreamBufferResize != nullptr ) 
		{ _FreamBufferResize ( window, width, height ); }
	}
	static void Caracters ( GLFWwindow* window, unsigned int codepoint ) {
		if ( _Caracters != nullptr ) 
		{ _Caracters ( window, codepoint ); }
	}
	
	static void Configure ( ) {
		// add first 128 ascii caracters
		for (char i = 32; i < 127; i++) {
			keys[std::string(1,(char)i)] = NONE;
		}
		// add special caracters:
		keys["Esc"] = NONE;
		keys["Ctrl"] = NONE;
		keys["Shift"] = NONE;
		keys["Alt"] = NONE;
		keys["Enter"] = NONE;
		keys["Del"] = NONE;
		keys["Mouse1"] = NONE;
		keys["Mouse2"] = NONE;
		keys["Mouse3"] = NONE;
	}
	static void Print_Status ( ) {
		for ( auto k : keys ) 
		{ std::cout << "Key: " << k.first << " Value: " << k.second << '\n'; }
	}

	static void Update ( ) {
		for ( auto k = keys.begin(); k != keys.end(); k++ ) {
			if ( k->second == PRESSED ) { k->second = HELD; }
			if ( k->second == RELEASED ) { k->second = NONE; }
		}
	}
	static bool Key_Down ( std::string key ) 
	{ return ( keys[key] == PRESSED ? true : false ); }
	static bool Key_Pressed ( std::string key ) 
	{ return ( keys[key] != NONE ? true : false ); }
	static bool Key_Up ( std::string key ) 
	{ return ( keys[key] == RELEASED ? true : false ); }
} /*Input*/

	// Basic Grapik functions
	static int Start ( std::string name, unsigned int SCR_WIDTH, unsigned int SCR_HEIGTH,  bool transparent = false, bool fullscreen = false, bool resizable = false );
	static int End ( ); 
	static int IsEnd ( );
	static void Terminate ( );
	static int SetIcon ( std::string path );
	static void Pool ( );
	
} /* Grapik */ } // ReKat

// Basic Grapik function Implementaion
namespace ReKat::grapik {
	static int Start 
	( std::string name, unsigned int SCR_WIDTH, unsigned int SCR_HEIGTH,  bool transparent, bool fullscreen, bool resizable ) {
		Internal::SCR_HEIGTH = SCR_HEIGTH; Internal::SCR_WIDTH = SCR_WIDTH;
		Input::screen_ration = (float)ReKat::grapik::Internal::SCR_WIDTH / (float)ReKat::grapik::Internal::SCR_HEIGTH;

		//Input::Configure();

		glfwInit ( );
		glfwWindowHint ( GLFW_CONTEXT_VERSION_MAJOR, 3 );
		glfwWindowHint ( GLFW_CONTEXT_VERSION_MINOR, 3 );
		glfwWindowHint ( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	#ifdef __APPLE__
		glfwWindowHint ( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
	#endif
		if ( transparent ) {
			glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
			glfwWindowHint(GLFW_DECORATED, 0);
		}

		if ( fullscreen ) { glfwWindowHint(GLFW_DECORATED, 0); }

		glfwWindowHint ( GLFW_RESIZABLE, resizable );

		if ( fullscreen ) {
			GLFWmonitor* _monitor =  glfwGetPrimaryMonitor ( );
			Internal::window = glfwCreateWindow(16, 9, name.c_str() , _monitor, NULL );
			// const GLFWvidmode* _mode = glfwGetVideoMode ( _monitor );
			//glfwSetWindowSize(Internal::window, _mode->width, _mode->height );
		} else 
		{ Internal::window = glfwCreateWindow ( Internal::SCR_WIDTH, Internal::SCR_HEIGTH, name.c_str(), nullptr, nullptr ); }

		if ( Internal::window == NULL ) 
		{ glfwTerminate(); return FAILED_CREATING_WINDOW; }

		glfwMakeContextCurrent ( Internal::window );
		glfwSetKeyCallback ( Internal::window, Input::Keyboard );
		glfwSetCursorPosCallback ( Internal::window, Input::Mouse_pos );
		glfwSetMouseButtonCallback ( Internal::window, Input::Mouse_key );
		glfwSetScrollCallback ( Internal::window, Input::ScrollWell );
		glfwSetFramebufferSizeCallback ( Internal::window, Input::FreamBufferResize );
		glfwSetCharCallback( Internal::window, Input::Caracters );

		glfwSetInputMode ( Internal::window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN );

		// glad: load all OpenGL function pointers
		// ---------------------------------------
		if ( !gladLoadGLLoader ( (GLADloadproc)glfwGetProcAddress ) ) 
		{ return FAILED_LOAD_GLAD; }

		glViewport ( 0, 0, Internal::SCR_WIDTH, Internal::SCR_HEIGTH );
		glEnable ( GL_CULL_FACE ); glEnable ( GL_BLEND ); glEnable(GL_DEPTH_TEST);
		glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

		return SUCCESS;
	}

	static int End ( ) { 
		if ( IsEnd() ) 
		{ glfwSetWindowShouldClose( Internal::window, true); return SUCCESS; }
		else 
		{ return ALDREADY_CLOSED; }
	}

	static void Terminate ( ) 
	{ End(); glfwTerminate(); }

	static int IsEnd ( ) 
	{ return !glfwWindowShouldClose(Internal::window); }

	static int SetIcon ( std::string path ) {
		GLFWimage images[1]; 
		images[0].pixels = stbi_load ( path.c_str () , &images[0].width, &images[0].height, 0, 4 ); //rgba channels 
		glfwSetWindowIcon ( Internal::window, 1, images ); 
		stbi_image_free ( images[0].pixels );

		return SUCCESS;
	}

	static void Pool ( ) 
	{ glfwSwapBuffers ( Internal::window ); glfwPollEvents ( ); }
}

#endif