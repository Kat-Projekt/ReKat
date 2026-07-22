#include "extensions/resource.hpp"

Resource::Resource ( void ) { }
Resource::~Resource ( void ) { }
int Resource::Make ( void ) { return 0; }
void Resource::End ( void ) { }
void Resource::Use ( void ) { }
void Resource::Use ( float ) { }

namespace Manager
{
	std::unordered_map < std::string, std::shared_ptr < Resource > > _resources;

	extern void Free 
	( void ) 
	{
        	DEBUG ( 3, u8"╔══════════════════╗" );
       		DEBUG ( 3, u8"║ FREEING RESURCES ║" );
        	DEBUG ( 3, u8"╚══════════════════╝" );
        	for ( auto R : _resources ) 
		{ R.second->End(); } 
	}
}
