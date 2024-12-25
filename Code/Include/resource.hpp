#ifndef RESOURCE
#define RESOURCE

#include "debugger.hpp"
#include <unordered_map>
#include <string>

class Resource {
public:
    Resource( void ) { }
    virtual int Make() { return 0; }
    virtual void End ( void ) { }
    virtual void Use ( void ) { }
    virtual void Use ( float ) { }
};

namespace Manager {
    static std::unordered_map < std::string, Resource* > _resources;

    // Get a resource
    // --------------
    template < class R > 
    inline static R* Get ( std::string name ) {
		if ( name == "" ) { return nullptr; }
		auto findit = _resources.find(name + std::string(typeid(R).name()));
		if ( findit != _resources.end() ) {
			return (R*)findit->second;
		} else { return nullptr; }
    }

	// Makes the resources disapear
	// ----------------------------
    static void Free ( ) {
        DEBUG ( 3, u8"╔══════════════════╗" );
        DEBUG ( 3, u8"║ FREEING RESURCES ║" );
        DEBUG ( 3, u8"╚══════════════════╝" );
        for ( auto R : _resources ) { R.second->End(); } }
    
} // namespace Manager


#endif