#include "_objekt_implentation_details/transform.h"

const glm::vec3 Transform::Get_Pos
( ) const noexcept
{
	if ( !_father )
	{ return _pos; }
	else
	// homogeneous coordinate is guaranteed to remain 1
	{ return glm::vec3 ( _father->Get_Model_Mat ( ) * glm::vec4 { _pos, 1 } ); }
}

const Transform::mono_axis_rotation Transform::Get_Rot_Mono
( ) const noexcept
{
	mono_axis_rotation rot;

	rot.angle = glm::angle ( _rot );

	// account for floating point error
	if ( glm::abs ( rot.angle ) <= 0.0001f )
	{
		rot.angle = 0.0f;
		rot.axis = glm::vec3 {0,0,1};
	}
	else
	{
		rot.angle = glm::degrees ( rot.angle );
		rot.axis = glm::axis ( _rot );
	}

	return rot;
}

const glm::mat4& Transform::Get_Local_Mat
( ) const noexcept
{
	if ( ! Is_Modified ( ) )
	{ return _local_model; }

	// this is for centering during render trust me bro i know ( reduces 1 transation => good ) 
	glm::vec3 pivot = {
		( _rot_pivot.x + 0.5f ) * _size.x,
		( _rot_pivot.y + 0.5f ) * _size.y,
		( _rot_pivot.z + 0.5f ) * _size.z
	};
	glm::vec3 position = Get_Local_Pos ( );

	_local_model = glm::mat4 ( 1.0f );

	_local_model = glm::translate ( _local_model, position );
	_local_model *= glm::mat4_cast ( _rot );
	_local_model = glm::translate ( _local_model, -pivot );
	_local_model = glm::scale ( _local_model, _size );

	Reset_Modify ( );
	return _local_model;
}

std::ostream & operator <<
( std::ostream & os, const Transform & n )
{
	Transform::mono_axis_rotation mono = n.Get_Rot_Mono ( );
	os	<< n.Get_Pos ( ) << ", " 
		<< n.Get_Size ( ) << ", {"
		<< mono.angle << ":" << mono.axis << "}";
	return os;
}