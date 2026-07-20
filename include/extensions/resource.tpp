namespace Manager {
	template < class R > 
	inline std::shared_ptr < R > Get
	( std::string name )
	{
		if ( name == "" ) { return nullptr; }
		
		auto findit = _resources.find ( Format_Name < R > ( name ) );
		
		if ( findit != _resources.end ( ) )
		{ return std::static_pointer_cast < R > ( findit->second ); }
		else
		{ return nullptr; }
	}

	template < class R >
	inline std::string Format_Name
	( std::string name )
	{
		static_assert ( std::is_base_of<Resource, R>::value, "R must derive from Resource" );
		return name + std::string ( typeid(R).name ( ) );
	}

	template < class R, typename ... Args>
	inline int Make
	( 
		std::string name,
		Args&&... args // forwarding references
	) {
		static_assert ( std::is_base_of<Resource, R>::value, "R must derive from Resource" );
		std::string _name = Format_Name < R > ( name );

		std::shared_ptr < R > _r = std::make_shared < R > ();
		_resources [ _name ] = _r;

		return _r->Make ( std::forward < Args > ( args ) ... );
	}

	template < class R >
	inline void Register
	( 
		std::string name,
		std::shared_ptr < R > resource
	) {
		static_assert ( std::is_base_of<Resource, R>::value, "R must derive from Resource" );
		std::string _name = Format_Name < R > ( name );

		_resources [ _name ] = resource;
	}
}