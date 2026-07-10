#pragma once

#include <unordered_map>
#include "core/window.hpp"

namespace ReKat {
namespace grapik {
	extern std::unordered_map < std::string, Window* > _windows;
	extern Window* _current_window;

	extern int Start
	(
		std::string name,
		unsigned int SCR_WIDTH,
		unsigned int SCR_HEIGTH,
		bool transparent = false,
		bool fullscreen = false,
		bool resizable = false,
		std::string icon_path = ""
	);
	extern void Clear_Screen
	(
		float r = 0.0f,
		float g = 0.0f,
		float b = 0.0f,
		float a = 1.0f
	);
	extern void Clear_Screen
	(
		unsigned char r = 0,
		unsigned char g = 0,
		unsigned char b = 0,
		unsigned char a = -1
	);
	extern void Update ( );
	extern void Set_Active_Window ( std::string name );
	extern void End ( std::string window = "" );
	extern int Is_End ( std::string window = "" );
	extern void Terminate ( );
} }
