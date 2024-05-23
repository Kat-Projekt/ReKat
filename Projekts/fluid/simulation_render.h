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
		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}
	virtual void Update ( ) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		for ( size_t i = 0; i < sim->total; i++ ) {
			ourShader->setFloat ("particles[" + std::to_string ( i ) + "]", sim->current_matrix[i].parzial_pression);
		}

        // render container
        ourShader->Use();
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