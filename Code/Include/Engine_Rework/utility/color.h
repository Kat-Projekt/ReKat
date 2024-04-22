#ifndef COLOR
#define COLOR
#if (defined (LINUX) || defined (__linux__) || defined (__APPLE__)) // linux implementation
	#include <stdio.h>
	#include <string>
	void color ( std::string string, int color ) {
		printf( ( std::string ( "\033[" ) + std::to_string ( color ) + std::string ( "m" ) + string + std::string ( "\033[0m" ) ).c_str() );
	}
#elif (defined (_WIN32) || defined (_WIN64)) // windows implementaion
	#include <stdio.h>
	#include <windows.h>
	void color ( const char* string, WORD color ) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, color);
		printf( string );
		SetConsoleTextAttribute( hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY );
	}
#endif
#endif