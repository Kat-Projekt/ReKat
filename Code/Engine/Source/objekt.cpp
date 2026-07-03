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
	_pos(pos),
	_size(size),
	_rot_pivot(rot_pivot) 
{
	DEBUG ( 4,
		"Inizializing Objekt: ", name,
		", pos: ", pos, 
		", size: ", size,
		// ", rot: ", rot,
		", rot_pivot: ", rot_pivot
	);
}


void Objekt::Free
( )
{
	DEBUG ( 4, "Deleting Objekt", Get_Name ( ) );
}

/* Deconstructor */
Objekt::~Objekt
( void ) { Free ( ); }

void Objekt::Set_Father
( std::shared_ptr < Objekt > father )
{
	DEBUG ( 4,"linking father: ", father->Get_Name (), " to: ", Get_Name ( ) );
	this->_father = father;
}
std::shared_ptr < Objekt > Objekt::Get_Father
( ) { return this->_father; }

std::shared_ptr < Objekt > Objekt::Add_Child
( std::shared_ptr < Objekt > child )
{
	DEBUG ( 5,"adding child: ", child->Get_Name (), " to: ", Get_Name ( ) );
	child->Set_Father ( shared_from_this ( ) );
	this->_childrens.append ( child );
	if ( _started ) { child->Start( ); }
	return shared_from_this ( );
}
std::shared_ptr < Objekt > Objekt::Rem_Child
( std::string name )
{
	auto O = Get_Children ( name );
	if ( O == nullptr )
	{ DEBUG ( 3, "child", name, "not found"); return shared_from_this ( ); }
	this->_childrens.remove ( O );
	DEBUG ( 4, "removing child: ", O->Get_Name (), " from: ", Get_Name ( ) );
	return shared_from_this ( );
}
std::shared_ptr < Objekt > Objekt::Rem_Child
( std::shared_ptr < Objekt > O )
{
	this->_childrens.remove ( O );
	DEBUG ( 3,"\tremoving child: ", O->Get_Name (), " from: ", this->Get_Name ( ) );
	return shared_from_this ( );
}

std::shared_ptr < Objekt > Objekt::Get_Children
( std::string name ) 
{
	for ( auto C : _childrens )  {
		if ( C->Get_Name() == name ) 
		{ return C; }
	}
	return nullptr;
}

bool Objekt::Has_Children
( std::shared_ptr < Objekt > child )
{
	for ( auto C : _childrens )  {
		if ( C == child ) 
		{ return true; }
		if ( C->Has_Children ( child ) ) { return true; }
	}
	return false;
}

List < std::shared_ptr < Objekt > > & Objekt::Get_Childrens 
( ) { return _childrens; }

// recursverly counts childrens
int Objekt::Count_Childrens
( ) {
	int count = 0;
	for ( auto C : _childrens ) {
		count ++;
		count += C->Count_Childrens ( );
	}
	return count;
}

std::shared_ptr < Objekt > Objekt::Set_Pos
( vec3 pos )
{ _pos = pos; _recalcutate = true; return shared_from_this ( ); }
std::shared_ptr < Objekt > Objekt::Set_Pos
( float z )
{ _pos.z = z; _recalcutate = true; return shared_from_this ( ); }
std::shared_ptr < Objekt > Objekt::Inc_Pos
( vec3 pos )
{ _pos += pos; _recalcutate = true; return shared_from_this ( ); }
std::shared_ptr < Objekt > Objekt::Set_Size
( vec3 size )
{ _size = size; _recalcutate = true; return shared_from_this ( ); }
std::shared_ptr < Objekt > Objekt::Set_Rot
( vec3 rot )
{
	if ( rot == vec3 { 0, 0, 0 } )
	{
		_rot = { 1, 0, 0, 0 }; 
		_recalcutate = true; 
		return shared_from_this ( );
	}

	double sz = sin ( rot.z / 2 );
	double cz = cos ( rot.z / 2 );
	double sy = sin ( rot.y / 2 );
	double cy = cos ( rot.y / 2 );
	double sx = sin ( rot.x / 2 );
	double cx = cos ( rot.x / 2 );

	// eq 10 from https://danceswithcode.net/engineeringnotes/quaternions/quaternions.html
	_rot.w = cx * cy * cz + sx * sy * sz;
	_rot.x = sx * cy * cz - cx * sy * sz;
	_rot.y = cx * sy * cz + sx * cy * sz;
	_rot.z = cx * cy * sz - sx * sy * cz;

	_recalcutate = true;

	return shared_from_this ( );
}
std::shared_ptr < Objekt > Objekt::Set_2D_Rot
( float rot )
{
	_rot.w = cos ( rot / 2 );
	_rot.x = 0;
	_rot.y = 0;
	_rot.z = sin ( rot / 2 );
	
	_recalcutate = true;
	
	return shared_from_this ( );
}
std::shared_ptr < Objekt > Objekt::Set_Active
( bool active )
{
	_active = active;
	
	if ( !_started && active )
	{ Start ( ); }
	
	for ( auto child : _childrens )
	{ child->Set_Active ( active ); }
	
	return shared_from_this ( );
}
std::shared_ptr < Objekt > Objekt::Set_Rot_Pivot
( vec3 rot_pivot )
{ _rot_pivot = rot_pivot; _recalcutate = true; return shared_from_this ( ); }
std::shared_ptr < Objekt > Objekt::Set_Name
( std::string name )
{ _name = name; return shared_from_this ( ); }

vec3 Objekt::Get_Pos
( )
{ return _pos + ( _father != nullptr ? _father->Get_Pos ( ) : vec3 { 0, 0, 0 } ); }
vec3& Objekt::Expose_Pos
( )
{ return _pos; }
vec3 Objekt::Get_Size
( )
{ return _size; }
vec3 Objekt::Get_Rot
( )
{
	// method from https://danceswithcode.net/engineeringnotes/quaternions/quaternions.html
	
	// safe to run every time
	double pitch = asin ( 2 * (_rot.w * _rot.y + _rot.z * _rot.x) );
	double roll, yaw;

	// gimbal lock check
	if ( pitch == M_PI_2 || pitch == - M_PI_2 )
	{
		roll = 0;
		yaw = - sign ( pitch ) * 2 * atan2 ( _rot.x, _rot.w );
	} 
	else // lock occured
	{
		roll  = atan2 ( 2 * (_rot.w * _rot.x + _rot.y * _rot.z)
					, pow (_rot.w,2) - pow (_rot.x,2) - pow (_rot.y,2) + pow (_rot.z,2) );
		yaw   = atan2 ( 2 * (_rot.w * _rot.z + _rot.y * _rot.x)
				, pow (_rot.w,2) + pow (_rot.x,2) - pow (_rot.y,2) - pow (_rot.z,2) );
	}

	return vec3 { roll,pitch,yaw };
}
vec3 Objekt::Get_Rot_Pivot
( )
{ return _rot_pivot; }
Objekt::mono_axis_rotation Objekt::Get_Rot_Mono
( )
{
	// convert quat to modo axis angle
	// using 6 form https://danceswithcode.net/engineeringnotes/quaternions/quaternions.html
	mono_axis_rotation rot;

	if ( _rot.w == 1 ) // no rotation
	{
		rot.angle = 0;
		rot.axis = {1,0,0};
	}
	else
	{
		rot.angle = 2 * acos ( _rot.w );
		float st = sin ( rot.angle / 2 ); // sin \theta
		rot.axis = { _rot.x / st, _rot.y / st, _rot.z / st };
	}

	return rot;
}
bool Objekt::Get_Active
( )
{ return _active; }
std::string Objekt::Get_Name
( )
{ return _name; }

void Objekt::Start
( std::string ind )
{
	if ( _started ) { return; }
	if ( !_active ) { return; }

	DEBUG ( 5, ind,"Starting Objekt: ", Get_Name ( ) );
	for ( auto &&C : _components ) {
		DEBUG ( 6, ind," - Starting Componenet: ", std::string(typeid(*(C)).name()) );
		C->_Start ( );
		DEBUG ( 6,"Done" );
		DEBUG ( 6,*this );
	}

	DEBUG ( 5, ind, "Starting Childrens of ", Get_Name ( ) );
	for ( auto &&O : _childrens ) 
	{ O->Start ( ind + "- "); }

	DEBUG ( 5,ind,"Started Objekt: ", Get_Name() );
	_started = true;
}

void Objekt::Update
( std::string ind )
{
	if ( !_active ) { return; }

	if ( _components.size ( ) != 0 )
	{
		DEBUG ( 5,ind,"Updating Components of: ", Get_Name ( ) );
		
		for ( auto &&C : _components ) {
			DEBUG ( 6,ind,"+ Updating Componenet: ", std::string(typeid(*C).name()));
			C->_Update ( );
		}
	}

	if ( _childrens.size ( ) != 0 )
	{
		DEBUG ( 5,ind,"Updating Childrens of: ", Get_Name ( ) );
		for ( auto &&O : _childrens )
		{ O->Update ( ind + "- " ); }
	}

	DEBUG ( 5,ind,"Updated Objekt: ", Get_Name ( ) );
}

void Objekt::Fixed_Update
( std::string ind )
{
	if ( !_active ) { return; }
	DEBUG ( 5,ind,"Updating Fixed Components of: ", Get_Name() );
	
	for ( auto C : _components ) {
		DEBUG ( 6,ind,"+ Updating Fixed Componenet: ", std::string(typeid(*C).name()));
		C->_Fixed_Update ( );
	}
	
	DEBUG ( 5,ind,"Updating Fixed Childrens of: ", Get_Name ( ) );
	for ( auto O : _childrens ) 
	{ O->Fixed_Update ( ind + "- "); }

	DEBUG ( 5,ind,"Updated Fixed Objekt: " + Get_Name() );
}

void Objekt::Handle_Collsions
( std::shared_ptr < Objekt > collider, float trigger, collision_type Type )
{
	for ( auto C : _components ) {
		if ( !trigger ) {
			switch ( Type ) {
				case 0: C->Collision ( collider ); break; // Stay 
				case 1: C->Collision_Enter ( collider ); break; // Enter 
				case 2: C->Collision_Exit ( collider ); break; // Exit
			}
			DEBUG ( 4, "calling ", typeid ( *C ).name ( ) );
		} else {
			switch ( Type ) {
				case 0: C->Collision_Trigger ( collider ); break; // Stay 
				case 1: C->Collision_Trigger_Enter ( collider ); break; // Enter 
				case 2: C->Collision_Trigger_Exit ( collider ); break; // Exit
			}
		}
	}

	DEBUG ( 5, "A collision of type: ", Type, ( trigger ? "(trigger)" : "(solid)" ), 
			" between ", Get_Name ( ), " and ", collider->Get_Name () );
}

mat4 Objekt::Get_Model_Mat
( )
{
	if ( _recalcutate )
	{
		// this is for centering during render trust me bro i know ( reduces 1 transation => good ) 
		vec3 Pivot = { (_rot_pivot.x + 0.5) * _size.x, (_rot_pivot.y + 0.5) * _size.y, (_rot_pivot.z) * _size.z };
		vec3 position = _pos;

		if ( _father != nullptr ) // get world postion recursively
		{ position+= _father->Get_Pos(); }

		_model = glm::mat4(1.0f);

		// apply rotation
		_model = glm::translate(_model, position);

		// _model = glm::rotate(_model, ang.angle, ang.axis);
		mat4 rotation_matrix = {
			{ 1 - 2 * pow ( _rot.y,2 ) - 2 * pow ( _rot.z,2 ), 2 * _rot.x * _rot.y - 2 * _rot.w * _rot.z, 2 * _rot.x * _rot.z + 2 * _rot.w * _rot.y, 0 },
			{ 2 * _rot.x * _rot.y + 2 * _rot.w * _rot.z, 1 - 2 * pow ( _rot.x,2 ) - 2 * pow ( _rot.z,2 ), 2 * _rot.y * _rot.z - 2 * _rot.w * _rot.x, 0 },
			{ 2 * _rot.x * _rot.z - 2 * _rot.w * _rot.y, 2 * _rot.y * _rot.z - 2 * _rot.w * _rot.x, 1 - 2 * pow ( _rot.x,2 ) - 2 * pow ( _rot.y,2 ), 0 },
			{0,0,0,1}
		};
		_model *= rotation_matrix;

		_model = glm::translate(_model, -Pivot);
		_model = glm::scale(_model, _size);

		_recalcutate = false;
		return _model;
	}
	else
	{ return _model; }
}

void Objekt::Print_Tree
( std::string level )
{
	DEBUG ( 4, level, Get_Name ( ) );
	level += "- ";
	for ( auto C : _childrens )
	{ C->Print_Tree ( level ); }
}

std::ostream & operator <<
( std::ostream & os, Objekt & n )
{
	os << n.Get_Name ( ) << " " << n._pos << " " << n._size << ( n._active ? " v" : " x" );
	if ( n._components.size ( ) == 0 )
	{
		return os;
	}
	os << " { ";
	// print components
	os << n._components.size ( ) << " ";
	for ( auto C : n._components ) 
	{
		assert(C); // this make sure C is not nullptr, it is for definition
		os << typeid(*C).name() << ( C->Get_Active () ? " v" : " x") << " ";
	}

	os << "}";
	
	return os;
}

std::ostream & operator ,
( std::ostream & out, Objekt & n )
{ out << n; return out; }

std::shared_ptr < Behaviour > Objekt::Add_Component_Special
( std::string type )
{
	DEBUG ( 3, "Requiring a special component", type );
	std::shared_ptr < Behaviour > comp = Factory::Construct ( type );
	
	if ( comp )
	{
		comp->obj = shared_from_this ( );
		_components.append ( comp );
		if ( _started ) { comp->_Start( ); }
		return comp;
	}

	return nullptr;
}

std::shared_ptr < Behaviour > Objekt::Get_Component
( std::string type )
{
	for ( auto c : _components ) 
	{
		if ( (typeid(*(c)).name ( ) == type ) )
		{ return c; }
	}
	
	return nullptr;
}

List < std::shared_ptr < Behaviour > > Objekt::Get_Component_Recursive ( std::string type )
{
	List < std::shared_ptr < Behaviour > > L;
	for ( auto c : _components ) 
	{
		if ( typeid ( *(c) ).name ( ) == type )
		{ L.append ( c ); }
	}

	for ( auto O : _childrens ) {
		auto Data = O->Get_Component_Recursive ( type );
		L.append ( Data );
	}
	
	return L;
}

bool Objekt::Has_Component ( std::string type )
{
	for ( auto c : _components ) 
	{
		if ( typeid(*(c)).name ( ) == type )
		{ return true; }
	}
	return false;
}