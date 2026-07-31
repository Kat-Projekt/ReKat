#pragma once

template < class C >
int Factory::Register ( )
{
	static_assert ( std::is_base_of<Behaviour, C>::value, "T must derive from Behaviour" );

	C name_get;

	std::string name = std::string ( name_get.Get_Type ( ) );
	constructors [ name ] = [ ] ( )
	{ return std::make_shared < C > ( ); };

	return 0;
}

template < class C >
std::shared_ptr<Behaviour> Factory::Construct ( )
{
	DEBUG ( 4, "Generating component" );
	static_assert ( std::is_base_of<Behaviour, C>::value, "C must derive from Behaviour" );

	if ( Register < C > ( ) )
	{ return nullptr; }

	return std::make_shared < C > ( );
}