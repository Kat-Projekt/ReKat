// --------------------------------------------------------
// ----------------- Components Managers  -----------------
// --------------------------------------------------------

template < class C >
C * Objekt::Add_Component ( const ComponentArguments& args )
{
	auto new_comp = Factory::Construct < C > ( );
	new_comp.Set ( args );
	if ( _started )
	{ new_comp->Start ( ); }

	auto new_comp_ptr = new_comp.get ( );
	_components.push_back ( std::move ( new_comp ) );
	return new_comp_ptr;
}

template < class C > 
C * Objekt::Add_Component
( std::unique_ptr < C > c )
{
	if ( _started )
	{ c->Start ( ); }
	
	auto c_ptr = c.get ( );
	_components.push_back ( std::move ( c ) );
	return c_ptr;
}


template < class C >
C * Objekt::Get_Component
( ) const
{
	auto comp_meta = C::Get_Metadata ( ).name;
	for ( auto& comp : _components )
	{
		if ( comp->Get_Metadata ( ).name == comp_meta )
		{ return (C*) comp.get ( ); }
	}
	return nullptr;
}


template < class C >
std::list < C * > Objekt::Get_Components
( ) const
{
	std::list < C* > pointers;
	auto comp_meta = C::Get_Metadata ( ).name;
	for ( auto& comp : _components )
	{
		if ( comp->Get_Metadata ( ).name == comp_meta )
		{ pointers.push_back ( (C*) comp.get ( ) ); }
	}
	return pointers;
}

template < class C >
std::list < C * > Objekt::Get_Component_Recursive
( ) const
{
	std::list < C* > all_pointers = Get_Components < C > ( );
	all_pointers.splice (
		all_pointers.end ( ),
		Get_Component_In_Children < C > ( )
	);

	return all_pointers;
}

template < class C >
std::list < C * > Objekt::Get_Component_In_Children
( ) const
{
	std::list < C* > all_pointers;

	for ( auto& child : _children )
	{
		all_pointers.splice (
			all_pointers.end ( ),
			child->Get_Component_Recursive < C > ( )
		);
	}

	return all_pointers;
}

template < class C >
std::unique_ptr < C > Objekt::Rem_Component ( )
{
	// find and pass ownership
	auto comp_meta = C::Get_Metadata ( ).name;
	for ( auto iter = _components.begin ( ); iter != _components.end ( ); iter ++ )
	{
		if ( (*iter)->Get_Metadata ( ).name == comp_meta )
		{
			auto comp = std::move ( *iter );
			_components.erase ( iter );

			return std::unique_ptr < C > (
				static_cast < C* > ( comp.release ( ) ) );
		}
	}
	return nullptr;
}

template < class C >
std::list < std::unique_ptr < C > > Objekt::Rem_Components ( )
{
	std::list < std::unique_ptr < C > > pointers;
	auto comp_meta = C::Get_Metadata ( ).name;
	for ( auto iter = _components.begin ( ); iter != _components.end ( ) )
	{
		if ( (*iter)->Get_Metadata ( ).name == comp_meta )
		{
			auto comp = std::move ( *iter );
			iter = _components.erase ( iter );

			pointers.push_back (
				std::unique_ptr < C > (
					static_cast < C* > (
						comp.release ( )
			) ) );
		} else {
			iter ++;
		}
	}

	return pointers;
}

template < class C >
bool Objekt::Has_Component
( ) const
{
	return Get_Component < C > ( ) != nullptr;
}

// ------------------------------------------------------
// ----------------- Behaviours Calling -----------------
// ------------------------------------------------------

template < typename Component_Call >
	void Recursive_Caller (
		const char* ind,
		const char* message,
		Component_Call component_call
	);
template < typename Component_Call >
	void Caller (
		const char* ind,
		const char* message,
		Component_Call component_call
	);