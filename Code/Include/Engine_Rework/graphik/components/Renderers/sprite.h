#ifndef SPRITE_H
#define SPRITE_H

#include "camera.h"
#include "../Resources/manager.hpp"

class Sprite : public Behaviour {
private:
	bool _UI_render = false;
    unsigned int _quad;
    Texture *_texture = nullptr;
    Shader  *_shader = nullptr; 
	Camera  *_camera = nullptr;
	ivec2 _frames = {1,1};
	vec4 _color = {1,1,1,1};
public:
	int frame = 0;

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

        glGenVertexArrays(1, &_quad);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindVertexArray(_quad);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
		
		_shader->setInt ( "image", 0 );
    }

	void Update ( ) {
		// prepare transformations
		if ( _shader == nullptr || _texture == nullptr || _camera == nullptr ) { return; }

		_shader->setMat4  ( "projection", ( _UI_render ? _camera->UI_Projkection ( ) : _camera->Projkection ( )) );
        _shader->setFloat ( "SPRITE_COLUMNS", _frames.x );
        _shader->setFloat ( "SPRITE_ROWS", _frames.y );
        _shader->setFloat ( "NUM_OF_SPRITES", (int)(_frames.x * _frames.y) );

		mat4 model = obj->Get_Model_Mat ( );

		_shader->setMat4 ( "model", model );
		_shader->setVec4 ( "spriteColor", _color );
        _shader->setInt  ( "frame", frame );

		_texture->Use();

		glBindVertexArray(_quad);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
	}

	Sprite* Set ( Texture* texture, Shader* shader, Camera* camera, ivec2 frames = {1,1}, int frame = 0, vec4 color = {1,1,1,1} ) 
	{ _texture = texture; _shader = shader; _camera = camera;
	_frames = frames; this->frame = frame; _color = color; return this; }

	Sprite* Set ( bool UI_sprite ) { _UI_render = UI_sprite; return this; }
};

#endif