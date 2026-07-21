#include "extensions/graphik/window.hpp"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_image_write.h>
#endif

int DEBUG_GRAPHIK_ERROR_CODE = 0;

int Window::Make
( ) { return 1; }

int Window::Make
(
	std::string name,
	unsigned int SCR_WIDTH,
	unsigned int SCR_HEIGTH,
	std::string icon_path,
	bool transparent,
	bool fullscreen,
	bool resizable
) {
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
	glfwSetKeyCallback ( window, ReKat::Graphik::Keyboard );
	glfwSetCursorPosCallback ( window, ReKat::Graphik::Mouse_pos );
	glfwSetMouseButtonCallback ( window, ReKat::Graphik::Mouse_key );
	glfwSetScrollCallback ( window, ReKat::Graphik::ScrollWell );
	glfwSetFramebufferSizeCallback ( window, ReKat::Graphik::FreamBufferResize );
	glfwSetCharCallback( window, ReKat::Graphik::Caracters );
	glfwSetWindowUserPointer ( window, this );

	if ( !gladLoadGLLoader ( (GLADloadproc)glfwGetProcAddress ) ) 
	{ return 1; }

	#ifdef __APPLE__
		// count on the internal resize for macos
		Width *= 2;
		Heigth *= 2;
	#endif

	glViewport ( 0, 0, Width, Heigth );
	// glEnable ( GL_CULL_FACE ); 
	glEnable ( GL_BLEND ); 
	glEnable ( GL_DEPTH_TEST );
	glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glFrontFace ( GL_CCW );

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


bool Window::Is_End
( )
{
	Use ( );
	GL_CHECK_ERROR;
	return !glfwWindowShouldClose( window );
}

void Window::End
( )
{
	if ( Is_End ( ) ) 
	{ glfwSetWindowShouldClose( window, true ); }
}
    
void Window::Use
( ) 
{
	glfwMakeContextCurrent ( window );
}

void Window::Pool
( ) {
	input.Update ( );
	glfwSwapBuffers ( window ); GL_CHECK_ERROR;
	glfwPollEvents ( ); GL_CHECK_ERROR;
}

