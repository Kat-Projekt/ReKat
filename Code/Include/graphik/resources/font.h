#ifndef FONT_H
#define FONT_H

#include "../../resource.hpp"

#include "texture.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../graphik_debugger.hpp"

class Font : public Resource {
private:
	unsigned int _padding = 2;
	unsigned int _texture;
	unsigned int _font_heigth;
	unsigned int _heigth;
	unsigned int _width;

	enum Status {
		SUCCESS,
		FAILED_INIT_FREETYPE,
		FAILED_LOAD_FONT,
		SHADER_NOT_SET
	};
public:
	unsigned int  * char_widths;

	Font ( ) { _font_heigth = 30; };
	Font ( unsigned int font_heigth ) { _font_heigth = font_heigth; }
	Font ( const char * path, unsigned int font_heigth = 30 ) : _font_heigth(font_heigth) { Make ( path ); }
	~Font ( ) { End( ); }

	int Make ( const char * );

	void End ( ) { };

	Texture * Get_Texture ( ) { return new Texture(_texture); }

    void Use ( ) const { glActiveTexture(GL_TEXTURE0); GL_CHECK_ERROR; glBindTexture(GL_TEXTURE_2D, _texture); GL_CHECK_ERROR; }

	int Get_Heigth ( ) { return _font_heigth; }
};

int Font::Make ( const char * path ) {
	DEBUG( 4,"Creating Font", path);
	FT_Library ft;
	if ( FT_Init_FreeType ( &ft ) ) 
	{ return FAILED_INIT_FREETYPE; }

	FT_Face face;
	if ( FT_New_Face ( ft, path, 0, &face ) ) { return FAILED_LOAD_FONT; }

	// set size to load glyphs as
	FT_Set_Pixel_Sizes(face, _font_heigth, 0);

	// disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// create bitmap parameters
	_width = ( _font_heigth + _padding ) * 16; // 16 per row with 2 px of padding
	_heigth = ( _font_heigth + _padding ) * 8; // 8 per collumb
	unsigned int p = _width*_heigth;
	unsigned char combined_buffer[p];
	DEBUG (5, "combined size: ", _width * _heigth );
	char_widths = (unsigned int*) calloc (128, sizeof(unsigned int));

	DEBUG( 5,"Texture size: " + std::to_string (_width) + " : " + std::to_string (_heigth));

	// load first 128 characters of ASCII set
	int Baseline = 0;
	for ( unsigned char c = 0 ; c < 127 ; c++ ) {
		// Load character glyph 
		if ( FT_Load_Char(face, c, FT_LOAD_RENDER) ) { continue; }

		// get the glyph metrics
		const FT_Glyph_Metrics& metrics = face->glyph->metrics;

		// find the character that reaches below the baseline by the biggest value
		int glyphHang = ( metrics.horiBearingY-metrics.height ) / 64;
		if( glyphHang < Baseline ) 
		{ Baseline = glyphHang; }
	}
	
	DEBUG( 6,"Baseline: " + std::to_string (Baseline));

	// draw all characters
	for ( unsigned char c = 32 ; c < 128 ; c++ ) {
		// Load character glyph 
		if ( FT_Load_Char(face, c, FT_LOAD_RENDER) ) { continue; }

		// save the character width
		char_widths[c] = face->glyph->metrics.width/64;

		// find the tile position where we have to draw the character
		int x = (c%16)*(_font_heigth+_padding);
		int y = (c/16)*(_font_heigth+_padding);
		x += 1; // 1 pixel padding from the left side of the tile
		y += (int)(_font_heigth+_padding) - face->glyph->bitmap_top + Baseline - _padding*0.5;

		// draw the character
		const FT_Bitmap& bitmap = face->glyph->bitmap;
		
		DEBUG ( 5, "P ", (y+bitmap.rows-1) * _width+(x+bitmap.width-1) );
		for ( int xx = 0; xx < bitmap.width; xx++ ) {
			for ( int yy = 0; yy < bitmap.rows; yy++ ) {
				// DEBUG ( 6, "Accesing bitmap ", y, "+", yy, " ", x, "+", xx );
				unsigned char r = bitmap.buffer[(yy*(bitmap.width)+xx)];
				// DEBUG ( 6, "Accesing combined" );
				combined_buffer[(y+yy)*_width+(x+xx)] = r;
			}
		}
		DEBUG ( 6, "Character: '", (char)c, "' created" );
	}

	DEBUG ( 5, "Created characters" );
	
	_texture = Texture::Make ( combined_buffer, _width, _heigth, 1 );
	glBindTexture(GL_TEXTURE_2D, 0);

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	DEBUG ( 5, "Font Loaded" );

	return 0;
}

#endif