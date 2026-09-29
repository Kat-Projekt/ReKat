#pragma once
/**********************************************************************
 * @file printer.h
 * @brief defines ostream for vec2, vec3, vec4, mat4, std::vector < T >
 *********************************************************************/

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

inline std::ostream& operator << ( std::ostream& os, const glm::vec2& vec ) {
	os << "{" << vec.x << ":" << vec.y << "}";
	return os;
}

inline std::ostream& operator << ( std::ostream& os, const glm::vec3& vec ) {
	os << "{" << vec.x << ":" << vec.y << ":" << vec.z << "}";
	return os;
}

inline std::ostream& operator << ( std::ostream& os, const glm::vec4& vec ) {
	os << "{ " << vec.x << ": " << vec.y << ": " << vec.z << ": " << vec.w << " }";
	return os;
}

inline std::ostream& operator << ( std::ostream& os, const glm::highp_mat4& mat ) {
	os << "{" << mat[0][0] << ":" << mat[0][1] << ":" << mat[0][2] << ":" << mat[0][3] << "},\n";
	os << "{" << mat[1][0] << ":" << mat[1][1] << ":" << mat[1][2] << ":" << mat[1][3] << "},\n";
	os << "{" << mat[2][0] << ":" << mat[2][1] << ":" << mat[2][2] << ":" << mat[2][3] << "},\n";
	os << "{" << mat[3][0] << ":" << mat[3][1] << ":" << mat[3][2] << ":" << mat[3][3] << "}";
	return os;
}

template < typename T > 
inline std::ostream& operator << ( std::ostream& os, const std::vector<T>& vec ) {
	if ( vec.size ( ) == 0 )
	{
		os << "{ }";
		return os;
	} else {
		os << "{ ";
		for ( auto e : vec ) 
		{ os << "{" << e << "} "; }
		os << "}";
		return os;
	}
}

inline std::ostream& operator << ( std::ostream& os, const std::unordered_map < std::string, std::string > map )
{
	if ( map.size ( ) == 0 )
	{
		os << "{ }";
		return os;
	} else {
		os << "{ ";
		for ( auto e : map ) 
		{ os << "{" << e.first << ", " << e.second << "} "; }
		os << "}";
		return os;
	}
}