#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include "texture.h"

class Framebuffer : public Texture {
private:
	unsigned int fbo;
	unsigned int rbo;
public:
	Framebuffer ( ) { }
	Framebuffer ( int W, int H ) { Make ( W, H ); }
	virtual int Make ( int W, int H ) {	
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		
		// create texture frome fbo
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, W, H, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ID, 0);

		// create render buffer
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo );
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, W, H);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		int e = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		return e; 
	}
	void Bind_Framebuffer ( ) { glBindFramebuffer(GL_FRAMEBUFFER, fbo); }
	void UBind_Framebuffer ( ) { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
	~Framebuffer ( ) { glDeleteFramebuffers( 1, &fbo ); }
};

#endif