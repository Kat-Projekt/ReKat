#ifndef FONT_H
#define FONT_H

#include "../graphik_resource.hpp"

#include "texture.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../graphik_debugger.hpp"

class Font : public Resource {
private:
	std::shared_ptr < Texture > _texture;

	unsigned int _padding = 2;
	unsigned int _font_heigth;
	unsigned int _heigth;
	unsigned int _width;
	unsigned int _letters_spacing = 0;
public:
	unsigned int  * char_widths;

	Font ( );

	int Make ( );
	int Make ( const char * path, unsigned int font_heigth = 30, unsigned int letters_spacing = 5 );
	void End ( );
	void Use ( );

	int Get_Heigth ( ) const;

	std::shared_ptr < Texture > Get_Texture ( ) const;
};

#endif