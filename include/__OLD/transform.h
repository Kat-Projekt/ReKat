#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>

#include "utility/debugger.h"
#include "behaviour.h"

class Transform : public Behaviour
{
public:
	// from quaternion rotation results
	struct mono_axis_rotation {
		float angle;
		glm::vec3 axis;
	};
private:
	// father like in objekt
	Transform * _father = nullptr;

	glm::vec3 _pos = {0,0,0};
	glm::vec3 _size = {100,100,100};
	// {w,x,y,z}
	glm::quat _rot = {1,0,0,0};
	glm::vec3 _rot_pivot = {0,0,0};

	// chashed model matrix
	mutable glm::mat4 _local_model = glm::mat4(1.0f);
public:
	REFLECT ( Transform );
	METADATA ( "Transform", 1.0, "Descries the affine transformation of the objekt" );
	PARAMETERS (
		PARAMETER ( "pos", _pos ),
		PARAMETER ( "size", _size ),
		PARAMETER ( "rot_pivot", _rot_pivot ),
		PROPERTY ( "_rot", Set_Rot, Get_Local_Rotation )
	);

	METHODS (
		METHOD ( "Set 2D rot", Set_2D_Rot, float, rot_z );
		METHOD ( "Set 2D rot", Set_2D_Rot );
	);


	/*************************************************************************
	 * \brief [internal] Used for setting the local space reference ( father )
	 ************************************************************************/
	Transform * _Set_Father ( Transform * father ) noexcept
	{ _father = father; return this; }


	/*****************************************
	 * \brief Sets the position in local space
	 ****************************************/
	Transform * Set_Pos ( glm::vec3 pos ) noexcept
	{ _pos = pos; Modify ( ); return this; }
	/******************************************************
	 * \brief Sets the z position, used for sprite layering
	 * 
	 * Note: use the ortographic camera mode
	 *****************************************************/
	Transform * Set_Depth ( float z_pos ) noexcept
	{ _pos.z = z_pos; Modify ( ); return this; }
	/***************************************
	 * \brief Increments the local position
	 **************************************/
	Transform * Translate ( glm::vec3 d_pos ) noexcept
	{ _pos += d_pos; Modify ( ); return this; }
	/********************************
	 * \brief Gets the world position
	 *******************************/
	const glm::vec3 Get_Pos ( ) const noexcept;
	/*************************************
	 * \brief Gets position in local space
	 ************************************/
	const glm::vec3& Get_Local_Pos ( ) const noexcept
	{ return _pos; }


	/*************************************
	 * \brief Sets the size in local space
	 ************************************/
	Transform * Set_Size ( glm::vec3 size ) noexcept
	{ _size = size; Modify ( ); return this; }
	/*****************************************************************
	 * \brief Hadamard product between current scale and scale_factors
	 ****************************************************************/
	Transform * Scale ( glm::vec3 scale_factors ) noexcept
	{ _size *= scale_factors; Modify ( ); return this; }
	/****************************************
	 * \brief Uniform scale in all directions
	 ***************************************/
	Transform * Scale ( float scale_factor ) noexcept
	{ _size *= scale_factor; Modify ( ); return this; }
	/*****************************
	 * \brief Gets the world scale
	 ****************************/
	const glm::vec3 Get_Size ( ) const noexcept
	{ return ( _father ? _father->Get_Size ( ) * _size : _size ); }
	/**********************************
	 * \brief Gets scale in local space
	 *********************************/
	const glm::vec3& Get_Local_Size ( ) const noexcept
	{ return _size; }


	/**********************************************
	 * \brief Sets the euclidean rotation
	 * 	The angles are in degress
	 * 
	 * This is the classic {x, y, z} axis rotation
	 * Where x, y, z are the rotation angle in that
	 * axis
	 * If you need to interpolate the rotation use
	 * quaternion rotation, it's more coherent
	 ********************************************/
	Transform * Set_Rot ( glm::vec3 rot ) noexcept
	{ _rot = glm::normalize ( glm::quat ( glm::radians ( rot ) ) ); Modify ( ); return this; }
	/**********************************************
	 * \brief Directly sets the rotation quaternion
	 *********************************************/
	Transform * Set_Rot ( glm::quat rot ) noexcept
	{ _rot = rot; Modify ( ); return this; }
	/*****************************************************
	 * \brief Sets the xy plane rotation, used for sprites
	 * 	The angle is in degress
	 * 
	 * Note: use the ortographic camera mode
	 ****************************************************/
	Transform * Set_2D_Rot ( float rot ) noexcept
	{ return Set_Rot ( glm::vec3 { 0.0f, 0.0f, rot } ); }
	/************************************************
	 * \brief Rotates the objekt by the specified rot
	 * 	The angles are in degress
	 * This is the classic {x, y, z} axis rotation
	 * Where x, y, z are the rotation angle
	 * 
	 ***********************************************/
	Transform * Rotate ( glm::vec3 rot ) noexcept
	{ _rot = glm::normalize ( glm::quat ( glm::radians ( rot ) ) * _rot ); Modify ( ); return this; }
	/**************************************************
	 * \brief Rotation on the xy plane used for sprites
	 * 	The angle is in degress
	 * 
	 * Note: use the ortographic camera mode
	 *************************************************/
	Transform * Rotate_2D ( float rot ) noexcept
	{ return Rotate ( glm::vec3 { 0.0f, 0.0f, rot } ); }
	/*************************************
	 * \brief Gets the rotation quaternion
	 ************************************/
	const glm::quat& Get_Local_Rotation ( ) const noexcept
	{ return _rot; }
	/***************************************************************
	 * \brief Gets the axis of the combined rotation for local space
	 * 
	 * Constructs form the rotations commands the axis of rotation
	 * and the angle between the {1,0,0} and {1,0,0} rotated
	 * This is the local space rotation
	 * 
	 * \return {axis,angle} the angle is in degress
	 **************************************************************/
	const mono_axis_rotation Get_Rot_Mono ( ) const noexcept;
	/*****************************************************
	 * \brief Gets the rotation on the xy plane
	 * 
	 * note that this feature wont work well on a 3D scene
	 * It is intended for 2D Games
	 ****************************************************/
	float Get_2D_Rot ( ) const noexcept
	{ return glm::eulerAngles ( _rot ).z; }

	// No global rotation will be implemented because of the rotation_pivot


	/********************************************************
	 * \brief Sets the center of the quaternion rotation
	 * 
	 * Consider a box of Get_Size ( ) where:
	 * - {0,0,0} is the center
	 * - \[{\pm0.5,\pm0.5,\pm0.5}\] are vertexes
	 * 
	 * the coordinates are:
	 * - left is postive
	 * - up is positve
	 * - near is positive
	 * 
	 * the rotation pivot is from where to apply the rotation
	 *******************************************************/
	Transform * Set_Rot_Pivot ( glm::vec3 rot_pivot ) noexcept
	{ _rot_pivot = rot_pivot; Modify ( ); return this;}
	/***************************************************
	 * \brief Gets the center of the quaternion rotation
	 **************************************************/
	const glm::vec3 Get_Rot_Pivot ( ) const noexcept
	{ return _rot_pivot; }

	
	/*************************************************************
	 * \brief Gets the global space model matrix that encapsulates
	 * 	position, size and rotation of the objekt
	 * 
	 * The model matrix [M] depends on the father model matrix [MF]
	 * and on the local space matrix [ML] in the following:
	 * ```latex
	 * 	M = MF * ML
	 * ```
	 * so that the father matrix transform the space first then
	 * the child transformation appens.
	 * 
	 ************************************************************/
	glm::mat4 Get_Model_Mat ( ) const noexcept
	{ return ( _father ? _father->Get_Model_Mat ( ) * Get_Local_Mat ( ) : Get_Local_Mat ( ) ); }
	/************************************************************
	 * \brief Gets the local space model matrix that encapsulates
	 * 	position, size and rotation of the objekt
	 ***********************************************************/
	const glm::mat4& Get_Local_Mat ( ) const noexcept;

	Transform ( glm::vec3 pos = {0,0,0}, glm::vec3 size = {100,100,100}, glm::vec3 rot_pivot = {0,0,0} )
	: _pos ( pos ), _size ( size ), _rot_pivot ( rot_pivot ) { }

	friend std::ostream& operator << ( std::ostream& os, Transform& n );
};
