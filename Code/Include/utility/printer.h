#include <iostream>
#include <vector>
#include <glm/glm.hpp>
using namespace glm;

std::ostream& operator << ( std::ostream& os, const vec2& vec ) {
	os << "{ " << vec.x << ": " << vec.y << " }";
	return os;
}

std::ostream& operator << ( std::ostream& os, const vec3& vec ) {
	os << "{ " << vec.x << ": " << vec.y<< ": " << vec.z << " }";
	return os;
}

std::ostream& operator << ( std::ostream& os, const highp_mat4& mat ) {
	os << "{ " << mat[0][0] << ": " << mat[0][1] << ": " << mat[0][2] << ": " << mat[0][3] << " },\n";
	os << "{ " << mat[1][0] << ": " << mat[1][1] << ": " << mat[1][2] << ": " << mat[1][3] << " },\n";
	os << "{ " << mat[2][0] << ": " << mat[2][1] << ": " << mat[2][2] << ": " << mat[2][3] << " },\n";
	os << "{ " << mat[3][0] << ": " << mat[3][1] << ": " << mat[3][2] << ": " << mat[3][3] << " }";
	return os;
}

template < typename T > 
std::ostream& operator << ( std::ostream& os, const std::vector<T>& vec ) {
	os << vec.size ( );
	if ( vec.size ( ) == 0 ) { return os; }
	os << " : ";
	for ( auto e : vec ) 
	{ std::cout << "{" << e << "} "; }
	return os;
}