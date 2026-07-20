#pragma once

#include "graphik_debugger.hpp"
#include "../resource.hpp"
#include "input.hpp"

#include <stb_image.h>

class Window : public Resource {
private:
	GLFWwindow* window;
public:
	std::string name;
	Input input; // input module
	unsigned int Width;
	unsigned int Heigth;	
	float Screen_Ratio = 0;

	int Make ( );
	int Make
	(
		std::string name,
		unsigned int SCR_WIDTH,
		unsigned int SCR_HEIGTH,
		std::string icon_path = "",
		bool transparent = false,
		bool fullscreen = false,
		bool resizable = false
	);

	void Use ( );
	void End ( ); 
	bool Is_End ( );
	void Pool ( );
};
