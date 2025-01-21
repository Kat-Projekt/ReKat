#ifndef TEXT_H
#define TEXT_H

#include "../camera.h"
#include "../../resources/manager.hpp"

#define HORIZONTAL_LEFT 0x1
#define HORIZONTAL_CENTER 0x2
#define HORIZONTAL_RIGTH 0x4
#define VERTICAL_LEFT 0x10
#define VERTICAL_CENTER 0x20
#define VERTICAL_RIGTH 0x40


class Text : public Behaviour {
public:
	enum ALIGNMENT {
		RIGHT,
		CENTER,
		LEFT,
		BOTTOM,
		TOP
	};
private:
    unsigned int _quad;
	unsigned int _inst;
	unsigned int VBO;
	
	std::string _text = "";
	std::string _new_text = "";
	ALIGNMENT _text_align_h;
	ALIGNMENT _text_align_v;
	bool _x_warp = false;
	int instances = 0;

	vec4 _color = {1,1,1,1};
	bool is_up_to_date = false;

	std::string _camera = "";
	std::string _shader = ""; 
	std::string _font = "";

public:
	void Update_Instance_Buffer ( ) {
		// only one update for the buffer per frame
		if ( is_up_to_date ) { return; }
		is_up_to_date = true;
		
        DEBUG ( 5, "Updating Instance buffer Text");

		if ( _font == "" ) { return; }
		auto font = Manager::Font_Get ( _font );
		// contains glyph texure index width and pos in pixels
		// {index,width,x,y} {index,width,x,y} {69'E',10,0,0} {32' ',20,10,0}
		int* instance_buffer = (int*) calloc (_text.size()*4, sizeof(int));
		
		std::vector < int > comulative_x;
		comulative_x.push_back ( 0 );
		int new_lines = 0;
		if ( _text_align_h != RIGHT ) {
			// calculate comulative_x
			for ( size_t c = 0; c < _text.size(); c++ )	{
				if ( _text[c] == 32 ) 
				{ comulative_x[new_lines] += font-> Get_Heigth ( ) / 2; continue; }
				if ( _text[c] == '\n' ) 
				{ new_lines++; comulative_x.push_back ( 0 ); DEBUG ( 4, "Fund new line" ); continue; }

				comulative_x[new_lines] += font-> char_widths[(int)_text[c]];
			}
		}
		for ( auto i = 0; i <= new_lines; i++ ) {
			switch ( _text_align_h ) {
				case CENTER: comulative_x[i] = comulative_x[i] = ( obj->Get_Size ( ).x - comulative_x[i] ) * 0.5; break;
				case RIGHT: comulative_x[i] = obj->Get_Size ( ).x * 0.5; break;
				case LEFT: comulative_x[i] = - comulative_x[i] + obj->Get_Size ( ).x * 0.5; break;
			}
		}
		int vertical_m = new_lines * font->Get_Heigth ( );
		if ( new_lines != 0 ) {
			switch ( _text_align_h ) {
				case CENTER: vertical_m *= 0.5; break;
				case TOP: vertical_m = 0; break;
				case BOTTOM: break;
			}
		}

		int new_lines_counter = 0;
		int vertical_y = font->Get_Heigth ( );
		for ( size_t c = 0; c < _text.size(); c++ )	{
			instance_buffer[c*4+0] = _text[c];
			instance_buffer[c*4+3] = vertical_m - vertical_y * new_lines_counter;
			// space 'glyph'
			if ( _text[c] == 32 ) {
				instance_buffer[c*4+1] = font-> Get_Heigth ( ) / 2;
				instance_buffer[c*4+2] = comulative_x[new_lines_counter];
				comulative_x[new_lines_counter] += font-> Get_Heigth ( ) / 2;
				continue;
			}
			if ( _text[c] == '\n' ) { DEBUG ( 4, "new line counted" ); new_lines_counter++; continue; }

			instance_buffer[c*4+1] = font-> char_widths[(int)_text[c]];
			instance_buffer[c*4+2] = comulative_x[new_lines_counter];
			comulative_x[new_lines_counter] += font-> char_widths[(int)_text[c]];
		}
		for (size_t c = 0; c < _text.size(); c++) {
			DEBUG ( 6, "{ ",instance_buffer[c*4+0], ',',
							instance_buffer[c*4+1], ',',
							instance_buffer[c*4+2], ',',
							instance_buffer[c*4+3], " }" );
		}

		// emply the buffer
		if ( glIsBuffer ( _inst ) ) 
		{ glDeleteBuffers ( 1, &_inst ); GL_CHECK_ERROR; DEBUG (5, "Cleared Buffer"); }
		
		// load data into buffer
		instances = _text.size ( );
		glGenBuffers (1, &_inst); GL_CHECK_ERROR;
        glBindBuffer(GL_ARRAY_BUFFER, _inst); GL_CHECK_ERROR;
        glBufferData(GL_ARRAY_BUFFER, (sizeof(int)*4) * instances, instance_buffer, GL_STATIC_DRAW); GL_CHECK_ERROR;
		DEBUG (5, "load data into buffer");
		// setting
		glBindVertexArray(_quad); GL_CHECK_ERROR;
		glVertexAttribPointer(1, 4, GL_INT, GL_FALSE, 4 * sizeof(int), (void*)0); GL_CHECK_ERROR;
		DEBUG (5,"setted vertex array");
		// unbind
        glBindVertexArray(0); GL_CHECK_ERROR;
        glBindBuffer(GL_ARRAY_BUFFER, 0); GL_CHECK_ERROR;
		DEBUG (4, "Updated Istance Buffer" );
	}

    void Start ( ) {
        DEBUG ( 5, "Staring Text");
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

		Manager::Shader_Get( _shader ) ->setInt ( "image", 0 );

		Update_Instance_Buffer ( );
	}

    void Update ( ) {
		// make sures that the buffer is update once per frame
		is_up_to_date = false;
		if ( _new_text != _text ) 
		{ _text = _new_text; Update_Instance_Buffer ( ); }

		if ( _text.size() == 0 ) { return; }
        DEBUG ( 5, "Staring Updating Text");
		// prepare transformations
		if ( _shader == "" || _font == "" ) { DEBUG ( 1, "Component not set Correctly" ); return; }
		auto shader = Manager::Shader_Get ( _shader );

		if ( _camera == "" ) 
		{ shader->setMat4  ( "projection", Camera::UI_Projkection ( ) ); }
		else
		{ shader->setMat4  ( "projection", Manager::Camera_Get( _camera )->Projkection ( ) ); }
		
		DEBUG ( 6, "Updated Camera uniform");

        shader->setFloat ( "SPRITE_COLUMNS", 16 );
        shader->setFloat ( "SPRITE_ROWS", 8 );
        shader->setFloat ( "NUM_OF_SPRITES", (int)(8 * 16) );
		DEBUG ( 6, "Updated Camera uniform");

		mat4 model = obj->Get_Model_Mat ( );
		shader->setMat4 ( "model", model );
        DEBUG ( 6, "Updated Model uniform");

		shader->setVec4 ( "spriteColor", _color );
        DEBUG ( 6, "Updated Frame uniforms");

		Manager::Font_Get( _font )->Use();

		glBindVertexArray(_quad); GL_CHECK_ERROR;
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, instances); GL_CHECK_ERROR;
        glBindVertexArray(0); GL_CHECK_ERROR;
		
        DEBUG (5, "Drawn Text");
	}

    void Delete ( ) {
        glDeleteVertexArrays(1, &_quad);
        glDeleteBuffers(1, &_inst);
        glDeleteBuffers(1, &VBO);
    }

	/* Text * Set ( Font* font, Shader* shader, Camera* camera = nullptr, vec4 color = {1,1,1,1} ) 
	{ _font = font; _shader = shader; _camera = camera; 
	_color = color; return this; } */

	Text * Set ( std::string font, std::string shader, std::string camera = "", vec4 color = {1,1,1,1} ) 
	{ _font = font; _shader = shader; _camera = camera; 
	_color = color; return this; }

	Text * Set ( std::string text, ALIGNMENT al_horizontal = CENTER, ALIGNMENT al_vertical = CENTER, bool x_warp = false ) {
		_new_text = text;
		_text_align_h = al_horizontal;
		_text_align_v = al_vertical;
		_x_warp = x_warp;
		DEBUG (3, "Text changed: ", text );
		return this;
	}

	Text * Set ( vec4 color ) 
	{ _color = color; return this; }

	vec4 * Expose_Color ( ) { return &_color; }
};

#endif