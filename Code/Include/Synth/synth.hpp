#ifndef SYNTH
#define SYNTH

// include openal libs
#include <iostream>

// per Gatone
/* Guardati openal-soft.org
 * Ho gia incluso la libreria in Code/Lib/openal-soft
 * cerca di scrivere l'implementazione descritta sotto
 * Grazie
*/

namespace ReKat {
namespace synth {
    #define BUF_LEN 1024

    enum STATUS {
        SUCCESS,
        FAILED
    };

    /// @brief Starts engine (work with static varibles)
    static void Start ( );
    /// @brief Terminates angine and deallocates resources
    static void End ( );

    /// @brief plays a buffer (_buf) of lenth (_len)
    /// @param _buf 
    /// @param _len 
    /// @return status
    static int Play ( double* _buf, long _len = BUF_LEN );
} /* Synth */ } // ReKat

// implementation
namespace ReKat::synth {
    static void Start ( ) 
    { std::cout << "hello\n"; }
    static void End ( ) 
    { }

    static int Play ( double* _buf, long _len ) 
    { return SUCCESS; }
}


#endif