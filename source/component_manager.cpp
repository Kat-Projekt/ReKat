#include "objekt/component_manager.hpp"

namespace Factory
{
	std::unordered_map
		<
			std::string,
			std::function<std::shared_ptr<Behaviour>()>
		> constructors;
}

std::shared_ptr<Behaviour> Factory::Construct
( const std::string& name )
{
	DEBUG ( 4, "Creating component", name );
	auto con = constructors.find ( name );

	if ( con == constructors.end ( ) )
	{ 
		DEBUG ( 1, "Cannot find component", name );
		return nullptr;
	}

	return con->second ( );
}

int Factory::Register ( const std::string& path )
{
	DEBUG ( 3, "Creating new Factory" );
	boost::dll::fs::path lib_path ( path );

	auto _factory = boost::dll::import_symbol < std::shared_ptr < Behaviour > ( ) > (
		lib_path,
		"_Factory",
		boost::dll::load_mode::append_decorations
	);

	Behaviour::Component_Metadata comp_info = _factory ( )->Get_Info ( );
	constructors [ comp_info.name ] = _factory;

	DEBUG ( 3, "Added ", comp_info.name, "::", comp_info.version, 
		" ( ", comp_info.description, " ) from ", path );

	return 0;
}