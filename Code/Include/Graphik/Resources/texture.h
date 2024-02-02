#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

class Texture {
public:
    enum Status {
        SUCCESS,
        FAILED_LOAD_IMAGE,
        EMPTY_BUFFER = -1
    };

    unsigned int ID;
    unsigned int Width, Height;
    unsigned int Format;
    unsigned int Wrap_S;
    unsigned int Wrap_T;
    unsigned int Filter_Min;
    unsigned int Filter_Max;

    Texture ( ) : Width(0), Height(0), Format(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_NEAREST), Filter_Max(GL_NEAREST) { }
    Texture ( int ID ) : Width(0), Height(0), Format(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_NEAREST), Filter_Max(GL_NEAREST) { this->ID = ID; }
	Texture ( const char * file ) : Width(0), Height(0), Format(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_NEAREST), Filter_Max(GL_NEAREST) 
	{ Make ( file ); }
    ~Texture ( ) { End ( ); }

    // loads the texture
	// -----------------
    int Make ( const char * file );
    // creates the texture
	// -------------------
    static int Make ( unsigned char* data, unsigned int width, unsigned int height, int nrChannels );
    // activate the texture
	// --------------------
    void Use ( ) const { glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, this->ID); }
	// deletes the texture
	// -------------------
	void End ( ) { glDeleteTextures(1,&ID); }
};

int Texture::Make ( const char * file ) {
    glGenTextures(1, &this->ID);

    // load image
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);

    if (data) {
        this->Width = width;
        this->Height = height;

        if (nrChannels == 1) { Format = GL_RED; }
        else if (nrChannels == 3) { Format = GL_RGB; }
        else if (nrChannels == 4) { Format = GL_RGBA; }

        glBindTexture(GL_TEXTURE_2D, this->ID);
        glTexImage2D(GL_TEXTURE_2D, 0, Format, width, height, 0, Format, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);

        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    } else { stbi_image_free(data); return FAILED_LOAD_IMAGE; }

    stbi_image_free(data);
    return SUCCESS; 
}

int Texture::Make ( unsigned char* data, unsigned int width, unsigned int height, int nrChannels ) {
    unsigned int ID;
    int Format;
    glGenTextures(1, &ID);
    if (data) {
        Format = GL_RED;
        switch ( nrChannels ) {
        case 1: Format = GL_RED; break;
        case 3: Format = GL_RGB; break;
        case 4: Format = GL_RGBA; break;
        }

        glBindTexture(GL_TEXTURE_2D, ID);
        glTexImage2D(GL_TEXTURE_2D, 0, Format, width, height, 0, Format, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    } else { return EMPTY_BUFFER; }

    return ID; 
}

#endif