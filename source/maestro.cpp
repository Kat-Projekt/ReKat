#include <core/maestro.h>

std::size_t Maestro::_Register
( Maestro::componentLibraryFunctions comp )
{
	// create alises
	std::string formatted_name =
		  std::string ( comp.metadata.name )
		+ std::string ( "-" )
		+ std::string ( Reflection::Version_Number ( comp.metadata ) );
	std::string latest_name =
		  std::string ( comp.metadata.name )
		+ std::string ( "-latest" );
	std::string stable_name =
		  std::string ( comp.metadata.name )
		+ std::string ( "-stable" );

	DEBUG ( DebugLevel::INFO, "Registering component: ", formatted_name );

	// duplicate version number
	if ( _factories.find ( formatted_name ) != _factories.end ( ) )
	{
		DEBUG ( DebugLevel::TRACE, "Duplicate found: ", formatted_name, " skipping" );
		return 0;
	} else {
		// for uniqueness
		_factories.emplace ( formatted_name,
			std::forward < Maestro::componentLibraryFunctions > ( comp ) );
		_aliases [ formatted_name ] = formatted_name;
	}
	// is this the first insertion?
	if ( _aliases.find ( stable_name ) == _aliases.end ( ) )
	{
		DEBUG ( DebugLevel::NOTICE, "First component of type: ", formatted_name );
		_aliases [ formatted_name ] = formatted_name;
		_aliases [ stable_name ] = formatted_name;
		_aliases [ latest_name ] = formatted_name;
		return 1;
	}
	// new latest version?
	if ( Is_Version_Major ( _factories [ _aliases [ latest_name ] ].metadata, comp.metadata ) )
	{
		DEBUG ( DebugLevel::NOTICE, "New latest version of: ", formatted_name );
		_aliases [ latest_name ] = formatted_name;
	}

	// new latest stable version?
	if (
		comp.metadata.stable
		&&
		Is_Version_Major ( _factories [ _aliases [ stable_name ] ].metadata, comp.metadata ) 
	)
	{
		DEBUG ( DebugLevel::NOTICE, "New stable version of: ", formatted_name );
		_aliases [ stable_name ] = formatted_name;
	}

	return 1;
}

std::size_t Maestro::Register_Directory
( const boost::dll::fs::path& path )
{
	if ( !boost::filesystem::is_directory ( path ) )
	{
		DEBUG ( DebugLevel::WARN, "Component directory not found: ", path );
		return 0;
	}

	std::size_t registered_components = 0;
	for ( auto fd : boost::filesystem::recursive_directory_iterator ( path ) )
	{
		if ( fd.is_regular_file ( ) )
		{ registered_components += Register ( fd.path ( ) ); }
	}

	return registered_components;
}

std::size_t Maestro::Register
( const boost::dll::fs::path& path )
{
	// Check if file is a library
	std::string ext = path.extension().string();
	#ifdef _WIN32
		if (ext != ".dll" && ext != ".DLL") return 0;
	#elif __APPLE__
		if (ext != ".dylib" && ext != ".DYLIB") return 0;
	#else
		if (ext != ".so" && ext != ".SO") return 0;
	#endif

	// get component lib
	DEBUG ( DebugLevel::NOTICE, "Trying to load component: ", path );
	componentLibraryFunctions comp;
	comp.constructor = nullptr;
	comp.deconstructor = nullptr;
	try
	{
		// shared lib code
		comp.lifeline = std::make_shared
			< boost::dll::shared_library >
				( path, boost::dll::load_mode::append_decorations );
		if ( !comp.lifeline->is_loaded ( ) )
		{ throw std::runtime_error ( "loading error" ); }
		
		// check for symbols presence
		if ( !comp.lifeline->has ( "_Construct") )
		{ throw std::runtime_error ( "Missing symbol: _Construct" ); }
		if ( !comp.lifeline->has ( "_Deconstruct") )
		{ throw std::runtime_error ( "Missing symbol: _Deconstruct" ); }
		if ( !comp.lifeline->has ( "_Metadata") )
		{ throw std::runtime_error ( "Missing symbol: _Metadata" ); }

		// both parts of the unique_ptr
		comp.constructor = boost::dll::import_symbol < Behaviour * ( void ) > (
			*( comp.lifeline ),
			"_Construct" );
			
		comp.deconstructor = boost::dll::import_symbol < void ( Behaviour * ) > (
			*( comp.lifeline ),
			"_Deconstruct" );

		// get metadata
		comp.metadata = comp.lifeline->get
			< Reflection::Metadata > ( "_Metadata" );

		// safety check
		if ( !comp.constructor || !comp.deconstructor )
		{
			DEBUG ( DebugLevel::WARN, "Function pointers not loaded correctly" );
			return 0;
		}
	}
	catch(const std::exception& e)
	{
		DEBUG ( DebugLevel::WARN, "Error loading component '", path, "': ", e.what ( ) );
		return 0;
	}
	return _Register ( comp );
}

Maestro::uniqueBehaviour
Maestro::Construct (
	const std::string& name,
	bool stable,
	uint64_t major,	
	uint64_t minor,
	uint64_t patch
) {
	// for consistence in version numbering
	auto _place_holder_meta = Reflection::Construct ( name.c_str ( ), "__", major, minor, patch );
	
	// find version name
	std::string true_name = name + "-";
	
	if ( major == 0 && minor == 0 && patch == 0 )
	{
		if ( stable )
		{ true_name += "stable"; }
		else
		{ true_name += "latest"; }
		DEBUG ( DebugLevel::NOTICE, "Pre get: ", true_name );
		true_name = _aliases [ true_name ];
	} else {
		true_name += Reflection::Version_Number ( _place_holder_meta );
	}

	DEBUG ( DebugLevel::INFO, "Requested to build component: ", true_name );

	auto factory = _factories.find ( true_name );
	if ( factory == _factories.end ( ) )
	{
		DEBUG ( DebugLevel::ERROR, "This specific version cannot be found: ", true_name );
		return uniqueBehaviour ( nullptr, nullptr );
	}
	
	DEBUG ( DebugLevel::NOTICE, "Constructing component: ", factory->second.metadata );

	Behaviour* new_component = nullptr;
	try 
	{ new_component = factory->second.constructor ( ); }
	catch ( const std::exception& e )
	{
		DEBUG ( DebugLevel::WARN, "Error clarification: ", e.what ( ) );
		DEBUG ( DebugLevel::ERROR, "Error during component ", factory->second.metadata, " construction" );
	}

	return uniqueBehaviour (
		new_component,
		factory->second.deconstructor
	);
}

std::vector < Reflection::Metadata >
Maestro::Get_Registered_Components ( )
{
	std::vector < Reflection::Metadata > v;
	for ( const auto & iter : _factories )
	{ v.push_back ( iter.second.metadata ); }
	return v;
}
