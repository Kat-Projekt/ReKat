#ifndef DEBUG_ONLINE
#define DEBUG_ONLINE

#include "../debugger.hpp"

int DEBUG_ONLINE_ERROR_CODE; // Global online error code

#define OL_CHECK_ERROR \
	{ DEBUG_ONLINE_ERROR_CODE = 0;      \
	if ( DEBUG_ONLINE_ERROR_CODE != 0 ) {\
        color ( "ERROR_G", FOREGROUND_INTENSITY | FOREGROUND_RED ); \
        switch ( DEBUG_ONLINE_ERROR_CODE ) { \
            case 0x0500: __DEBUG (  "", "GL_INVALID_ENUM" ); break; \
            case 0x0501: __DEBUG (  "", "GL_INVALID_VALUE" ); break; \
            case 0x0502: __DEBUG (  "", "GL_INVALID_OPERATION" ); break; \
            case 0x0503: __DEBUG (  "", "GL_STACK_OVERFLOW" ); break; \
            case 0x0504: __DEBUG (  "", "GL_STACK_UNDERFLOW" ); break; \
            case 0x0505: __DEBUG (  "", "GL_OUT_OF_MEMORY" ); break; \
            case 0x0506: __DEBUG (  "", "GL_INVALID_FRAMEBUFFER_OPERATION" ); break; \
            default: __DEBUG ( "", "Unown code: ", DEBUG_ONLINE_ERROR_CODE ); \
        } \
        throw; \
    } }

#endif