#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <utilities/printer.h>

using namespace glm;

class Transform
{
public:
	struct quat
	{ float w,x,y,z; };

	// from quaternion rotation results
	struct mono_axis_rotation {
		float angle;
		vec3 axis;
	};
private:
	std::shared_ptr < Transform > _father = nullptr;

	vec3 _pos = {0,0,0};
	vec3 _father_pos = {0,0,0};
	vec3 _exposable_pos = {0,0,0};
	float _exposable_rot_z = 0;
	vec3 _size = {100,100,100};
	quat _rot = {1,0,0,0};
	vec3 _rot_pivot = {0,0,0};
	float _rot_z = 0;

	bool _recalcutate = true;
	mat4 _model = mat4(1.0f);
public:
	Transform ( vec3 pos = {0,0,0}, vec3 size = {100,100,100}, vec3 rot_pivot = {0,0,0} );

	Transform& Set_Father ( std::shared_ptr <Transform> parent);
	std::shared_ptr <Transform> Get_Father ( ) const;

	Transform& Set_Pos ( vec3 pos );
	Transform& Set_Pos ( float x_pos );
	Transform& Inc_Pos ( vec3 d_pos );

	Transform& Set_Size ( vec3 size );

	Transform& Set_Rot ( vec3 rot );
	Transform& Set_2D_Rot ( float z_rot );
	Transform& Set_Rot_Pivot ( vec3 rot_pivot );

	const vec3 Get_Pos ( ) const;
	const vec3 Get_Size ( ) const;
	const vec3 Get_Rot ( ) const;
	const vec3 Get_Rot_Pivot ( ) const;
	const float Get_2D_Rot ( ) const;

	// position in local space
	vec3& Expose_Pos ( );
	// rotation 2D in local space
	float& Expose_2D_Rot ( );

	mono_axis_rotation Get_Rot_Mono ( ) const;

	mat4 Get_Model_Mat ( );

	friend std::ostream& operator << ( std::ostream& os, Transform& n );
	friend std::ostream& operator , ( std::ostream& out, Transform& n );
};