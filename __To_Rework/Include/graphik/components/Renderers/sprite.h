#ifndef SPRITE_H
#define SPRITE_H

#include "../camera.h"
#include "../../resources/manager.hpp"
#include "../../graphik_debugger.hpp"

// default shaders
#include "default_shaders/sprite/sprite.s.h"

class Sprite : public Behaviour {
private:
	bool _UI_render = true;
	unsigned int _quad;
	unsigned int VBO;
	std::string _instacer = "";
	std::string _texture = "";
	std::string _shader = ""; 
	std::string _camera = "";
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
			if ( _shader == "" ) { _shader = "sprite_shader_default"; }
			Manager::Shader_Get ( _shader )->setInt ( "image", 0 );
		DEBUG ( 5,"Started Sprite");
		DEBUG ( 5, "VBO: ",VBO, " VAO: ", _quad );
	}

	void Update ( ) {
		// prepare transformations
		DEBUG ( 5, "Staring Updating Sprite");
		DEBUG ( 6, "Shader: '", _shader, 
					"' Texture: '", _texture, 
					"' Camera: '", _camera, 
					"' UI rendered '", (_UI_render?"true":"false"), 
					"' 'Insancer: '", _instacer, '\'' );

		if ( _shader == "" || ( _camera == "" && !_UI_render ) )
		{ DEBUG ( 2, "Component not set Correctly" ); return; }

		auto shader = Manager::Shader_Get ( _shader );

		shader->setMat4  ( "projection", ( _UI_render ? 
		Camera::UI_Projkection ( ) : Manager::Camera_Get( _camera )->Projkection ( )) );
		DEBUG ( 6, "Updated Camera uniform");

		shader->setFloat ( "SPRITE_COLUMNS", (int)_frames.x );
		shader->setFloat ( "SPRITE_ROWS", (int)_frames.y );
		shader->setFloat ( "NUM_OF_SPRITES", (int)(_frames.x * _frames.y) );
		DEBUG ( 6, "Updated Sprite uniforms");

		mat4 model = obj->Get_Model_Mat ( );
		shader->setMat4 ( "model", model );
		DEBUG ( 6, "Updated Model uniform");

		shader->setVec4 ( "spriteColor", _color );
		shader->setInt  ( "frame", frame );
		DEBUG ( 6, "Updated Frame uniforms");

		glBindVertexArray(_quad); GL_CHECK_ERROR;
		glBindBuffer(GL_ARRAY_BUFFER, VBO); GL_CHECK_ERROR;

		if ( _texture != "" ) 
		{ Manager::Texture_Get( _texture )->Use(); }
			
		if ( _instacer != "" ) {
			auto I = Manager::Get < Instance > ( _instacer );
			I->Use ( );
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, I->Instances ( ) ); GL_CHECK_ERROR;
			DEBUG ( 4,"Drawing ", I->Instances( ), " instances");
		} else {
			glDrawArrays(GL_TRIANGLES, 0, 6); GL_CHECK_ERROR;
		}

		glBindVertexArray(0); GL_CHECK_ERROR;
		
		DEBUG (5, "Drawn Sprite");
	}

    void Delete ( ) {
        glDeleteVertexArrays(1, &_quad);
        glDeleteBuffers(1, &VBO);
    }

	/* Sprite* Set ( Texture* texture, Shader* shader, Camera* camera = nullptr, ivec2 frames = {1,1}, int frame = 0, vec4 color = {1,1,1,1}, bool UI_sprite = false ) 
	{ _texture = texture; _shader = shader; _camera = camera;
	_frames = frames; this->frame = frame; _color = color; _UI_render = UI_sprite; return this; } */

    Sprite* Set ( std::string texture, std::string shader, std::string camera = "", ivec2 frames = {1,1}, int frame = 0, vec4 color = {1,1,1,1}, bool UI_sprite = false, std::string instancer = "" ) 
	{ _texture = texture; _shader = shader; _camera = camera; _instacer = instancer;
	_frames = frames; this->frame = frame; _color = color; _UI_render = UI_sprite; return this; }
    Sprite* Set(const char* texture) { _texture = (std::string)texture; return this; }

	Sprite* Set ( bool UI_sprite ) { _UI_render = UI_sprite; return this; }
	Sprite* Set ( int frame ) { this->frame = frame; return this; }

    Sprite* Set ( vec4 color = {1,1,1,1} ) 
	{ _color = color; return this; }

	vec4 * Expose_Color ( ) { return &_color; }
};

#endif