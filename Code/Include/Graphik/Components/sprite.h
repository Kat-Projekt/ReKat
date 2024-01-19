#ifndef SPRITE_H
#define SPRITE_H

#include "camera.h"
#include "../Resources/manager.hpp"

class Sprite : public Behaviour {
private:
    unsigned int quadVAO;
public:
    Shader  *shader = nullptr; 
    Texture *texture = nullptr;
	Camera  *camera = nullptr;
	int frame = 0;
	vec2 frames = {1,1};
	vec4 color = {1,1,1,1};

    void Start ( ) {
        unsigned int VBO;
        float vertices[] = { 
            // pos      // tex
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 1.0f,

            0.0f, 1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 0.0f
        };

        glGenVertexArrays(1, &this->quadVAO);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindVertexArray(this->quadVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
		
		shader->setInt ( "image", 0 );
    }

	void Update ( ) {
		std::cout << "drawing\n";
		// prepare transformations
		if ( shader == nullptr || texture == nullptr || camera == nullptr ) { return; }

		shader->setMat4  ( "projection", camera->Projkection ( ) );
        shader->setFloat ( "SPRITE_COLUMNS", frames.x );
        shader->setFloat ( "SPRITE_ROWS", frames.y );
        shader->setFloat ( "NUM_OF_SPRITES", (int)(frames.x * frames.y) );

		mat4 model = obj->Get_Model_Mat ( );

		shader->setMat4 ( "model", model );
		shader->setVec4 ( "spriteColor", color );
        shader->setInt  ( "frame", frame );

		texture->Use();

		glBindVertexArray(this->quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
	}
};

#endif