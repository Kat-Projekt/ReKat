#define DIAGNOSTIC
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

int Factory::Register
( const boost::dll::fs::path& lib_path )
{
	std::function < std::shared_ptr < Behaviour > ( ) > _factory;
	try
	{
		_factory = boost::dll::import_symbol < std::shared_ptr < Behaviour > ( ) > (
			lib_path,
			"_Factory",
			boost::dll::load_mode::append_decorations
		);
	}
	catch(const std::exception& e)
	{
		DEBUG ( 3, "CANNOT REGISTER COMPONENT: ", lib_path );
		return 1;
	}

	Behaviour::Component_Metadata comp_info = _factory ( )->Get_Info ( );
	constructors [ comp_info.name ] = _factory;

	DEBUG ( 5, "Added ", comp_info.name, "::", comp_info.version, 
		" ( ", comp_info.description, " ) from ", lib_path );

	return 0;
}

int Factory::Register_Directory
( const std::string& path )
{
	if ( !boost::filesystem::is_directory ( path ) )
	{
		DEBUG ( 2, "Component directory does not exist: ", path );
		return -1;
	}

	int components = 0;
	for ( auto file : boost::filesystem::recursive_directory_iterator ( path ) )
	{
		const boost::filesystem::path file_path = file.path ( );

		int result = Register ( file_path );
		components += 1 - result;
	}

	return components;
}
