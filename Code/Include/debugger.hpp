#ifndef COLOR
#define COLOR
#if (defined (LINUX) || defined (__linux__) || defined (__APPLE__)) // unix implementation
#include <iostream>
#include <string>
#define FOREGROUND_RED 1
#define FOREGROUND_BLUE 2
#define FOREGROUND_GREEN 4
#define FOREGROUND_INTENSITY 0
#define BACKGROUND_RED 8
#define BACKGROUND_BLUE 16
#define BACKGROUND_GREEN 32
#define BACKGROUND_INTENSITY 0

	void color ( std::string string, int color ) {
		int foreground_color = ( color & 1 ) == 1 ? 31 : ( color & 2 ) == 2 ? 34 : ( color & 4 ) == 4 ? 32 : 0;
		int background_color = ( color & 8 ) == 8 ? 41 : ( color & 16 ) == 16 ? 44 : ( color & 32 ) == 32 ? 42 : 0;

		std::cout << ( (  std::string ( "\033[1;" ) 
						+ ( foreground_color != 0 ? std::to_string ( foreground_color ) : std::string( ) )
						+ ( background_color != 0 ? std::string ( ";" ) + std::to_string ( background_color ) : std::string( ) ) + std::string ( "m" )
						+ string
						+ std::string ( "\033[0m" ) ).c_str() );
	}
#elif (defined (_WIN32) || defined (_WIN64)) // windows implementaion
	#include <stdio.h>
	#include <windows.h>
	#include <iostream>
	void color ( const char* string, WORD color ) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, color);
		std::cout << string;
		SetConsoleTextAttribute( hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY );
	}
#endif
#endif

#ifndef DEBUG
#define DIAGNOSTIC
#ifdef DIAGNOSTIC
#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 8
#endif

#include <iostream>

#if (defined (LINUX) || defined (__linux__) || defined (__APPLE__)) // unix implementation
// #define DEBUG(level, ...) ( ( level < DEBUG_LEVEL ) ? ( DEBUG_##level(__VA_ARGS__) ) : ( std::cout ) )
#define DEBUG(...)
#else
#define DEBUG(level, ...) ( ( level < DEBUG_LEVEL ) ? ( DEBUG_##level(__VA_ARGS__) ) : ( std::cout ) )
#endif

#define DEBUG_0(...) ( color ( "MESSAGE TO TERMINATE", FOREGROUND_RED | FOREGROUND_INTENSITY ), throw )
#define DEBUG_1(...) ( color ( "FATAL", FOREGROUND_RED | FOREGROUND_INTENSITY ), __DEBUG( "", __VA_ARGS__ ), throw )
#define DEBUG_2(...) ( color ( "ERROR", FOREGROUND_RED | FOREGROUND_INTENSITY ), __DEBUG( "", __VA_ARGS__ ) )
#define DEBUG_3(...) ( color ( "WARN", FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY ), __DEBUG( "", __VA_ARGS__ ) )
#define DEBUG_4(...) ( color ( "NOTICE", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY ), __DEBUG( "", __VA_ARGS__ ) ) 
#define DEBUG_5(...) ( color ( "INFO", FOREGROUND_GREEN | FOREGROUND_INTENSITY ), __DEBUG( "", __VA_ARGS__ ) )
#define DEBUG_6(...) __DEBUG( "TRACE", __VA_ARGS__ )
#define DEBUG_7(...) ( color ( "IMPORTANT", FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY ), __DEBUG( "", __VA_ARGS__ ) )

/*	debug levels
	0. TERMINATE
	1. FATAL
	2. ERROR
	3. WARN
	4. NOTICE
	5. INFO
	6. TRACE
	7. IMPORTANT NOTICE
*/

#define __DEBUG(error_type,...) std::cout , error_type, "\t[" , time(0) , "]: " __FILE__ , " at " , __LINE__ , "\t: " , __VA_ARGS__ , std::endl
template <typename T>
std::ostream& operator,(std::ostream& out, const T& t)
{ out << t; return out; }
std::ostream& operator,(std::ostream& out, std::ostream&(*f)(std::ostream&))
{ out << f; return out; }

#else
#define DEBUG(...)
#define __DEBUG(...)
#endif

#endif