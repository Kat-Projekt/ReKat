#include <core/aktor.h>

void Aktor::Start ( )
{
	// every component hase been inizialized
	if ( _pending_behaviours.size ( ) == 0 )
	{ return; }

	// using a list with foreach iterator
	// makes sure that components added while
	// Starting are started
	for ( const auto& b : _pending_behaviours )
	{ b->_Start ( ); }

	_behaviours.splice ( _behaviours.end ( ), _pending_behaviours );
}

void Aktor::Frame_Start ( )
{
	// check if there are not started components
	if ( _pending_behaviours.size ( ) != 0 )
	{ Start ( ); }

	for ( const auto& b : _behaviours )
	{ b->_Frame_Start ( ); }
}

void Aktor::Update ( )
{
	for ( const auto& b : _behaviours )
	{ b->_Early_Update ( ); }
	for ( const auto& b : _behaviours )
	{ b->_Update ( ); }
	
	// recursion here

	for ( const auto& b : _behaviours )
	{ b->_Late_Update ( ); }
}

void Aktor::Frame_End ( )
{
	for ( const auto& b : _behaviours )
	{ b->_Frame_End ( ); }
}

Behaviour * Aktor::Add_Behaviour (
	std::string type,
	bool stable,
	uint64_t major,
	uint64_t minor,
	uint64_t pathc,
	bool start_behaviour
) {
	Behaviour * getted_ptr = nullptr;
	try {
		// this trows if the component is not present
		getted_ptr = Get_Behaviour ( type, stable, major, minor, pathc );
		DEBUG ( DebugLevel::WARN, "duplicate component, returning old: ", type );
		return getted_ptr;
	}
	catch ( std::runtime_error & )
	{
		_pending_behaviours.emplace_back ( Maestro::Construct ( type, stable, major, minor, pathc ) );
		getted_ptr = _pending_behaviours.back ( ).get ( );
		
		DEBUG ( DebugLevel::NOTICE, "adding new component ", type );
	}

	// Make sure that the new
	if ( start_behaviour )
	{ Start ( ); }
	else
	{ DEBUG ( DebugLevel::VERBOSE, "component added but not started" ); }

	return getted_ptr;
}

Behaviour * Aktor::Get_Behaviour (
	std::string type,
	bool stable,
	uint64_t major,
	uint64_t minor,
	uint64_t pathc
) {
	// if component meta is not present
	auto meta = Maestro::Get_Metadata ( type, stable, major, minor, pathc );
		
	for ( const auto& b : _behaviours )
	{
		if ( Is_Metadata_Equal ( b->Metadata ( ), meta ) )
		{
			DEBUG ( DebugLevel::NOTICE, "found component ", meta );
			return b.get ( );
		}
	}
	DEBUG ( DebugLevel::ERROR, "cannot find component ", meta );
	return nullptr;
}

void Aktor::Rem_Behaviour (
	std::string type,
	bool stable,
	uint64_t major,
	uint64_t minor,
	uint64_t pathc
) {
	auto meta = Maestro::Get_Metadata ( type, stable, major, minor, pathc );
	
	// using iterator istead of reference for erase
	for ( auto b = _behaviours.begin ( ); b != _behaviours.end ( ); b++ )
	{
		if ( Is_Metadata_Equal ( (*b)->Metadata ( ), meta ) )
		{
			_behaviours.erase ( b );
			DEBUG ( DebugLevel::NOTICE, "removed component ", meta );
			return;
		}
	}
	DEBUG ( DebugLevel::ERROR, "cannot find component ", meta );
}

