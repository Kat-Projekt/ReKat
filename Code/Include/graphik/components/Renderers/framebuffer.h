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

// default shaders
#include "default_shaders/framebuffer/frambuffer.s.h"

int gl_error;
#define GL_ERRORS(msg) gl_error = glGetError( ); if ( gl_error != GL_NO_ERROR) { std::cout << "gl_error: '" << gl_error << "' error: '" << msg << "' at line: " << __LINE__ << " of file: " << __FILE__ << '\n'; } 

class Framebuffer : public Behaviour {
private:
	// wher to do the main render call // this objekt must be exluded from the other callers: like manager
	Objekt * _to_render;

	// risolution
	unsigned int _width = 0;
	unsigned int _heigth = 0;
	bool resize = false;
	bool ready_to_render = false;

	// sprite part
	std::string _shader = "";
	std::string _camera = "";

	bool _UI_render = true;

	// render links
	unsigned int FBO = -1;
	unsigned int TEX = -1;
	unsigned int VAO = -1;
	unsigned int VBO = -1;
	unsigned int RBO = -1;
public:
	float _aspect_ratio = -1;
	void Create_Frame_Buffer ( ) {
		// clear evetual memeory
		if ( FBO != -1 ) {
			glDeleteRenderbuffers(1, &RBO); GL_CHECK_ERROR;
			glDeleteFramebuffers(1, &FBO); GL_CHECK_ERROR;
			glDeleteTextures(1, &TEX); GL_CHECK_ERROR;
		}

		// generation of buffers
        glGenFramebuffers(1, &FBO); GL_CHECK_ERROR;
		glGenRenderbuffers(1, &RBO); GL_CHECK_ERROR;
		glGenTextures(1, &TEX); GL_CHECK_ERROR;
		DEBUG ( 4, "generating buffers" );

		// binding buffers
		glBindFramebuffer(GL_FRAMEBUFFER, FBO); GL_CHECK_ERROR;
		glBindRenderbuffer(GL_RENDERBUFFER, RBO); GL_CHECK_ERROR;
		glBindTexture(GL_TEXTURE_2D, TEX); GL_CHECK_ERROR;
		DEBUG ( 5, "binding buffers" );

		// creating texture
		// _aspect_ratio = (float)_width / (float)_heigth;
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

		// unbinding
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		DEBUG ( 4, "succesfuly created framebuffer" );
	}

	void Create_Sprite ( ) {
		float vertices[] = { 
            // pos      // tex
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 1.0f,

            0.0f, 1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 0.0f
        };
		glGenVertexArrays(1, &VAO); GL_CHECK_ERROR;
		glGenBuffers(1, &VBO); GL_CHECK_ERROR;

		glBindVertexArray(VAO); GL_CHECK_ERROR;
		glBindBuffer(GL_ARRAY_BUFFER, VBO); GL_CHECK_ERROR;
		
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);  GL_CHECK_ERROR;// for position
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); GL_CHECK_ERROR;
		glEnableVertexAttribArray(1);  GL_CHECK_ERROR;// for texture
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))); GL_CHECK_ERROR;

		// unbinding
		glBindBuffer(GL_ARRAY_BUFFER, 0); // first VBO then VAO
		glBindVertexArray(0);

		DEBUG ( 4, "succesfuly created sprite" );
	}

    void Start ( ) {
		// STARTING TO RENDER OBJEKT
		// VERY IMPORTANT LINE OF MADNESS
		if ( _to_render != nullptr ) { _to_render->Start ( ); }

		if ( _width == 0 || _heigth == 0 ) 
		{ DEBUG ( 3, "zero dimension framebuffer" ); return; }

		Create_Frame_Buffer ( );

		if ( _shader == "" ) { _shader = "framebuffer_shader_default"; }
		
		Create_Sprite ( );

		resize = false; 
		ready_to_render = true;
    }

	void Update ( ) {
		if ( resize ) {
			Create_Frame_Buffer ( );
			// auto S = obj->Get_Size ( );
			// obj->Set_Size ( { S.x , S.y, S.z } );
			resize = false;
			ready_to_render = true;
		}
		if ( !ready_to_render ) { return; }
		// set render buffer
		glBindFramebuffer(GL_FRAMEBUFFER, FBO); GL_CHECK_ERROR;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERROR;
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); GL_CHECK_ERROR;
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
		if ( _shader == "" || ( _camera == "" && !_UI_render ) ) { DEBUG ( 3, "FrameBuffer Sprite not set Correctly" ); return; }
		
		Manager::Shader_Get ( _shader )->setMat4  ( "projection", ( _UI_render ? Camera::UI_Projkection ( ) : Manager::Camera_Get ( _camera )->Projkection ( ) ) );
        DEBUG ( 6, "Updated Camera uniform");
		Manager::Shader_Get ( _shader )->setMat4 ( "model", obj->Get_Model_Mat ( ) );

        glBindVertexArray(VAO); GL_CHECK_ERROR;
        glBindTexture(GL_TEXTURE_2D, TEX); GL_CHECK_ERROR;
        glDrawArrays(GL_TRIANGLES, 0, 6); GL_CHECK_ERROR;
        glBindVertexArray(0); GL_CHECK_ERROR;

		DEBUG (4, "framebuffer rendered to main context");
	}

	void Fixed_Update ( ) {
		_to_render->Fixed_Update ( );
	}

	void Delete ( ) {
		glDeleteRenderbuffers(1, &RBO);
		glDeleteFramebuffers(1, &FBO);
        glDeleteVertexArrays(1, &VAO);
		glDeleteTextures(1, &TEX);
        glDeleteBuffers(1, &VBO);
    }

	// set resolution of the buffer
	Framebuffer* Set ( unsigned int width, unsigned int heigth ) {
		_width = width; _heigth = heigth; 
		resize = true; 
		if ( _aspect_ratio == -1 ) 
		{ _aspect_ratio = (float)_width / (float)_heigth; }
		return this;
	}

	// set parametes for the sprite part
	// if left empty it behaves like a texture
	Framebuffer* Set ( std::string shader, std::string camera = "", bool UI_sprite = true ) 
	{ _shader = shader; _camera = camera; _UI_render = UI_sprite; return this; }
	// Framebuffer* Set ( Shader* shader, Camera* camera = nullptr, bool UI_sprite = true ) 
	// { _shader = shader; _camera = camera; _UI_render = UI_sprite; return this; }

	// sender render target
	Framebuffer* Set ( Objekt* to_render ) {
		if ( _started ) { to_render->Start ( ); }
		DEBUG ( 3, "Added ne context scene " );
		_to_render = to_render; return this;
	}
};


namespace Manager {
	static int Camera_Load ( std::string name, Objekt* pointer, Framebuffer* fb ) {
		auto C = pointer->Add_Component < Camera > ( );
		if ( pointer == nullptr ) 
		{ DEBUG ( 1, "Invalid camera pointer" ); return 1; }
		if ( fb != nullptr ) 
		{ C->fb_scale = &fb->_aspect_ratio; }
		if ( cameras.find ( name ) == cameras.end ( ) ) 
		{ cameras.insert ( {name, C} ); }
		else 
		{ cameras [name] = C; }
		
		return 0;
	}
}

#endif