#ifndef TILEMAP_H
#define TILEMAP_H

#include "camera.h"
#include "../Resources/manager.hpp"

#define GL_ERRORS(msg) if (glGetError() != GL_NO_ERROR) { std::cout << "error: " << msg << " at line: " << __LINE__ << " of file: " << __FILE__; } 

class Tilemap : public Behaviour {
private:
	std::string _path;

	int _instances;
    unsigned int _quad;
	
    Shader  *_shader = nullptr; 
    Texture *_texture = nullptr;
	Camera  *_camera = nullptr;

	vec2 _tile_set = {32,32};
	vec4 _color = {1,1,1,1};
public:
    void Start ( ) {
		int H, W;
		_shader->setInt ( "image", 0 );

        std::vector < std::string > Data;
        std::ifstream Data_stream;
        // ensure ifstream objects can throw exceptions:
        Data_stream.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try {
            Data_stream.open ( _path );
            std::string intermediate;
            std::stringstream sData_stream;
            sData_stream << Data_stream.rdbuf();
            Data_stream.close ();
            // Tokenizing 
            H = 0;
            while ( getline ( sData_stream, intermediate, '\n' ) ) { H++; Data.push_back(intermediate); }
        }
        catch(const std::exception& e) { std::cerr << e.what() << '\n'; }
        
		std::vector < int > D;
		D.reserve ( Data.size( ) );
        for ( std::string s : Data ) {
            std::string intermediate;
            std::stringstream sData_stream;
            sData_stream << s;
            // Tokenizing 
            W = 0;
            while ( getline ( sData_stream, intermediate, ',' ) ) { W++; D.push_back( std::stoi(intermediate)); }
        }

		// set up instatiation
		// getting the position and frame of the tile:
		std::vector < vec3 > pos_frame;
		vec2 size = obj->Get_Size();
		for (size_t x = 0; x < W; x++) {
			for (size_t y = 0; y < H; y++) {
				int F = D[y*W + x];
				if ( F >= 0 ) { pos_frame.push_back ( { x, y, F } ); }
			}
		}

		_instances = pos_frame.size ( );

        unsigned int VBO;
		unsigned int instanceVBO;

        float vertices[] = { 
            // pos      // tex
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 1.0f,

            0.0f, 1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 0.0f
        };

		// generating buffers
        glGenVertexArrays(1, &_quad);
        glGenBuffers(1, &VBO);
		glGenBuffers(1, &instanceVBO);

		// loading data to buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * pos_frame.size(), pos_frame.data(), GL_STATIC_DRAW);

		// configuring array buffer
        glBindVertexArray(_quad);
		// per instance data ( pos and tex )
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		// instance data
		glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		// set attribute 1 as an instace array
		glVertexAttribDivisor(1, 1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

	void Update ( ) {
		// prepare transformations
		if ( _shader == nullptr || _texture == nullptr || _camera == nullptr ) { return; }

		_shader->setMat4  ( "projection", _camera->Projkection ( ) );
 		
		mat4 model = obj->Get_Model_Mat ( );

		_shader->setMat4 ( "model", model );
		
		_shader->setVec4  ( "spriteColor", _color );
        _shader->setFloat ( "SPRITE_COLUMNS", _tile_set.x );
        _shader->setFloat ( "SPRITE_ROWS", _tile_set.y );
        _shader->setFloat ( "NUM_OF_SPRITES", (int)(_tile_set.x * _tile_set.y) );

		_texture->Use();

		glBindVertexArray(_quad);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, _instances); // 100 triangles of 6 vertices each
        glBindVertexArray(0);
	}

	Tilemap* Set ( std::string path, Texture* texture, Shader* shader, Camera* camera, vec2 tile_set = {1,1}, vec4 color = {1,1,1,1} ) 
	{ _path = path; _texture = texture; _shader = shader; _camera = camera; _tile_set = tile_set; _color = color; return this; }
};
#endif