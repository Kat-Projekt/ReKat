#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

/*	manifesto
Behaves like a separate render envoirement
renders the objekts "_to_render" inside the canvas
and saves it in a texture
than it behaves like a sprite Behaviour
*/

#include "../../../objekt.hpp"
#include "../camera.h"
#include "../../resources/shader.h"
#include "../../graphik.hpp"

int gl_error;
#define GL_ERRORS(msg) gl_error = glGetError( ); if ( gl_error != GL_NO_ERROR) { std::cout << "gl_error: '" << gl_error << "' error: '" << msg << "' at line: " << __LINE__ << " of file: " << __FILE__ << '\n'; } 

class Framebuffer : public Behaviour {
private:
	// wher to do the main render call // this objekt must be exluded from the other callers: like manager
	Objekt * _to_render;

	// risolution
	int _width = 0;
	int _heigth = 0;

	// sprite part
	Shader * _shader;
	Camera * _camera;
	bool _UI_render = false;

	// render links
	unsigned int FBO;
	unsigned int TEX;
	unsigned int VAO;
	unsigned int VBO;
public:
    void Start ( ) {
		// STARTING TO RENDER OBJEKT
		// VERY IMPORTANT LINE OF MADNESS
		_to_render->Start ( );

		if ( _width == 0 || _heigth == 0 ) 
		{ DEBUG ( 2, "zero dimension framebuffer" ); return; }

		unsigned int RBO;
		// generation of buffers
        glGenFramebuffers(1, &FBO); GL_CHECK_ERROR;
		glGenRenderbuffers(1, &RBO); GL_CHECK_ERROR;
		glGenTextures(1, &TEX); GL_CHECK_ERROR;
		glGenVertexArrays(1, &VAO); GL_CHECK_ERROR;
		glGenBuffers(1, &VBO); GL_CHECK_ERROR;
		DEBUG ( 4, "generating buffers" );

		// binding buffers
		glBindFramebuffer(GL_FRAMEBUFFER, FBO); GL_CHECK_ERROR;
		glBindRenderbuffer(GL_RENDERBUFFER, RBO); GL_CHECK_ERROR;
		glBindTexture(GL_TEXTURE_2D, TEX); GL_CHECK_ERROR;
		glBindVertexArray(VAO); GL_CHECK_ERROR;
		glBindBuffer(GL_ARRAY_BUFFER, VBO); GL_CHECK_ERROR;
		DEBUG ( 5, "binding buffers" );

		// creating texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _heigth, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); GL_CHECK_ERROR;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); GL_CHECK_ERROR;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); GL_CHECK_ERROR;
		DEBUG ( 5, "created texture" );
		
		// creating RenderBuffer
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _heigth); GL_CHECK_ERROR;
		DEBUG ( 5, "created render and stencil buffer" );
		DEBUG ( 6, "only one combined buffer" );

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TEX, 0); GL_CHECK_ERROR;
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO); GL_CHECK_ERROR;
		DEBUG ( 5, "attach buffers" );

		
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{ DEBUG ( 2, "Framebuffer is not complete" ); return; }

		DEBUG ( 4, "succesfuly created framebuffer" );

		if ( _shader == nullptr ) { DEBUG ( 3, "Skipping sprite genreation" ); return; }
		
		float vertices[] = { 
            // pos      // tex
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 1.0f,

            0.0f, 1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 0.0f
        };
		
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);  GL_CHECK_ERROR;// for position
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); GL_CHECK_ERROR;
		glEnableVertexAttribArray(1);  GL_CHECK_ERROR;// for texture
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))); GL_CHECK_ERROR;

		DEBUG ( 4, "succesfuly created sprite" );

		// unbinding
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0); // first VBO then VAO
		glBindVertexArray(0);

    }

	void Update ( ) {
		// set render buffer
		glBindFramebuffer(GL_FRAMEBUFFER, FBO); GL_CHECK_ERROR;
        glClearColor(0.0, 0.0, 0.0, 0.0f); GL_CHECK_ERROR;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERROR;
		glEnable(GL_DEPTH_TEST); GL_CHECK_ERROR;
		DEBUG (4, "set render framebuffer");
		// do the rendering
		_to_render->Update ( );
		DEBUG (4, "all objekts rendered to framebuffer");

		// set default buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0); GL_CHECK_ERROR;
		// glClearColor(0.0, 0.0, 0.0, 1.0f); GL_CHECK_ERROR;
		// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERROR;
		// glEnable(GL_DEPTH_TEST); GL_CHECK_ERROR;

		// render frame buffer as sprite
		if ( _shader == nullptr || ( _camera == nullptr && !_UI_render ) ) { DEBUG ( 3, "FrameBuffer Sprite not set Correctly" ); return; }
		
		_shader->setMat4  ( "projection", ( _UI_render ? Camera::UI_Projkection ( ) : _camera->Projkection ( )) );
        DEBUG ( 6, "Updated Camera uniform");
		_shader->setMat4 ( "model", obj->Get_Model_Mat ( ) );

        glBindVertexArray(VAO); GL_CHECK_ERROR;
        glBindTexture(GL_TEXTURE_2D, TEX); GL_CHECK_ERROR;
        glDrawArrays(GL_TRIANGLES, 0, 6); GL_CHECK_ERROR;
        glBindVertexArray(0); GL_CHECK_ERROR;

		DEBUG (4, "framebuffer rendered to main context");
	}

	// set resolution of the buffer
	Framebuffer* Set ( int width, int heigth )
	{ _width = width; _heigth = heigth; return this; }

	// set parametes for the sprite part
	// if left empty it behaves like a texture
	Framebuffer* Set ( std::string shader, Camera* camera = nullptr, bool UI_sprite = true ) 
	{ _shader = Manager::Shader_Get ( shader ); _camera = camera; _UI_render = UI_sprite; return this; }
	Framebuffer* Set ( Shader* shader, Camera* camera = nullptr, bool UI_sprite = true ) 
	{ _shader = shader; _camera = camera; _UI_render = UI_sprite; return this; }

	// sender render target
	Framebuffer* Set ( Objekt* to_render )
	{ _to_render = to_render; return this; }
};

#endif