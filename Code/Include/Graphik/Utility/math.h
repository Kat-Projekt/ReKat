#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "list.h"

using namespace glm;

inline float Dist ( vec2 p1, vec2 p2 ) {
	vec2 D = p1 - p2;
	float DD = abs (D.x) + abs(D.y);
	return DD;
}

bool Near ( vec2 p1, vec2 p2, float d ) {
	float DD = Dist (p1,p2);
	if ( DD < d ) { return true; }
	return false;
}

vec2 lerp ( vec2 a, vec2 b, float t ) { return a + (b - a) * t; }

vec2 Qerp ( vec2 a, vec2 b, float t ) {
	return a + (b - a) * t * t;
}

vec2 Normalize ( vec2 a ) {
    float m = sqrt(a.x*a.x + a.y*a.y);
    if ( m == 0 ) { return {0,0}; }
    return a / m;
}

float angle ( vec2 v ) {
	float angle = 0;
	float X = v.x / sqrt( v.x * v.x + v.y *v.y );
	if ( v.y > 0 ) { angle = 3.1415+std::asin ( X ); } 
	else { angle = -std::asin ( X ); }
	return 3.1415 -angle;
}

std::ostream& operator << ( std::ostream& os, const vec2& vec ) {
	os << vec.x << " : " << vec.y;
	return os;
}