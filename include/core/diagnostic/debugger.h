#pragma once
#ifndef DEBUGGER_IMPL
#define DEBUGGER_IMPL

#include "printer.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <stdexcept>

#if ( defined (LINUX) || defined (__linux__) || defined (__APPLE__) ) // unix includes
	#include <execinfo.h>
	#include <stdlib.h>
#elif ( defined (_WIN32) || defined (_WIN64) ) // windows includes
	#include <windows.h>
	#include <dbghelp.h>

	#pragma comment(lib, "Dbghelp.lib")
#endif


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
	/******************************************
	 * @brief [internal] prints the stack trace
	 *****************************************/
	std::string _print_stack_backtrace ( void );
	
	/*****************************************
	 * @brief [internal] internal color values
	 ****************************************/
	inline constexpr std::string_view _Red		= "\033[31m";
	inline constexpr std::string_view _Green	= "\033[32m";
	inline constexpr std::string_view _Yellow	= "\033[33m";
	inline constexpr std::string_view _Blue		= "\033[34m";
	inline constexpr std::string_view _Magenta	= "\033[35m";
	inline constexpr std::string_view _Cyan		= "\033[36m";
	inline constexpr std::string_view _Reset	= "\033[0m";

	/**************************************************
	 * @brief [internal] formats a string to be colored
	 *************************************************/
	std::string _print_colored
	( const char* string, std::string_view color );

	/************************************************
	 * @brief [internal] gets the file name from path
	 * 
	 * @param file the file path
	 * @return the name of the file with extension
	 ***********************************************/
	const char* _strip_root_path 
	( const char* file );

	/***********************************************************
	 * \brief [internal] for storing the information for a level
	 **********************************************************/
	struct LevelInfo
	{
		const char* name;
		std::string_view color;
	};

	/***********************************************
	 * @brief [internal] processes the thread safety
	 **********************************************/
	void _debug_printer_out_wrapper
	( const std::string & final_message );

	/***********************************************
	 * @brief [internal] processes the debug printer
	 **********************************************/
	void _debug_printer_impl (
		DebugLevel level,
		const char * file,
		int line,
		const char * message
	);

	/***********************************************************
	 * @brief [internal] processes the debug arguments expansion
	 **********************************************************/
	template < typename ... Args >
	inline void _debug_printer (
		DebugLevel level,
		const char * file,
		int line,
		Args && ... args
	) {
		std::ostringstream stream;
		( stream << ... << std::forward < Args > ( args ) );
		_debug_printer_impl ( level, file, line, stream.str ( ).c_str ( ) );
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

#endif
