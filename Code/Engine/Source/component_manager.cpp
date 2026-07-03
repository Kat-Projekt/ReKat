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
	{ return nullptr; }

	return con->second ( );
}

int Factory::Register ( const std::string& name, const std::string& path )
{
	DEBUG ( 3, "Creating new Factory" );
	boost::dll::fs::path lib_path ( path );

	auto factory = boost::dll::import_symbol
		 < std::shared_ptr < Behaviour > ( ) > (
			lib_path,
			"_Factory",
			boost::dll::load_mode::append_decorations
	);

	constructors [ name ] = factory;

	DEBUG ( 3, "Created new Factory ", name, " from file: ", path );

	return 0;
}