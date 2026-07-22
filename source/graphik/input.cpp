#include "extensions/graphik/window.hpp"

void ReKat::Graphik::Keyboard
( GLFWwindow* window, int key, int scancode, int action, int mode )
{
	Window* _window = static_cast<Window*>( glfwGetWindowUserPointer ( window ) );
	if ( _window != nullptr )
	{ _window->input.Keyboard ( window, key, scancode, action, mode ); }
}

void ReKat::Graphik::Mouse_pos
( GLFWwindow* window, double xpos, double ypos )
{
	Window* _window = static_cast<Window*>( glfwGetWindowUserPointer ( window ) );
	if ( _window != nullptr )
	{ _window->input.Mouse_pos ( window, xpos, ypos ); }
}

void ReKat::Graphik::Mouse_key
( GLFWwindow* window, int button, int action, int mode )
{
	Window* _window = static_cast<Window*>( glfwGetWindowUserPointer ( window ) );
	if ( _window != nullptr )
	{ _window->input.Mouse_key ( window, button, action, mode ); }
}

void ReKat::Graphik::ScrollWell
( GLFWwindow* window, double xoffset, double yoffset )
{
	Window* _window = static_cast<Window*>( glfwGetWindowUserPointer ( window ) );
	if ( _window != nullptr )
	{ _window->input.ScrollWell ( window, xoffset, yoffset ); }
}

void ReKat::Graphik::FreamBufferResize
( GLFWwindow* window, int width, int height )
{
	Window* _window = static_cast<Window*>( glfwGetWindowUserPointer ( window ) );
	if ( _window != nullptr )
	{ _window->input.FreamBufferResize ( window, width, height ); }
}

void ReKat::Graphik::Caracters
( GLFWwindow* window, unsigned int codepoint )
{
	Window* _window = static_cast<Window*>( glfwGetWindowUserPointer ( window ) );
	if ( _window != nullptr )
	{ _window->input.Caracters ( window, codepoint ); }
}


void Input::Update ( ) {
	for ( auto k = keys.begin(); k != keys.end(); k++ )
	{
		if ( k->second == InputMode::PRESSED ) { k->second = InputMode::HELD; }
		if ( k->second == InputMode::RELEASED ) { k->second = InputMode::NONE; }
	}
	typed = "";
}


bool Input::Key_Down
( std::string key ) 
{ return ( keys[key] == InputMode::PRESSED ? true : false ); }

bool Input::Key_Pressed
( std::string key ) 
{ return ( keys[key] != InputMode::NONE ? true : false ); }

bool Input::Key_Up
( std::string key ) 
{ return ( keys[key] == InputMode::RELEASED ? true : false ); }

bool Input::Key_Down
( int key ) 
{ return ( keys[std::to_string(key)] == InputMode::PRESSED ? true : false ); }

bool Input::Key_Pressed
( int key ) 
{ return ( keys[std::to_string(key)] != InputMode::NONE ? true : false ); }

bool Input::Key_Up
( int key ) 
{ return ( keys[std::to_string(key)] == InputMode::RELEASED ? true : false ); }


void Input::Keyboard
( GLFWwindow* window, int key, int scancode, int action, int mode )
{
	DEBUG ( 3, "Updating Keyboard" );
	if ( _Keyboard != nullptr ) 
	{ _Keyboard ( window, key, scancode, action, mode ); }
	// adding meta charaters
	if ( action == GLFW_PRESS ) {
		if ( key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL ) { keys["Ctrl"] = InputMode::PRESSED; return; }
		if ( key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT ) { keys["Shift"] = InputMode::PRESSED; return; }
		if ( key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT ) { keys["Alt"] = InputMode::PRESSED; return; }
		if ( key == GLFW_KEY_DELETE || key == GLFW_KEY_BACKSPACE ) { keys["Del"] = InputMode::PRESSED; return; }
		if ( key == GLFW_KEY_ENTER ) { keys["Enter"] = InputMode::PRESSED; return; }
		if ( key == GLFW_KEY_ESCAPE ) { keys["Esc"] = InputMode::PRESSED; return; }
	}
	if ( action == GLFW_RELEASE ) {
		if ( key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL ) { keys["Ctrl"] = InputMode::RELEASED; return; }
		if ( key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT ) { keys["Shift"] = InputMode::RELEASED; return; }
		if ( key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT ) { keys["Alt"] = InputMode::RELEASED; return; }
		if ( key == GLFW_KEY_DELETE || key == GLFW_KEY_BACKSPACE ) { keys["Del"] = InputMode::RELEASED; return; }
		if ( key == GLFW_KEY_ENTER ) { keys["Enter"] = InputMode::RELEASED; return; }
		if ( key == GLFW_KEY_ESCAPE ) { keys["Esc"] = InputMode::RELEASED; return; }	
	}

	if ( ( GLFW_KEY_A <= key && key <= GLFW_KEY_Z ) || 
			( key >= GLFW_KEY_0 && key <= GLFW_KEY_9 ) ||
			( key == GLFW_KEY_SPACE )) { 
		if ( action == GLFW_PRESS ) { keys[std::string(1,(char)key)] = InputMode::PRESSED; }
		if ( action == GLFW_RELEASE ) { keys[std::string(1,(char)key)] = InputMode::RELEASED; }
	}

	// default
	switch ( action ) {
		case GLFW_RELEASE: keys[std::to_string(key)] = InputMode::RELEASED; break;
		case GLFW_PRESS:  keys[std::to_string(key)] = InputMode::PRESSED; break;
	}
	
	DEBUG ( 5, "Key ", key );
}

void Input::Mouse_pos
( GLFWwindow* window, double xpos, double ypos )
{
	Window* _window = static_cast<Window*>( glfwGetWindowUserPointer ( window ) );
	if ( _window == nullptr )
	{ return; }

	DEBUG ( 3, "Updating Mouse" );
	if ( _Mouse_pos != nullptr ) 
	{ _Mouse_pos ( window, xpos, ypos ); }
	old_mouse_pos = mouse_pos;
	mouse_pos = {xpos, ypos};
	// traslate mouse_pos
	mouse_pos.x -= _window->Width/2;
	mouse_pos.y = -mouse_pos.y + _window->Heigth/2;
	// resize mouse_pos
	mouse_pos = glm::vec2 (
		mouse_pos.x / (_window->Width) * 1000 * 
		( (float)_window->Width / (float)_window->Heigth )
		, mouse_pos.y / _window->Heigth * 1000 );
}

void Input::Mouse_key
( GLFWwindow* window, int button, int action, int mode )
{
	if ( _Mouse_key != nullptr ) 
	{ _Mouse_key ( window, button, action, mode ); }
	if ( action == GLFW_PRESS ) {
		if ( button == GLFW_MOUSE_BUTTON_LEFT ) { keys["Mouse1"] = InputMode::PRESSED; return; }
		if ( button == GLFW_MOUSE_BUTTON_MIDDLE ) { keys["Mouse2"] = InputMode::PRESSED; return; }
		if ( button == GLFW_MOUSE_BUTTON_RIGHT ) { keys["Mouse3"] = InputMode::PRESSED; return; }
	}
	if ( action == GLFW_RELEASE ) {
		if ( button == GLFW_MOUSE_BUTTON_LEFT ) { keys["Mouse1"] = InputMode::RELEASED; return; }
		if ( button == GLFW_MOUSE_BUTTON_MIDDLE ) { keys["Mouse2"] = InputMode::RELEASED; return; }
		if ( button == GLFW_MOUSE_BUTTON_RIGHT ) { keys["Mouse3"] = InputMode::RELEASED; return; }
	}
}

void Input::ScrollWell
( GLFWwindow* window, double xoffset, double yoffset )
{
	DEBUG ( 3, "Updating ScroolWell" );
	scrool_pos += yoffset;
	if ( _ScrollWell != nullptr ) 
	{ _ScrollWell ( window, xoffset, yoffset ); }
}

void Input::FreamBufferResize
( GLFWwindow* window, int width, int height )
{
	Window* _window = static_cast<Window*>( glfwGetWindowUserPointer ( window ) );
	if ( _window == nullptr )
	{ return; }

	DEBUG ( 3, "Updating Framebuffer" );
	glViewport ( 0, 0, _window->Width = width, _window->Heigth = height );
	if ( _FreamBufferResize != nullptr ) 
	{ _FreamBufferResize ( window, width, height ); }

#ifdef __APPLE__
	// count on the internal resize for macos ( retina )
	width /= 2;
	height /= 2;
#endif

	_window->Heigth = height;
	_window->Width = width;
	_window->Screen_Ratio = (float)_window->Width / (float)_window->Heigth;
}

void Input::Caracters
( GLFWwindow* window, unsigned int codepoint )
{
	typed.push_back ( (char)codepoint );
	if ( _Caracters != nullptr ) 
	{ _Caracters ( window, codepoint ); }
}