#pragma once

struct collision_result {
	// specifies if the collision appened
	bool triggered = false;
	// specifies the direction for the shortes exit
	vec3 exit_direction = {0,0,0};
};

std::ostream& operator << ( std::ostream& os, const collision_result& C ) {
	os << ( C.triggered ? "[collided] { " :  "[not collided] { " ) << C.exit_direction << " } ";
	return os;
}