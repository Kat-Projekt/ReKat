#include <_objekt_implentation_details/component_manager.h>

std::size_t Factory::_Register
( Factory::componentLibraryFunctions&& comp )
{
	// create alises
	std::string formatted_name =
		  std::string ( comp.metadata.name ) 
		+ std::string ( "-" )
		+ comp.metadata.version.To_String ( );
	std::string latest_name =
		  std::string ( comp.metadata.name ) 
		+ std::string ( "-latest" );
	std::string stable_name =
		  std::string ( comp.metadata.name ) 
		+ std::string ( "-stable" );
	std::string comp_name = 
		std::string ( comp.metadata.name );
	
	// duplicate version number
	if ( constructors.find ( formatted_name ) != constructors.end ( ) )
	{
		DEBUG ( DebugLevel::NOTICE, "Duplicate found: ", formatted_name, " skipping" );
		return 0;
	} else {
		// for uniqueness
		constructors [ formatted_name ] = std::move ( comp );
		aliases [ formatted_name ] = formatted_name;
	}
	// is this the first insertion?
	if ( aliases.find ( comp_name ) != aliases.end ( ) )
	{
		DEBUG ( DebugLevel::NOTICE, "First component of type: ", comp_name );
		aliases [ formatted_name ] = formatted_name;
		aliases [ comp_name ] = formatted_name;
		aliases [ stable_name ] = formatted_name;
		aliases [ latest_name ] = formatted_name;
		return 1;
	}
	// new latest version?
	if (
		comp.metadata.version
		> constructors [ aliases [ latest_name ] ].metadata.version
	)
	{
		DEBUG ( DebugLevel::NOTICE, "New latest version of: ", comp_name );
		aliases [ latest_name ] = formatted_name;
	}

	// new latest stable version?
	if (
		comp.metadata.version
		> constructors [ aliases [ latest_name ] ].metadata.version
		&& comp.metadata.version.stable
	)
	{
		DEBUG ( DebugLevel::NOTICE, "New stable version of: ", comp_name );
		aliases [ stable_name ] = formatted_name;
		aliases [ comp_name ] = formatted_name;
	}

	return 1;
}

std::size_t Factory::Register_Directory
( const boost::dll::fs::path& path )
{
	if ( !boost::filesystem::is_directory ( path ) )
	{
		DEBUG ( DebugLevel::WARN, "Component directory not found: ", path );
		return 0;
	}

	int registered_components = 0;
	for ( auto fd : boost::filesystem::recursive_directory_iterator ( path ) )
	{
		if ( fd.is_regular_file ( ) )
		{ registered_components += Register ( fd.path ( ) ); }
	}

	return registered_components;
}

std::size_t Factory::Register
( const boost::dll::fs::path& path )
{
	// get component lib
	componentLibraryFunctions comp;
	try
	{
		comp.lifeline = std::make_unique 
			< boost::dll::shared_library > 
				( path, boost::dll::load_mode::append_decorations );

		comp.constructor = comp.lifeline->get
			< constructor_t > ( "_factory" );
		comp.constructor = comp.lifeline->get
			< constructor_t > ( "_destroyer" );

		comp.metadata = comp.lifeline->get
			< ComponentMetadata& ( * ) ( ) > ( "_metadata" ) ( );
	}
	catch(const std::exception& e)
	{
		DEBUG ( DebugLevel::WARN, "Error loading component: ", path, " ", e );
		return 0;
	}

	return _Register ( std::move ( comp ) );
}

std::unique_ptr < Behaviour, Factory::deconstructor_t >
Factory::Construct (
	const std::string& name,
	bool stable = true,
	ComponentVersion version = 0
) {
	// find version name
	std::string true_name = name;
	if ( version == 0 )
	{
		if ( stable )
		{ true_name += "-stable"; }
		else
		{ true_name += "-latest"; }

		true_name = aliases [ true_name ];
	} else {
		true_name += version.To_String ( );
	}

	if ( constructors.find ( true_name ) == constructors.end ( ) )
	{
		DEBUG ( DebugLevel::ERROR, "This specific version cannot be found: ", true_name );
		return std::unique_ptr < Behaviour, Factory::deconstructor_t > ( nullptr, nullptr );
	}
	
	DEBUG ( DebugLevel::NOTICE, "Trying to construct component: ", true_name );

	return std::unique_ptr < Behaviour, Factory::deconstructor_t > (
		constructors [ true_name ].constructor ( ),
		constructors [ true_name ].deconstructor
	);
}
