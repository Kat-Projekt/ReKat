#include "objekt/transform.hpp"

Transform::Transform
( vec3 pos, vec3 size, vec3 rot_pivot )
: _pos(pos), _exposable_pos(pos), _size(size), _rot_pivot(rot_pivot)
{ }


Transform& Transform::Set_Father
( std::shared_ptr < Transform > father )
{
	_father = father;
	_recalcutate = true;
	return *this;
}

std::shared_ptr < Transform > Transform::Get_Father
( ) const
{ return _father; }

Transform& Transform::Set_Pos
( vec3 pos )
{ _pos = pos; _exposable_pos = _pos; _recalcutate = true; return *this; }

Transform& Transform::Set_Pos
( float z )
{ _pos.z = z;_exposable_pos.z = _pos.z; _recalcutate = true; return *this; }

Transform& Transform::Inc_Pos
( vec3 pos )
{ _pos += pos; _exposable_pos = _pos; _recalcutate = true; return *this; }

Transform& Transform::Set_Size
( vec3 size )
{ _size = size; _recalcutate = true; return *this; }

Transform& Transform::Set_Rot
( vec3 rot )
{
	if ( rot == vec3 { 0, 0, 0 } )
	{
		_rot = { 1, 0, 0, 0 };
		_recalcutate = true;
		return *this;
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

	return *this;
}

Transform& Transform::Set_Rot_Pivot
( vec3 rot_pivot )
{ _rot_pivot = rot_pivot; _recalcutate = true; return *this; }

Transform& Transform::Set_2D_Rot
( float rot )
{
	_rot_z = rot;
	_exposable_rot_z = rot;

	_recalcutate = true;

	return *this;
}

const vec3 Transform::Get_Pos
( ) const
{ return _pos + ( _father != nullptr ? _father->Get_Pos ( ) : vec3 { 0, 0, 0 } ); }

vec3& Transform::Expose_Pos
( )
{ return _exposable_pos; }

float& Transform::Expose_2D_Rot
( )
{ return _exposable_rot_z; }

const vec3 Transform::Get_Size
( ) const
{ return _size; }

const vec3 Transform::Get_Rot_Pivot
( ) const
{ return _rot_pivot; }

const vec3 Transform::Get_Rot
( ) const
{
	// method from https://danceswithcode.net/engineeringnotes/quaternions/quaternions.html

	double pitch = asin ( 2 * (_rot.w * _rot.y + _rot.z * _rot.x) );
	double roll, yaw;

	if ( pitch == M_PI_2 || pitch == -M_PI_2 )
	{
		roll = 0;
		yaw = - sign ( pitch ) * 2 * atan2 ( _rot.x, _rot.w );
	}
	else
	{
		roll = atan2 (
			2 * (_rot.w * _rot.x + _rot.y * _rot.z),
			pow ( _rot.w, 2 ) - pow ( _rot.x, 2 ) - pow ( _rot.y, 2 ) + pow ( _rot.z, 2 )
		);

		yaw = atan2 (
			2 * (_rot.w * _rot.z + _rot.y * _rot.x),
			pow ( _rot.w, 2 ) + pow ( _rot.x, 2 ) - pow ( _rot.y, 2 ) - pow ( _rot.z, 2 )
		);
	}

	return vec3 { roll, pitch, yaw };
}

float Transform::Get_2D_Rot
( ) const
{ return _rot_z + ( _father != nullptr ? _father->Get_2D_Rot ( ) : 0 ); }

Transform::mono_axis_rotation Transform::Get_Rot_Mono
( ) const
{
	mono_axis_rotation rot;

	if ( _rot.w == 1 )
	{
		rot.angle = 0;
		rot.axis = { 1, 0, 0 };
	}
	else
	{
		rot.angle = 2 * acos ( _rot.w );
		float st = sin ( rot.angle / 2 );

		rot.axis = {
			_rot.x / st,
			_rot.y / st,
			_rot.z / st
		};
	}

	return rot;
}

mat4 Transform::Get_Model_Mat
( )
{
	if ( _father != nullptr && _father_pos != _father->Get_Pos ( ) )
	{
		_father_pos = _father->Get_Pos ( );
		_recalcutate = true;
	}
	if ( _pos != _exposable_pos )
	{
		_pos = _exposable_pos;
		_recalcutate = true;
	}
	if ( _rot_z != _exposable_rot_z )
	{
		_rot_z = _exposable_rot_z;
		_recalcutate = true;
	}
	if ( _recalcutate )
	{
		float rot = Get_2D_Rot ( );
		_rot.w = cos ( rot / 2 );
		_rot.x = 0;
		_rot.y = 0;
		_rot.z = sin ( rot / 2 );

		// this is for centering during render trust me bro i know ( reduces 1 transation => good ) 
		vec3 Pivot = { (_rot_pivot.x + 0.5) * _size.x, (_rot_pivot.y + 0.5) * _size.y, (_rot_pivot.z) * _size.z };
		vec3 position = Get_Pos ( );

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

std::ostream & operator <<
( std::ostream & os, Transform & n )
{
	Transform::mono_axis_rotation mono = n.Get_Rot_Mono ( );
	os	<< n.Get_Pos ( ) << ", " 
		<< n.Get_Size ( )
		<< ", " << n.Get_Rot ( )
		<< ", {" << mono.angle << ":" << mono.axis << "}";
	return os;
}

std::ostream & operator ,
( std::ostream & out, Transform & n )
{ out << n; return out; }