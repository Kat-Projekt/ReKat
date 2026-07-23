#include "objekt/objekt.hpp"

Objekt::Objekt
( void )
{
	DEBUG ( 4, "Inizializing void Objekt" );
}

Objekt::Objekt (
	std::string name,
	vec3 pos,
	vec3 size,
	vec3 rot_pivot
) 
:
	_name(name),
	_transform (pos,size,rot_pivot)
{
	DEBUG ( 4,
		"Inizializing Objekt: ", name,
		", pos: ", pos, 
		", size: ", size,
		// ", rot: ", rot,
		", rot_pivot: ", rot_pivot
	);
}


void Objekt::Delete
( )
{
	DEBUG ( 4, "Deleting Objekt", Get_Name ( ) );

	for ( auto C : _children )  {
		C->Delete( );
	}
}

/* Deconstructor */
Objekt::~Objekt
( void ) { Delete ( ); }

void Objekt::Set_Father
( std::shared_ptr < Objekt > father )
{
	DEBUG ( 4,"linking father: ", father->Get_Name (), " to: ", Get_Name ( ) );
	this->_father = father;
}
std::shared_ptr < Objekt > Objekt::Get_Father
( ) const 
{ return this->_father; }

Objekt& Objekt::Add_Child
( std::shared_ptr < Objekt > child )
{
	DEBUG ( 5,"adding child: ", child->Get_Name (), " to: ", Get_Name ( ) );
	child->Set_Father ( shared_from_this ( ) );
	this->_children.append ( child );
	if ( _started ) { child->Start( ); }
	return *this;
}
Objekt& Objekt::Rem_Child
( std::string name )
{
	auto O = Get_Child ( name );
	if ( O == nullptr )
	{ DEBUG ( 3, "child", name, "not found"); return *this; }
	this->_children.remove ( O );
	DEBUG ( 4, "removing child: ", O->Get_Name (), " from: ", Get_Name ( ) );
	return *this;
}
Objekt& Objekt::Rem_Child
( std::shared_ptr < Objekt > O )
{
	this->_children.remove ( O );
	DEBUG ( 3,"\tremoving child: ", O->Get_Name (), " from: ", this->Get_Name ( ) );
	return *this;
}

std::shared_ptr < Objekt > Objekt::Get_Child
( std::string name ) 
{
	for ( auto C : _children )  {
		if ( C->Get_Name() == name ) 
		{ return C; }
	}
	return nullptr;
}

bool Objekt::Has_Child
( std::shared_ptr < Objekt > child )
{
	for ( auto C : _children )  {
		if ( C == child ) 
		{ return true; }
		if ( C->Has_Child ( child ) ) { return true; }
	}
	return false;
}

List < std::shared_ptr < Objekt > > & Objekt::Get_Children
( ) { return _children; }

// recursverly counts childrens
int Objekt::Count_Children
( ) {
	int count = 0;
	for ( auto C : _children ) {
		count ++;
		count += C->Count_Children ( );
	}
	return count;
}

Objekt& Objekt::Set_Active
( bool active )
{
	_active = active;
	
	if ( !_started && active )
	{ Start ( ); }
	
	for ( auto child : _children )
	{ child->Set_Active ( active ); }
	
	return *this;
}

Objekt& Objekt::Set_Name
( std::string name )
{ _name = name; return *this; }

bool Objekt::Get_Active
( ) const
{ return _active; }

std::string Objekt::Get_Name
( ) const
{ return _name; }


template < typename Component_Call, typename Objekt_Call >
void Objekt::Recursive_Caller
(
	std::string ind,
	std::string message,
	Component_Call component_call,
	Objekt_Call object_call
)
{
	if ( !_active )
	{ return; }
	(void)message; // to prevent warngin when not debugging

	if ( _components.size ( ) != 0 )
	{
		DEBUG ( 4, message, " components:" );
		for ( auto &&C : _components )
		{
			DEBUG ( 5, message, " component: ", C->Get_Type ( ) );
			component_call ( C );
		}
	}

	if ( _children.size ( ) != 0 )
	{
		DEBUG ( 4, message, " children:" );
		for ( auto &&O : _children )
		{
			DEBUG ( 5, message, " child: ", O->Get_Name ( ) );
			object_call ( O, ind + "- " );
		}
	}
}

void Objekt::Start
( std::string ind )
{
	if ( _started ) { return; }
	Recursive_Caller (
		ind, "Starting",
		[] ( std::shared_ptr < Behaviour > C )
		{ C->_Start ( ); },
		[] ( std::shared_ptr < Objekt > O, std::string ind )
		{ O->Start ( ind ); }
	);
	_started = true;
}

void Objekt::Early_Update
( std::string ind )
{
	Recursive_Caller (
		ind, "Early Updating",
		[] ( std::shared_ptr < Behaviour > C )
		{ C->_Early_Update ( ); },
		[] ( std::shared_ptr < Objekt > O, std::string ind )
		{ O->Early_Update ( ind ); }
	);
}

void Objekt::Update
( std::string ind )
{
	Recursive_Caller (
		ind, "Updating",
		[] ( std::shared_ptr < Behaviour > C )
		{ C->_Update ( ); },
		[] ( std::shared_ptr < Objekt > O, std::string ind )
		{ O->Update ( ind ); }
	);
}

void Objekt::Late_Update
( std::string ind )
{
	Recursive_Caller (
		ind, "Late Updating",
		[] ( std::shared_ptr < Behaviour > C )
		{ C->_Late_Update ( ); },
		[] ( std::shared_ptr < Objekt > O, std::string ind )
		{ O->Late_Update ( ind ); }
	);
}

void Objekt::Fixed_Update
( std::string ind )
{
	Recursive_Caller (
		ind, "Fixed Updating",
		[] ( std::shared_ptr < Behaviour > C )
		{ C->_Fixed_Update ( ); },
		[] ( std::shared_ptr < Objekt > O, std::string ind )
		{ O->Fixed_Update ( ind ); }
	);
}

void Objekt::Handle_Collisions
( Objekt* collider, bool trigger, collision_type Type )
{
	for ( auto C : _components ) {
		if ( !trigger ) {
			switch ( Type ) {
				case collision_type::Stay: C->Collision ( collider ); break; // Stay 
				case collision_type::Enter: C->Collision_Enter ( collider ); break; // Enter 
				case collision_type::Exit: C->Collision_Exit ( collider ); break; // Exit
			}
			DEBUG ( 4, "calling ", C->Get_Type ( ) );
		} else {
			switch ( Type ) {
				case collision_type::Stay: C->Collision_Trigger ( collider ); break; // Stay 
				case collision_type::Enter: C->Collision_Trigger_Enter ( collider ); break; // Enter 
				case collision_type::Exit: C->Collision_Trigger_Exit ( collider ); break; // Exit
			}
		}
	}

	DEBUG ( 5, "A collision of type: ", Type, ( trigger ? "(trigger)" : "(solid)" ), 
			" between ", Get_Name ( ), " and ", collider->Get_Name () );
}

std::shared_ptr < Behaviour > Objekt::Add_Component
( std::string type )
{
	DEBUG ( 3, "Requiring a special component", type );
	std::shared_ptr < Behaviour > comp = Factory::Construct ( type );
	
	if ( comp )
	{
		comp->obj = this;
		_components.append ( comp );
		if ( _started ) { comp->_Start( ); }
		return comp;
	}

	return nullptr;
}

std::shared_ptr < Behaviour > Objekt::Get_Component
( std::string type )
{
	DEBUG ( 5, "finding component: ", type );
	for ( auto c : _components ) 
	{
		if ( ( c->Get_Type ( ) == type ) )
		{ DEBUG ( 5, "found component: ", type ); return c; }
	}
	
	return nullptr;
}

List < std::shared_ptr < Behaviour > > Objekt::Get_Component_Recursive ( std::string type )
{
	List < std::shared_ptr < Behaviour > > L;
	for ( auto c : _components ) 
	{
		if ( c->Get_Type ( ) == type )
		{ L.append ( c ); }
	}

	for ( auto O : _children ) {
		auto Data = O->Get_Component_Recursive ( type );
		L.append ( Data );
	}
	
	return L;
}

bool Objekt::Has_Component ( std::string type )
{
	for ( auto c : _components ) 
	{
		if ( c->Get_Type ( ) == type )
		{ return true; }
	}
	return false;
}


Transform& Objekt::Get_Transform
( )
{ return _transform; }

const Transform& Objekt::Get_Transform
( ) const
{ return _transform; }


Objekt& Objekt::Set_Pos
( vec3 pos )
{ _transform.Set_Pos ( pos ); return *this; }

Objekt& Objekt::Set_Pos
( float z )
{ _transform.Set_Pos ( z ); return *this; }

Objekt& Objekt::Inc_Pos
( vec3 pos )
{ _transform.Inc_Pos ( pos ); return *this; }

Objekt& Objekt::Set_Size
( vec3 size )
{ _transform.Set_Size ( size ); return *this; }

Objekt& Objekt::Set_Rot
( vec3 rot )
{ _transform.Set_Rot ( rot ); return *this; }

Objekt& Objekt::Set_2D_Rot
( float rot )
{ _transform.Set_2D_Rot ( rot ); return *this; }

Objekt& Objekt::Set_Rot_Pivot
( vec3 rot_pivot )
{ _transform.Set_Rot_Pivot ( rot_pivot ); return *this; }


const vec3 Objekt::Get_Pos
( ) const
{ return _transform.Get_Pos ( ); }

const vec3 Objekt::Get_Size
( ) const
{ return _transform.Get_Size ( ); }

const vec3 Objekt::Get_Rot
( ) const
{ return _transform.Get_Rot ( ); }

const vec3 Objekt::Get_Rot_Pivot
( ) const
{ return _transform.Get_Rot_Pivot ( ); }

const Transform::mono_axis_rotation Objekt::Get_Rot_Mono
( ) const
{ return _transform.Get_Rot_Mono ( ); }

mat4 Objekt::Get_Model_Mat
( )
{ return _transform.Get_Model_Mat ( ); }


vec3* Objekt::Expose_Pos
( )
{ return & ( _transform.Expose_Pos ( ) ); }


void Objekt::Print_Tree
( std::string level )
{
	DEBUG ( 4, level, Get_Name ( ) );
	level += "- ";
	for ( auto C : _children )
	{ C->Print_Tree ( level ); }
}

std::ostream & operator <<
( std::ostream & os, Objekt & n )
{
	os << ( n._active ? "[ready] " : "[sleep] " ) << n.Get_Name ( );
	if ( n._components.size ( ) == 0 )
	{
		return os;
	}
	os << " < ";
	// print components
	for ( auto C : n._components ) 
	{ os << ( C->Get_Active () ? "[+]" : "[-]") << C->Get_Type ( ) << " "; }

	os << ">";
	
	return os;
}

std::ostream & operator ,
( std::ostream & out, Objekt & n )
{ out << n; return out; }