#pragma once

template < class C > 
std::shared_ptr < C > Objekt::Add_Component ( )
{
	std::shared_ptr < Behaviour > comp = Factory::Construct < C > ( );
	
	if ( comp )
	{
		comp->obj = shared_from_this ( );
		_components.append ( comp );
		if ( _started ) { comp->_Start( ); }
		return std::static_pointer_cast < C > ( comp );
	}

	return nullptr;
}
template < class C >
std::shared_ptr < C > Objekt::Add_Component
( std::shared_ptr < C > c )
{
	static_assert ( std::is_base_of<Behaviour, C>::value, "C must derive from Behaviour" );

	c->obj = shared_from_this ( );
	_components.append ( std::static_pointer_cast < Behaviour > ( c ) );
	if ( _started ) { c->_Start( ); }

	return c;
}

template < class C > 
std::shared_ptr < C > Objekt::Get_Component ( )
{
	std::shared_ptr < Behaviour > p = Get_Component ( typeid ( C ).name ( ) );
	if ( p )
	{ return std::static_pointer_cast < C > ( p ); }
	else
	{ return nullptr; }
}
template < class C > 
List < std::shared_ptr < C > > Objekt::Get_Component_Recursive ( )
{
	List < std::shared_ptr < C > > L;
	for ( auto c : _components ) 
	{
		std::shared_ptr < C > recast = std::dynamic_pointer_cast < C > ( c );
		if ( recast )
		{ L.append ( recast ); }
	}

	for ( auto O : _childrens ) {
		auto Data = O->Get_Component_Recursive < C > ( );
		L.append ( Data );
	}

	return L;
}

template < class C > 
bool Objekt::Has_Component ( )
{
	return Has_Component ( typeid (C).name ( ) );
}
