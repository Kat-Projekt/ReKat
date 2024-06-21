#ifndef _simulation_render
#define _simulation_render

#include <engine.hpp>
#include "simulation.h"
using namespace ReKat::grapik::Input;

class simulation_render : public Behaviour {
	simulation * sim;
	unsigned int VBO, VAO, EBO;
	Shader * ourShader;

	unsigned int texture;
public:
	virtual void Start ( ) {
		sim = obj->Get_Component < simulation > ( );
		ourShader = Manager::Shader_Get ( "renderer" );

		// conifigure render
		float vertices[] = {
			// positions          // colors           // texture coords
			1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // top right
			1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // bottom right
			-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f, // bottom left
			-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f  // top left 
		};
		unsigned int indices[] = {  
			3, 2, 1, // first triangle
			3, 1, 0  // second triangle
		};
		
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}
	virtual void Update ( ) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		// create a texture
		unsigned char * data = ( unsigned char * ) calloc ( sim->total, sizeof(unsigned char) );
		for ( size_t i = 0; i < sim->total; i++ ) {
			unsigned char d = sim->current_matrix [i].parzial_pression * 256;
			if ( sim->current_matrix [i].parzial_pression == 1 ) { d = 255; }
			data[i] = d;
		}

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// load image, create texture and generate mipmaps

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, sim->size_W, sim->size_H, 0, GL_RED, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

        // render container
        glBindTexture(GL_TEXTURE_2D, texture);
        ourShader->Use( );
		ourShader->setInt ( "image", 0 );
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
	}

	virtual void Delete ( ) {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}
};

#endif