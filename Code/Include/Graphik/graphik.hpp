#ifndef GRAPIK
#define GRAPIK

#include <string>

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
	static void Keyboard ( GLFWwindow* window, int key, int scancode, int action, int mode );
	static void Mouse_pos ( GLFWwindow* window, double xpos, double ypos );
	static void Mouse_key ( GLFWwindow* window, int button, int action, int mods );
	static void ScrollWell ( GLFWwindow* window, double xoffset, double yoffset );
	static void FreamBufferResize ( GLFWwindow* window, int width, int height );
	static void Caracters ( GLFWwindow* window, unsigned int codepoint );
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

		// glad: load all OpenGL function pointers
		// ---------------------------------------
		if ( !gladLoadGLLoader ( (GLADloadproc)glfwGetProcAddress ) ) 
		{ return FAILED_LOAD_GLAD; }

		glViewport ( 0, 0, Internal::SCR_WIDTH, Internal::SCR_HEIGTH );
		glEnable ( GL_CULL_FACE ); glEnable ( GL_BLEND ); 
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