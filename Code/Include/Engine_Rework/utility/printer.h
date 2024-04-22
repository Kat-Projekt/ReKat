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

template < typename T > 
std::ostream& operator << ( std::ostream& os, const std::vector<T>& vec ) {
	os << vec.size ( );
	if ( vec.size ( ) == 0 ) { return os; }
	os << " : ";
	for ( auto e : vec ) 
	{ std::cout << "{" << e << "} "; }
	return os;
}