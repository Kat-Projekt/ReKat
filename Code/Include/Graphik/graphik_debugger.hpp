#ifndef DEBUG_GRAPHIK
#define DEBUG_GRAPHIK

#include "../debugger.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int DEBUG_GRAPHIK_ERROR_CODE; // Global graphik error code

#define GL_CHECK_ERROR \
	{ DEBUG_GRAPHIK_ERROR_CODE = glGetError ( );      \
	if ( DEBUG_GRAPHIK_ERROR_CODE != GL_NO_ERROR ) {\
        color ( "ERROR_G", FOREGROUND_INTENSITY | FOREGROUND_RED ); \
        switch ( DEBUG_GRAPHIK_ERROR_CODE ) { \
            case 0x0500: __DEBUG (  "", "GL_INVALID_ENUM" ); break; \
            case 0x0501: __DEBUG (  "", "GL_INVALID_VALUE" ); break; \
            case 0x0502: __DEBUG (  "", "GL_INVALID_OPERATION" ); break; \
            case 0x0503: __DEBUG (  "", "GL_STACK_OVERFLOW" ); break; \
            case 0x0504: __DEBUG (  "", "GL_STACK_UNDERFLOW" ); break; \
            case 0x0505: __DEBUG (  "", "GL_OUT_OF_MEMORY" ); break; \
            case 0x0506: __DEBUG (  "", "GL_INVALID_FRAMEBUFFER_OPERATION" ); break; \
            default: __DEBUG ( "", "Unown code: ", DEBUG_GRAPHIK_ERROR_CODE ); \
        } \
        throw; \
    } }

#endif