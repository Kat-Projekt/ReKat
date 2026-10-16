#include <core/diagnostic/debugger.h>

namespace ReKat {
namespace Debug {
	/***************************
	 * @brief The level of debug
	 **************************/
	#ifndef DEBUG_LEVEL
		inline constexpr DebugLevel DefaultDebugLevel = DebugLevel::INFO;
	#else
		inline constexpr DebugLevel DefaultDebugLevel = DEBUG_LEVEL;
	#endif

	/******************************
	 * @brief If errors are printed
	 *****************************/
	#ifndef DEBUG_SILENT_DEBUGGER
		inline constexpr bool SilenceDebugger = false;
	#else
		inline constexpr bool SilenceDebugger =  DEBUG_SILENT_DEBUGGER;
	#endif

	std::string _print_colored (
			const char* message,
			std::string_view color
	)
	{ return std::string ( color ) + message + std::string ( _Reset ); }

#if ( defined (LINUX) || defined (__linux__) || defined (__APPLE__) ) // unix implementation
	std::string _print_stack_backtrace
	( void )
	{
		void* frames [32];
		int count = backtrace ( frames, 32 );

		char** symbols = backtrace_symbols ( frames, count );

		std::ostringstream stream;

		for ( int i = 0; i < count; ++i )
		{ stream << symbols [i] << '\n'; }

		free ( symbols );

		return stream.str ( );
	}
#elif ( defined (_WIN32) || defined (_WIN64) ) // windows implementaion	
	std::string_view _print_stack_backtrace
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

	const char* _strip_root_path ( const char* file )
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

	void _debug_printer_out_wrapper
	( const std::string & final_message )
	{
		std::cout << final_message;
	}

	void _debug_printer_impl (
		DebugLevel level,
		const char * file,
		int line,
		const char * message
	) {
		if ( SilenceDebugger && level > DebugLevel::ERROR )
		{ return; }
		if ( level > DefaultDebugLevel )
		{ return; }

		static constexpr LevelInfo level_descriptor[] =
		{
			{ "FATAL",	_Magenta },
			{ "ERROR",	_Red },
			{ "WARN",	_Yellow },
			{ "NOTICE",	_Cyan },
			{ "INFO",	_Green },
			{ "TRACE",	_Blue },
			{ "VERBOSE",	_Reset }
		};

		auto level_info = level_descriptor [ static_cast < int > ( level ) ];

		std::ostringstream stream;

		// prints level
		stream << "[" << _print_colored ( level_info.name, level_info.color ) << "]";
		// prints where it is called
		stream << _strip_root_path ( file ) << ":" << line;
		// prints message
		stream << " -> " << message << '\n';

		// fatal check
		if ( level == DebugLevel::FATAL )
		{
			if ( !SilenceDebugger )
			{ stream << _print_stack_backtrace ( ); }

			_debug_printer_out_wrapper ( stream.str ( ) );

			throw std::runtime_error (
				"FATAL ERROR on "
				+ std::string ( _strip_root_path ( file ) ) +
				":"
				+ std::to_string ( line )
			);
		}

		// error check
		if ( level == DebugLevel::ERROR )
		{
			if ( !SilenceDebugger )
			{ stream << _print_stack_backtrace ( ); }

			_debug_printer_out_wrapper ( stream.str ( ) );

			throw std::runtime_error (
				"ERROR on "
				+ std::string ( _strip_root_path ( file ) ) +
				":"
				+ std::to_string ( line )
				+ " -> "
				+ message 
			);
		}

		_debug_printer_out_wrapper ( stream.str ( ) );
	}
} }
