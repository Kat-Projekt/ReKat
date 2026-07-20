#ifndef TEXTURE_H
#define TEXTURE_H

#include "../graphik_resource.hpp"

#include <glad/glad.h>
#include "../graphik_debugger.hpp"

#include <stb_image.h>
#include <stb_image_write.h>

class Texture: public Resource {
private:

	unsigned int _id = 0;
	unsigned int _texture_number = 0;
	unsigned int _width = 0, _height = 0;
	unsigned int _format = 0;
	unsigned int _wrap_s = 0;
	unsigned int _wrap_t = 0;
	unsigned int _filter_min = 0;
	unsigned int _filter_max = 0;

public:
	Texture ( );
	Texture (
		unsigned int id,
		unsigned int texture_number = 0
	);
	Texture (
		const char * file,
		unsigned int texture_number = 0
	);

	int Make ( );
	// loads the texture
	// -----------------
	int Make ( const char * file );
	// creates the texture
	// ------------------
	int Make (
		unsigned char* data,
		unsigned int width,
		unsigned int height,
		int channels
	);
	// activate the texture
	// --------------------
	void Use ( );
	// deletes the texture
	// -------------------
	void End ( );
};

#endif