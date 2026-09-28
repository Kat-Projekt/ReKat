#include "extensions/graphik/resources/texture.h"

Texture::Texture
( )
: _width(0), _height(0), _format(GL_RGB), _wrap_s(GL_REPEAT), _wrap_t(GL_REPEAT), _filter_min(GL_NEAREST), _filter_max(GL_NEAREST) { }

Texture::Texture
( unsigned int id, unsigned int texture_number )
: _width(0), _height(0), _format(GL_RGB), _wrap_s(GL_REPEAT), _wrap_t(GL_REPEAT), _filter_min(GL_NEAREST), _filter_max(GL_NEAREST) 
{ this->_id = id; this->_texture_number = texture_number; }

Texture::Texture
( const char * file, unsigned int texture_number )
: _width(0), _height(0), _format(GL_RGB), _wrap_s(GL_REPEAT), _wrap_t(GL_REPEAT), _filter_min(GL_NEAREST), _filter_max(GL_NEAREST) 
{
	Make ( file );
	this->_texture_number = texture_number;
}

void Texture::End
( )
{ glDeleteTextures(1,&_id); }

void Texture::Use
( ) { 
	glActiveTexture(GL_TEXTURE0 + _texture_number); GL_CHECK_ERROR; 
	glBindTexture(GL_TEXTURE_2D, this->_id); GL_CHECK_ERROR; 
}

int Texture::Make
( )
{ return 1; }

int Texture::Make
( const char * file )
{
	glGenTextures(1, &this->_id); GL_CHECK_ERROR;

	// load image
	int width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);

	if ( data )
	{
		this->_width = width;
		this->_height = height;

		if (nrChannels == 1) { _format = GL_RED; }
		else if (nrChannels == 3) { _format = GL_RGB; }
		else if (nrChannels == 4) { _format = GL_RGBA; }

		glActiveTexture(GL_TEXTURE0); GL_CHECK_ERROR;
		glBindTexture(GL_TEXTURE_2D, this->_id); GL_CHECK_ERROR;
		glTexImage2D(GL_TEXTURE_2D, 0, _format, width, height, 0, _format, GL_UNSIGNED_BYTE, data); GL_CHECK_ERROR;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->_wrap_s); GL_CHECK_ERROR;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->_wrap_t); GL_CHECK_ERROR;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->_filter_min); GL_CHECK_ERROR;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->_filter_max); GL_CHECK_ERROR;

		glGenerateMipmap(GL_TEXTURE_2D); GL_CHECK_ERROR;
		glBindTexture(GL_TEXTURE_2D, 0); GL_CHECK_ERROR;
	}
	else
	{
		stbi_image_free(data);
		DEBUG ( 1, "Failed loading Texture: ", file );
		return FAILED_LOAD_IMAGE;
	}

	stbi_image_free(data);
	DEBUG ( 4, "Succesfuly loaded image: ", file );
	return SUCCESS; 
}

int Texture::Make (
	const char * file,
	unsigned int Texture_Number
) {
	_texture_number = Texture_Number;
	return Make ( file );
}

int Texture::Make
( unsigned char* data, unsigned int width, unsigned int height, int channels )
{
	this->_width = width;
	this->_height = height;
	glGenTextures(1, &_id);
	if ( data ) {
		switch ( channels ) {
			case 1: _format = GL_RED; break;
			case 3: _format = GL_RGB; break;
			case 4: _format = GL_RGBA; break;
			default: _format = GL_RED;
		}

		glBindTexture(GL_TEXTURE_2D, _id);
		glTexImage2D(GL_TEXTURE_2D, 0, _format, _width, _height, 0, _format, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{ return EMPTY_BUFFER; }

	return SUCCESS; 
}