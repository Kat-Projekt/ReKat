#ifndef DEBUG_SYNTH
#define DEBUG_SYNTH

#include "../debugger.hpp"
#define AL_LIBTYPE_STATIC
#include <AL/al.h>
#include <AL/alc.h>

int DEBUG_SYNTH_ERROR_CODE; // Global Synth error code

#define AL_CHECK_ERROR \
	{ DEBUG_SYNTH_ERROR_CODE = alGetError ( );      \
	if ( DEBUG_SYNTH_ERROR_CODE != AL_NO_ERROR ) {\
        color ( "ERROR_A", FOREGROUND_INTENSITY | FOREGROUND_RED ); \
        switch ( DEBUG_SYNTH_ERROR_CODE ) { \
            case 0xA001: __DEBUG (  "", "AL_INVALID_NAME" ); break; \
            case 0xA002: __DEBUG (  "", "AL_INVALID_ENUM" ); break; \
            case 0xA003: __DEBUG (  "", "AL_INVALID_VALUE" ); break; \
            case 0xA004: __DEBUG (  "", "AL_INVALID_OPERATION" ); break; \
            case 0xA005: __DEBUG (  "", "AL_OUT_OF_MEMORY" ); break; \
            default: __DEBUG ( "", "Unown code: ", DEBUG_SYNTH_ERROR_CODE ); \
        } \
        throw; \
    } }

#endif