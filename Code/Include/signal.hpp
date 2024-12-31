#ifndef SYGNAL
#define SYGNAL

#include <unordered_map>
#include <string>

enum signal_type {
    S_INT,
    S_FLOAT,
    S_STRING,
    S_BOOL,
    S_ERROR
};
namespace ReKat {
namespace sygnal {
    typedef struct {
        signal_type type;
        union {
            int idata;
            float fdata;
            char sdata[16]; // this i because std::string has not trivial deconstructor
            bool bdata;
        };
        
    } signal;
    std::unordered_map < std::string, signal > signals;
    
    void SetSignal ( std::string name, signal sign ) {
        if ( signals.find ( name ) == signals.end() ) 
        { signals[name] = sign; return; }
        signals.insert ( { name, sign } );
    }
    
    signal GetSignal ( std::string name ) {
        if ( signals.find ( name ) == signals.end() ) 
        { return {S_ERROR,0}; }
        return signals[name];
    }

} // namespace sygnal
} // namespace ReKat


#endif