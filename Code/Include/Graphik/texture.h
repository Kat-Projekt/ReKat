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
        EMPTY_BUFFER
    };

    // holds the ID of the texture object, used for all texture operations to reference to this particular texture
    unsigned int ID;
    // texture image dimensions
    unsigned int Width, Height; // width and height of loaded image in pixels
    // texture Format
    unsigned int Format; // format of texture object
    // texture configuration
    unsigned int Wrap_S; // wrapping mode on S axis
    unsigned int Wrap_T; // wrapping mode on T axis
    unsigned int Filter_Min; // filtering mode if texture pixels < screen pixels
    unsigned int Filter_Max; // filtering mode if texture pixels > screen pixels
    // constructor (sets default texture modes)
    Texture ( ) : Width(0), Height(0), Format(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_NEAREST), Filter_Max(GL_NEAREST) { }
    Texture ( const char * file ) : Width(0), Height(0), Format(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_NEAREST), Filter_Max(GL_NEAREST) { Make ( file ); }
    ~Texture ( ) { glDeleteTextures(1,&ID); }
    // generates texture from image data
    int Make ( const char * file );
    // generates texture from provided buffer
    static int Make ( unsigned char* data, unsigned int width, unsigned int height, int nrChannels );
    // binds the texture as the current active GL_TEXTURE_2D texture object
    void Bind ( ) const { glBindTexture(GL_TEXTURE_2D, this->ID); }
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