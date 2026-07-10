#include "extensions/graphik/graphik.hpp"

namespace ReKat {
namespace grapik {
	std::unordered_map < std::string, Window* > _windows;
	Window* _current_window;

	extern int Start
	(
		std::string name,
		unsigned int SCR_WIDTH,
		unsigned int SCR_HEIGTH,
		bool transparent,
		bool fullscreen,
		bool resizable,
		std::string icon_path
	) {
		Window *t = new Window;
        	_windows.insert( { name , t } );
		_current_window = t;
        	return (*t).Make (
			name, SCR_WIDTH, SCR_HEIGTH, 
			icon_path, transparent, fullscreen, resizable
		);
	}

	extern void Clear_Screen
	(
		float r,
		float g,
		float b,
		float a
	) {
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
	}

	extern void Clear_Screen
	(
		unsigned char r,
		unsigned char g,
		unsigned char b,
		unsigned char a
	) {
		glClearColor( (float)r/256.0f, (float)g/256.0f, (float)b/256.0f, (float)a/256.0f );
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
	}

	extern void Update
	( ) {
		for ( auto W : _windows ) 
		{ W.second->Pool ( ); }
	}

	extern void Set_Active_Window
	( std::string name )
	{
		auto n_w = _windows.find ( name );
		if ( n_w == _windows.end ( ) )
		{ return; }
		_current_window = n_w->second;
		_current_window->Use ( );
	}

	extern void End
	( std::string window ) 
	{
		if ( window == "" )
		{ _current_window->End ( ); }
		else
		{ _windows[ window ]->End ( ); }
	}

	extern int Is_End
	( std::string window )
	{
		if ( window == "" )
		{ return _current_window->Is_End ( ); }
		else
		{ return _windows [ window ]->Is_End ( ); }
	}

	extern void Terminate
	( ) {
		for ( auto W : _windows ) 
		{ W.second->End ( ); }
	}
} }