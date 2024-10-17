#ifndef INTERPOLATION
#define INTERPOLATION

template < typename T >
T lerp ( T a, T b, float t ) {
	return a + (b - a) * t;
}

template < typename T >
T Qerp ( T a, T b, float t ) {
	return a + (b - a) * t * t;
}

#endif