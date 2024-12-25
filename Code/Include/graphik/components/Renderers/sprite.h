#ifndef SPRITE_H
#define SPRITE_H

#include "../camera.h"
#include "../../resources/manager.hpp"

#include "../../graphik_debugger.hpp"

class Sprite : public Behaviour {
private:
	bool _UI_render = false;
    unsigned int _quad;
    unsigned int VBO;
    Texture *_texture = nullptr;
    Shader  *_shader = nullptr; 
	Camera  *_camera = nullptr;
	ivec2 _frames = {1,1};
	vec4 _color = {1,1,1,1};
public:
	int frame = 0;

    void Start ( ) {
        DEBUG ( 4, "Starting Sprite");
        float vertices[] = { 
            // pos      // tex
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 1.0f,

            0.0f, 1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 0.0f
        };

        glGenVertexArrays(1, &_quad); GL_CHECK_ERROR;
        glGenBuffers(1, &VBO); GL_CHECK_ERROR;

        // binding
        glBindVertexArray(_quad); GL_CHECK_ERROR;
        glBindBuffer(GL_ARRAY_BUFFER, VBO); GL_CHECK_ERROR;
        
        // loading and layout
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); GL_CHECK_ERROR;
        glEnableVertexAttribArray(0); GL_CHECK_ERROR;
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); GL_CHECK_ERROR;
        
        // unbind
        // glBindBuffer(GL_ARRAY_BUFFER, 0); GL_CHECK_ERROR;
        // glBindVertexArray(0); GL_CHECK_ERROR;
		
        DEBUG ( 6,"setting shader");
		_shader->setInt ( "image", 0 );
        DEBUG ( 5,"Started Sprite");
        DEBUG ( 5, "VBO: ",VBO, " VAO: ", _quad );
    }

	void Update ( ) {
		// prepare transformations
        DEBUG ( 5, "Staring Updating Sprite");
        DEBUG ( 6, _shader, " ", _texture, " ", _camera, " ", _UI_render );
		if ( _shader == nullptr || _texture == nullptr || ( _camera == nullptr && !_UI_render ) ) { DEBUG ( 2, "Component not set Correctly" ); return; }

		_shader->setMat4  ( "projection", ( _UI_render ? Camera::UI_Projkection ( ) : _camera->Projkection ( )) );
        DEBUG ( 6, "Updated Camera uniform");

        _shader->setFloat ( "SPRITE_COLUMNS", (int)_frames.x );
        _shader->setFloat ( "SPRITE_ROWS", (int)_frames.y );
        _shader->setFloat ( "NUM_OF_SPRITES", (int)(_frames.x * _frames.y) );
        DEBUG ( 6, "Updated Sprite uniforms");

		mat4 model = obj->Get_Model_Mat ( );
		_shader->setMat4 ( "model", model );
        DEBUG ( 6, "Updated Model uniform");

		_shader->setVec4 ( "spriteColor", _color );
        _shader->setInt  ( "frame", frame );
        DEBUG ( 6, "Updated Frame uniforms");

		glBindVertexArray(_quad); GL_CHECK_ERROR;
        glBindBuffer(GL_ARRAY_BUFFER, VBO); GL_CHECK_ERROR;
		_texture->Use();
        DEBUG ( 5, "VBO: ",VBO, " VAO: ", _quad );
        glDrawArrays(GL_TRIANGLES, 0, 6); GL_CHECK_ERROR;
        glBindVertexArray(0); GL_CHECK_ERROR;
        
        DEBUG (5, "Drawn Sprite");
	}

	Sprite* Set ( Texture* texture, Shader* shader, Camera* camera = nullptr, ivec2 frames = {1,1}, int frame = 0, vec4 color = {1,1,1,1}, bool UI_sprite = false ) 
	{ _texture = texture; _shader = shader; _camera = camera;
	_frames = frames; this->frame = frame; _color = color; _UI_render = UI_sprite; return this; }

    Sprite* Set ( std::string texture, std::string shader, std::string camera = "", ivec2 frames = {1,1}, int frame = 0, vec4 color = {1,1,1,1}, bool UI_sprite = false ) 
	{ _texture = Manager::Texture_Get ( texture ); _shader = Manager::Shader_Get ( shader ); _camera = Manager::Camera_Get ( camera );
	_frames = frames; this->frame = frame; _color = color; _UI_render = UI_sprite; return this; }

	Sprite* Set ( bool UI_sprite ) { _UI_render = UI_sprite; return this; }
};

#endif