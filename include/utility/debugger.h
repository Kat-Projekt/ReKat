#pragma once
/***************************************************
 * @file debugger.h
 * @brief Debugger usage
 * \defgroup Debug Debugger informations and methods
 * 
 * ## DEBUG macro usage
 * ```cpp
 * DEBUG ( <Level>, ... ); 
 * // for example
 * DEBUG ( ERROR, "CANNOT LOAD SHADER" );
 * DEBUG ( NOTICE, "Hello", " world", 1234 );
 * ```
 * 
 * ## DEBUG_LEVEL usage
 * Normaly the debug level is setted to NOTICE
 * to increment the debug level you need to define:
 * ```cpp
 * #define DEBUG_LEVEL <LEVEL>
 * // for all debug messages use
 * #define DEBUG_LEVEL VERBOSE
 * ```
 **************************************************/

/*******************
 * @addtogroup Debug
 * @{
 */
enum class DebugLevel : int {
	/** @brief used for system errors, stops the program and prints the stackframe. */
	FATAL = 0,
	/** @brief used for components/resources errors, stops the program and prints the stackframe. */
	ERROR = 1,
	/** @brief used when something shoud work but the default behaviour is used */
	WARN = 2,
	/** @brief used to notify the use */
	NOTICE = 3,
	/** @brief for more precise notifications */
	INFO = 4,
	/** @brief for telling the user something is not setted usualy after a notification */
	TRACE = 5,
	/** @brief for implementation detatils */
	VERBOSE = 6
};

/** @} */

namespace ReKat {
namespace Debug {

#ifndef DEBUG_LEVEL
/***************************
 * @brief The level of debug
 **************************/
inline constexpr DebugLevel DefaultDebugLevel = DebugLevel::NOTICE;
#else
inline constexpr DebugLevel DefaultDebugLevel = DEBUG_LEVEL;
#endif

/******************************************
 * @brief [internal] prints the stack trace
 * 
 * To test the windows implementation
 *****************************************/
inline void _print_stack_backtrace ( void );

#if ( defined (LINUX) || defined (__linux__) || defined (__APPLE__) ) // unix implementation
	#include "printer.h"
	#include <string>
	#include <execinfo.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <stdexcept>

	#define FOREGROUND_RED 1
	#define FOREGROUND_BLUE 2
	#define FOREGROUND_GREEN 4
	#define FOREGROUND_INTENSITY 0
	#define BACKGROUND_RED 8
	#define BACKGROUND_BLUE 16
	#define BACKGROUND_GREEN 32
	#define BACKGROUND_INTENSITY 0

	inline void _print_colored (
		const char* string,
		int color
	) {
		// used for converting windows colors to linux ones
		int foreground_color = ( color & 1 ) == 1 ? 31 : ( color & 2 ) == 2 ? 34 : ( color & 4 ) == 4 ? 32 : 0;
		int background_color = ( color & 8 ) == 8 ? 41 : ( color & 16 ) == 16 ? 44 : ( color & 32 ) == 32 ? 42 : 0;

		std::cout << "\033[1";
		if ( foreground_color )
		{
			std::cout << ';';
			std::cout << foreground_color;
		}
		if ( background_color )
		{
			std::cout << ';';
			std::cout << background_color;
		}
		std::cout << "m";
		std::cout << string;
		std::cout << "\033[0m";
	}

	inline void _print_stack_backtrace
	( void )
	{
		void* frames [32];
		int count = backtrace ( frames, 32 );

		char** symbols = backtrace_symbols ( frames, count );

		for ( int i = 0; i < count; ++i )
		{ std::cout << symbols [i] << '\n'; }

		free ( symbols );
	}
#elif ( defined (_WIN32) || defined (_WIN64) ) // windows implementaion
	#include "printer.h"
	#include <string>
	#include <windows.h>
	#include <dbghelp.h>
	#include <stdio.h>
	#include <stdexcept>

	#pragma comment(lib, "Dbghelp.lib")

	inline void _print_colored (
		const char* string,
		WORD color
	) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleOutputCP(CP_UTF8);
		SetConsoleTextAttribute(hConsole, color);
		std::cout << string;
		SetConsoleTextAttribute( hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY );
	}

	inline void _print_stack_backtrace
	( void )
	{
		void* stack[32];
		USHORT frames = CaptureStackBackTrace(0, 32, stack, nullptr);

		HANDLE process = GetCurrentProcess();

		static bool initialized = false;
		if (!initialized)
		{
			SymInitialize(process, nullptr, TRUE);
			initialized = true;
		}

		SYMBOL_INFO* symbol = (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 256, 1);
		symbol->MaxNameLen = 255;
		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

		for (USHORT i = 0; i < frames; ++i)
		{
			DWORD64 displacement = 0;
			if (SymFromAddr(process, (DWORD64)stack[i], &displacement, symbol))
			{
				printf("#%u %s +0x%llx\n",
					i,
					symbol->Name,
					(unsigned long long)displacement);
			}
			else
			{
				printf("#%u %p\n", i, stack[i]);
			}
		}

		free(symbol);
	}
#endif

/************************************************
 * @brief [internal] gets the file name from path
 * 
 * @param file the file path
 * @return the name of the file with extension
 ***********************************************/
inline const char* _strip_root_path 
( const char* file )
{
	if ( !file )
	{ return nullptr; }

	const char* filename = file;

	while ( *file )
	{
		if ( *file == '/' || *file == '\\' )
		{ filename = file + 1; }
		file++;
	}

	return filename;
}

/***********************************************************
 * \brief [internal] for storing the information for a level
 **********************************************************/
struct LevelInfo
{
	const char* name;
	// because on windows it is WORD and unix int
	decltype(FOREGROUND_RED) color;
};


/***********************************************
 * @brief [internal] processes the debug command
 **********************************************/
template < typename ... Args >
inline void _debug_printer (
	DebugLevel level,
	const char * file,
	int line,
	Args && ... args
) {
	if ( level > DefaultDebugLevel )
	{ return; }

	constexpr LevelInfo level_descriptor[] =
	{
		{ "FATAL",    FOREGROUND_RED | FOREGROUND_INTENSITY },
		{ "ERROR",    FOREGROUND_RED | FOREGROUND_INTENSITY },
		{ "WARN",     FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY },
		{ "NOTICE",   FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY },
		{ "INFO",     FOREGROUND_GREEN | FOREGROUND_INTENSITY },
		{ "TRACE",    FOREGROUND_INTENSITY },
		{ "VERBOSE",  FOREGROUND_INTENSITY }
	};

	auto level_info = level_descriptor [ static_cast < int > ( level ) ];

	std::cout << "[";
	_print_colored ( level_info.name, level_info.color );
	std::cout << "] " << _strip_root_path ( file ) << ":" << line << " -> ";
	( std::cout << ... << std::forward < Args > ( args ) );
	std::cout << '\n';

	// in case of errors print the stack strace
	if ( level == DebugLevel::FATAL )
	{
		_print_stack_backtrace ( );
		throw std::runtime_error (
			"FATAL ERROR on "
			+ std::string ( _strip_root_path ( file ) ) +
			":"
			+ std::to_string ( line )
		);
	}

	if ( level == DebugLevel::ERROR )
	{ _print_stack_backtrace ( ); }
}

} }
/**
 * @addtogroup Debug
 * @{
 */
#ifndef DEBUG
/*********************************************************************
 * @brief Used for sending debugging messages to stdout
 * 
 * \param level specifies the debugger level and is of type DebugLevel
 * \param ... the variadic arcuments to print
 ********************************************************************/
#define DEBUG(level, ...) ReKat::Debug::_debug_printer (level,__FILE__,__LINE__,__VA_ARGS__)
#endif
/** @} */
