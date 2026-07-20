#pragma once

#include "graphik_debugger.hpp"
#include <map>
#include <string>

namespace ReKat {
namespace Graphik {
	extern void Keyboard ( GLFWwindow* window, int key, int scancode, int action, int mode );
	extern void Mouse_pos ( GLFWwindow* window, double xpos, double ypos );
	extern void Mouse_key ( GLFWwindow* window, int button, int action, int mode );
	extern void ScrollWell ( GLFWwindow* window, double xoffset, double yoffset );
	extern void FreamBufferResize ( GLFWwindow* window, int width, int height );
	extern void Caracters ( GLFWwindow* window, unsigned int codepoint );
} }

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

	void Update ( );

	bool Key_Down ( std::string key );
	bool Key_Pressed ( std::string key );
	bool Key_Up ( std::string key );

	bool Key_Down ( int key );
	bool Key_Pressed ( int key );
	bool Key_Up ( int key );
};
