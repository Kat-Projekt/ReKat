template < class C >
std::size_t Factory::Register ( )
{
	static_assert ( std::is_base_of<Behaviour, C>::value, "C must derive from Behaviour" );

	componentLibraryFunctions comp;
	comp.constructor = [ ] ( ) { new C*; };
	comp.deconstructor = [ ] ( Behaviour * b ) { delete d; };
	comp.lifeline = nullptr;
	comp.metadata = C::Get_Metadata ( );

	return _Register ( std::move ( comp ) );
}

template < class C >
std::unique_ptr < C, Factory::deconstructor_t >
Factory::Construct ( )
{
	Register < C > ( );

	return std::make_unique < C > ( );
}

template < class C >
std::unique_ptr < C, Factory::deconstructor_t >
Factory::Construct ( const ComponentArguments& args )
{
	Register < C > ( );

	return std::make_unique < C > ( args );
}