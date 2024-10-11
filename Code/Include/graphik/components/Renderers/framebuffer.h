#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "../../../objekt.hpp"
#include "../camera.h"
#include "../../resources/shader.h"
#include "../../graphik.hpp"

int gl_error;
#define GL_ERRORS(msg) gl_error = glGetError( ); if ( gl_error != GL_NO_ERROR) { std::cout << "gl_error: '" << gl_error << "' error: '" << msg << "' at line: " << __LINE__ << " of file: " << __FILE__ << '\n'; } 

class Framebuffer : public Behaviour {
private:
	Objekt * _to_render;
	Shader * _shader;
	Camera * _camera;
	unsigned int FBO;
	unsigned int TEX;
	unsigned int VAO;
public:
    void Start ( ) {
        glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		// create a color attachment texture
		glGenTextures(1, &TEX);
		glBindTexture(GL_TEXTURE_2D, TEX);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TEX, 0);
		// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
		unsigned int rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600); // use a single renderbuffer object for both a depth AND stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
		// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			1.0f, -1.0f,  1.0f, 0.0f,
			1.0f,  1.0f,  1.0f, 1.0f
		};

		
		unsigned int quadVBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

		//_shader->setInt ("screenTexture",0);
    }

	void Update ( ) {
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		_to_render->Update ( );
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
        glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		_shader->Use ( );
		_shader->setMat4 ( "projection", _camera->Projkection ( ) );
		_shader->setMat4 ( "model", obj->Get_Model_Mat ( ) );
        glBindVertexArray(VAO);
        glBindTexture(GL_TEXTURE_2D, TEX);
        glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	Framebuffer* Set ( Objekt* to_render, Shader * shader, Camera * camera )
	{ _to_render = to_render; _shader = shader; _camera = camera; return this; }
};

#endif