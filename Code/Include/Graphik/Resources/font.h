#ifndef FONT_H
#define FONT_H

#include "shader.h"
#include "texture.h"

#include <map>
#include <vector>
#include <ft2build.h>
#include FT_FREETYPE_H

class Font {
private:
	/* data */
public:
	Font ( ) { };
	Font ( std::string path ) { Make ( path );};
	~Font ( ) { End(); }

	int Make ( std::string path );

	void End ( ) { };
};

int Font::Make ( std::string path ) {

}

#endif