#ifndef TEXT_H
#define TEXT_H

#include "../camera.h"
#include "../../resources/shader.h"
#include "../../resources/font.h"

class Text : public Behaviour {
    unsigned int _quad;
	unsigned int _inst;

	std::string _text = "";

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
			std::cout << "{ " << instance_buffer[c*4+0] << ',' <<
			instance_buffer[c*4+1] << ',' <<
			instance_buffer[c*4+2] << ',' <<
			instance_buffer[c*4+3] << " }\n";
		}

		// load data into buffer
		glGenBuffers (1, &_inst);
        glBindBuffer(GL_ARRAY_BUFFER, _inst);
        glBufferData(GL_ARRAY_BUFFER, (sizeof(int)*4) *_text.size(), instance_buffer, GL_STATIC_DRAW);
		// setting
		glBindVertexArray(_quad);
		glVertexAttribPointer(1, 4, GL_INT, GL_FALSE, 4 * sizeof(int), (void*)0);
		// unbind
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
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

        glGenVertexArrays(1, &_quad);
        glGenBuffers(1, &VBO);
		glGenBuffers(1, &_inst);

		// load sprite data ( equal per instance )
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, _inst);
        glBufferData(GL_ARRAY_BUFFER, (sizeof(int)*4), calloc (4,sizeof(int)), GL_STATIC_DRAW);
		// configuring array buffer
        glBindVertexArray(_quad);
		// setting per instance data
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		// setting instance data
		glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, _inst);
		glVertexAttribPointer(1, 4, GL_INT, GL_FALSE, 4 * sizeof(int), (void*)0);
		// set attribute 1 as an instace array
		glVertexAttribDivisor(1, 1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

		_shader->setInt ( "image", 0 );
		Update_Instance_Buffer ( );
	}

    void Update ( ) {
		if ( _text.size() == 0 ) { return; }
		// prepare transformations
		if ( _shader == nullptr || _font == nullptr || _camera == nullptr ) { return; }

		_shader->setMat4  ( "projection", _camera->UI_Projkection ( ) );
        _shader->setFloat ( "SPRITE_COLUMNS", 16 );
        _shader->setFloat ( "SPRITE_ROWS", 8 );
        _shader->setFloat ( "NUM_OF_SPRITES", (int)(8 * 16) );

		mat4 model = obj->Get_Model_Mat ( );

		_shader->setMat4 ( "model", model );
		_shader->setVec4 ( "spriteColor", _color );
        _shader->setInt  ( "frame", 104 );

		_font->Use();

		glBindVertexArray(_quad);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, _text.size());
        glBindVertexArray(0);
	}

	Text * Set ( Font* font, Shader* shader, Camera* camera, vec4 color = {1,1,1,1} ) 
	{ _font = font; _shader = shader; _camera = camera; _color = color; return this; }

	Text * Set ( std::string text ) { 
		if ( _text != text ) {
			_text = text;
			Update_Instance_Buffer ();
		}
		return this;
	}
};

#endif