#ifndef SPRITE_H
#define SPRITE_H

#include "../components/camera.h"
#include "../manager.hpp"
#include "../graphik_debugger.hpp"

class Sprite : public Behaviour {
private:
	unsigned int _quad;
	unsigned int VBO;
	ivec2 _frames = {1,1};
	vec4 _color = {1,1,1,1};
	std::string _instacer = "";
	std::string _texture = "";
	std::string _shader = ""; 
	std::string _camera = "";
	bool _UI_render = true;
public:
	int _frame = 0;
	Sprite ( );

	void Start ( );
	void Update ( );
	void Delete ( );

	Sprite& Set (
		std::string texture,
		std::string shader = "",
		std::string camera = "",
		ivec2 frames = {1,1},
		int frame = 0,
		vec4 color = {1,1,1,1},
		bool UI_sprite = false,
		std::string instancer = ""
	);
	Sprite& Set ( const char* texture );
	Sprite& Set ( bool UI_sprite );
	Sprite& Set ( int frame );
	Sprite& Set ( vec4 color );

	vec4 * Expose_Color ( );
};

#endif