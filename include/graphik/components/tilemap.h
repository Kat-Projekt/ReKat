#ifndef TILEMAP_H
#define TILEMAP_H

#include "camera.h"

#include "../manager.hpp"
#include "../graphik_debugger.hpp"

class Tilemap : public Behaviour {
private:
	std::string _path = "";
	std::string _shader = ""; 
	std::string _texture = "";
	std::string _camera = "";

	vec4 _color = {1,1,1,1};
	vec2 _tile_set = {32,32};

	int _instances = 0;
	unsigned int _quad = 0;
public:
	Tilemap ( );

	void Start ( );
	void Update ( );

	Tilemap* Set (
		std::string path,
		std::string texture,
		std::string shader,
		std::string camera,
		vec2 tile_set = {1,1},
		vec4 color = {1,1,1,1}
	);
};
#endif
