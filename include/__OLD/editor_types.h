#include <string>
#include <glm/glm.hpp>

enum class EditorType {
	Unknown,
	Int,
	Float,
	Double,
	String,
	Vec2,
	Ivec2,
	Vec3,
	Ivec3,
	Vec4,
	Ivec4,
	Mat4
};

template <typename T>
constexpr EditorType GetType ( )
{
	return Type
		< std::remove_cv_t
			< std::remove_reference_t
				< T >
			>
		> :: value;
}

template < typename T >
struct Type
{ static constexpr EditorType value = EditorType::Unknown; };

template <>
struct Type<int>
{ static constexpr EditorType value = EditorType::Int; };

template <>
struct Type<float>
{ static constexpr EditorType value = EditorType::Float; };

template <>
struct Type<double>
{ static constexpr EditorType value = EditorType::Double; };

template <>
struct Type<std::string>
{ static constexpr EditorType value = EditorType::String; };

template <>
struct Type<glm::vec2>
{ static constexpr EditorType value = EditorType::Vec2; };

template <>
struct Type<glm::ivec2>
{ static constexpr EditorType value = EditorType::Ivec2; };

template <>
struct Type<glm::vec3>
{ static constexpr EditorType value = EditorType::Vec3; };

template <>
struct Type<glm::ivec3>
{ static constexpr EditorType value = EditorType::Ivec3; };

template <>
struct Type<glm::vec4>
{ static constexpr EditorType value = EditorType::Vec4; };

template <>
struct Type<glm::ivec4>
{ static constexpr EditorType value = EditorType::Ivec4; };

template <>
struct Type<glm::mat4>
{ static constexpr EditorType value = EditorType::Mat4; };
