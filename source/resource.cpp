#include "extensions/resource.hpp"

Resource::Resource ( void ) { }
Resource::~Resource ( void ) { }
int Resource::Make ( void ) { return 0; }
void Resource::End ( void ) { }
void Resource::Use ( void ) { }

namespace Manager
{
	std::unordered_map < std::string, std::shared_ptr < Resource > > _resources;

	template < class R > 
	extern inline std::shared_ptr < R > Get
	( std::string name )
	{
		if ( name == "" ) { return nullptr; }
		
		auto findit = _resources.find ( name + std::string ( typeid(R).name ( ) ) );
		
		if ( findit != _resources.end ( ) )
		{ return std::static_pointer_cast < R > ( findit->second ); }
		else
		{ return nullptr; }
	}

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
