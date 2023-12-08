#ifndef SPRITE_H
#define SPRITE_H

#include "shader.h"
#include "texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Sprite {
private:
    Shader       *shader; 
    Texture      *texture;
    unsigned int quadVAO;
    glm::vec2 sprite_dim;
public:
    Sprite ( ) { }
    Sprite ( Shader *shader ) : shader(shader) { Make ( ); }
    Sprite ( Shader *shader, Texture *texture, glm::vec2 sprite_set ) : shader(shader) 
    { Make ( ); Set_Texture(texture,sprite_set); }
    // sprite_set is {columns, rows}
    void Set_uniform ( ) {
        shader->setFloat ( "SPRITE_COLUMNS", sprite_dim.x );
        shader->setFloat ( "SPRITE_ROWS", sprite_dim.y );
        shader->setFloat ( "NUM_OF_SPRITES", (int)(sprite_dim.x * sprite_dim.y) );
    }
    void Set_Texture ( Texture *texture, glm::vec2 sprite_set ) {
        this->texture = texture;
        sprite_dim = sprite_set;
        Set_uniform ( );
    }
    int Make ( )  {
        unsigned int VBO;
        float vertices[] = { 
            // pos      // tex
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f, 

            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f
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
        return 0;
    }

    ~Sprite ( ) { glDeleteVertexArrays(1, &this->quadVAO); }

    void Draw ( Texture *texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color ) {
        // prepare transformations
        Set_uniform ( );
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

        model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
        model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
        model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back

        model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale

        this->shader->setMat4("model", model);

        // render textured quad
        this->shader->setVec3("spriteColor", color);

        glActiveTexture(GL_TEXTURE0);
        this->texture->Bind();

        glBindVertexArray(this->quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    void Draw_frame ( int frame,  glm::vec2 position, glm::vec2 size, float rotate, glm::vec4 color = {1,1,1,1}, glm::vec2 Rot_pivot = {0.5,0.5}) {
        // modify pos based on pivot (alwais on center)
        position += glm::vec2{ Rot_pivot.x * size.x, - Rot_pivot.y * size.y };

        // prepare transformations
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

        model = glm::translate(model, glm::vec3(Rot_pivot.x * size.x, Rot_pivot.y * size.y, 0.0f)); // move origin of rotation to center of quad
        model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
        model = glm::translate(model, glm::vec3(-Rot_pivot.x * size.x, -Rot_pivot.y * size.y, 0.0f)); // move origin back

        model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale
        
        // set uniforms
        this->shader->setMat4 ("model", model);
        this->shader->setVec4 ("spriteColor", color);
        this->shader->setInt ("frame", frame);

        glActiveTexture(GL_TEXTURE0);
        this->texture->Bind();

        glBindVertexArray(this->quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
};

#endif