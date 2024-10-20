#ifndef TEXT_H
#define TEXT_H

#include "../camera.h"
#include "../../resources/manager.hpp"

class Text : public Behaviour {
public:
	enum ALIGNMENT {
		RIGHT,
		CENTER,
		LEFT
	};
private:
    unsigned int _quad;
	unsigned int _inst;
	
	
	std::string _text = "";
	ALIGNMENT _text_aligne;
	bool _x_warp = false;


	Camera  *_camera = nullptr;
    Shader  *_shader = nullptr; 
	Font *_font = nullptr;

	vec4 _color = {1,1,1,1};

public:
	void Update_Instance_Buffer ( ) {
		if ( _font == nullptr ) { return; }
		// contains glyph texure index width and pos in pixels
		// {index,width,x,y} {index,width,x,y} {69'E',10,0,0} {32' ',20,10,0}
		int* instance_buffer = (int*) calloc (_text.size()*3, sizeof(int));
		
		int comulative_x = 0;
		if ( _text_aligne != RIGHT ) {
			// calculate comulative_x
			for ( size_t c = 0; c < _text.size(); c++ )	{
				if ( _text[c] == 32 ) 
				{ comulative_x += _font->Get_Heigth()/2; }

				comulative_x += _font->char_widths[_text[c]];
			}
		}

		switch ( _text_aligne ) {
			case CENTER: comulative_x = comulative_x = ( obj->Get_Size ( ).x - comulative_x ) * 0.5; break;
			case RIGHT: comulative_x = obj->Get_Size ( ).x * 0.5; break;
			case LEFT: comulative_x = - comulative_x + obj->Get_Size ( ).x * 0.5; break;
		}


		for ( size_t c = 0; c < _text.size(); c++ )	{
			instance_buffer[c*4+0] = _text[c];
			instance_buffer[c*4+3] = 0;

			// space 'glyph'
			if ( _text[c] == 32 ) {
				instance_buffer[c*4+1] = _font->Get_Heigth()/2;
				instance_buffer[c*4+2] = comulative_x;
				comulative_x += _font->Get_Heigth()/2;
			}
			instance_buffer[c*4+1] = _font->char_widths[_text[c]];
			instance_buffer[c*4+2] = comulative_x;
			comulative_x += _font->char_widths[_text[c]];
		}
		for (size_t c = 0; c < _text.size(); c++) {
			DEBUG ( 6, "{ ",instance_buffer[c*4+0], ',',
							instance_buffer[c*4+1], ',',
							instance_buffer[c*4+2], ',',
							instance_buffer[c*4+3], " }" );
		}

		// load data into buffer
		glGenBuffers (1, &_inst); GL_CHECK_ERROR;
        glBindBuffer(GL_ARRAY_BUFFER, _inst); GL_CHECK_ERROR;
        glBufferData(GL_ARRAY_BUFFER, (sizeof(int)*4) *_text.size(), instance_buffer, GL_STATIC_DRAW); GL_CHECK_ERROR;
		// setting
		glBindVertexArray(_quad); GL_CHECK_ERROR;
		glVertexAttribPointer(1, 4, GL_INT, GL_FALSE, 4 * sizeof(int), (void*)0); GL_CHECK_ERROR;
		// unbind
        glBindVertexArray(0); GL_CHECK_ERROR;
        glBindBuffer(GL_ARRAY_BUFFER, 0); GL_CHECK_ERROR;
		DEBUG (4, "Updated Istance Buffer" );
	}

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

        glGenVertexArrays(1, &_quad); GL_CHECK_ERROR;
        glGenBuffers(1, &VBO); GL_CHECK_ERROR;
		glGenBuffers(1, &_inst); GL_CHECK_ERROR;

		// load sprite data ( equal per instance )
        glBindBuffer(GL_ARRAY_BUFFER, VBO); GL_CHECK_ERROR;
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); GL_CHECK_ERROR;
        glBindBuffer(GL_ARRAY_BUFFER, _inst); GL_CHECK_ERROR;
        glBufferData(GL_ARRAY_BUFFER, (sizeof(int)*4), calloc (4,sizeof(int)), GL_STATIC_DRAW); GL_CHECK_ERROR;
		// configuring array buffer
        glBindVertexArray(_quad); GL_CHECK_ERROR;
		// setting per instance data
        glEnableVertexAttribArray(0); GL_CHECK_ERROR;
        glBindBuffer(GL_ARRAY_BUFFER, VBO); GL_CHECK_ERROR;
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); GL_CHECK_ERROR;
		// setting instance data
		glEnableVertexAttribArray(1); GL_CHECK_ERROR;
        glBindBuffer(GL_ARRAY_BUFFER, _inst); GL_CHECK_ERROR;
		glVertexAttribPointer(1, 4, GL_INT, GL_FALSE, 4 * sizeof(int), (void*)0); GL_CHECK_ERROR;
		// set attribute 1 as an instace array
		glVertexAttribDivisor(1, 1); GL_CHECK_ERROR;

        glBindBuffer(GL_ARRAY_BUFFER, 0); GL_CHECK_ERROR;
        glBindVertexArray(0); GL_CHECK_ERROR;

		_shader->setInt ( "image", 0 );
		Update_Instance_Buffer ( );
	}

    void Update ( ) {
		if ( _text.size() == 0 ) { return; }
        DEBUG ( 5, "Staring Updating Text");
		// prepare transformations
		if ( _shader == nullptr || _font == nullptr || _camera == nullptr ) { DEBUG ( 1, "Component not set Correctly" ); return; }

		_shader->setMat4  ( "projection", _camera->UI_Projkection ( ) );
		DEBUG ( 6, "Updated Camera uniform");

        _shader->setFloat ( "SPRITE_COLUMNS", 16 );
        _shader->setFloat ( "SPRITE_ROWS", 8 );
        _shader->setFloat ( "NUM_OF_SPRITES", (int)(8 * 16) );
		DEBUG ( 6, "Updated Camera uniform");

		mat4 model = obj->Get_Model_Mat ( );
		_shader->setMat4 ( "model", model );
        DEBUG ( 6, "Updated Model uniform");

		_shader->setVec4 ( "spriteColor", _color );
        _shader->setInt  ( "frame", 104 );
        DEBUG ( 6, "Updated Frame uniforms");

		_font->Use();

		glBindVertexArray(_quad); GL_CHECK_ERROR;
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, _text.size()); GL_CHECK_ERROR;
        glBindVertexArray(0); GL_CHECK_ERROR;
		
        DEBUG (5, "Drawn Text");
	}

	Text * Set ( Font* font, Shader* shader, Camera* camera, vec4 color = {1,1,1,1} ) 
	{ _font = font; _shader = shader; _camera = camera; 
	_color = color; return this; }

	Text * Set ( std::string font, std::string shader, Camera* camera, vec4 color = {1,1,1,1} ) 
	{ _font = Manager::Font_Get ( font ); _shader = Manager::Shader_Get ( shader ); _camera = camera; 
	_color = color; return this; }

	Text * Set ( std::string text, ALIGNMENT al = RIGHT, bool x_warp = false ) {
		if ( _text != text ) {
			_text = text;
			_text_aligne = al;
			_x_warp = x_warp;
			Update_Instance_Buffer ( );
		}
		DEBUG (3, "Text changed: ", text );
		return this;
	}
};

#endif