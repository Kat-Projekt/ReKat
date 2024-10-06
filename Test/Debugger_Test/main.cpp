#define DIAGNOSTIC
#include <debugger.hpp>
#include <graphik\graphik_debugger.hpp>

int main ( ) {
    DEBUG ( 2, "livello " );
    DEBUG ( 3, "livello " );
    DEBUG ( 4, "livello " );
    DEBUG ( 5, "livello " );
    DEBUG ( 6, "livello " );
    // DEBUG ( 1, "livello " );
    // DEBUG ( 0, "livello " );
    
    GL_CHECK_ERROR;
}