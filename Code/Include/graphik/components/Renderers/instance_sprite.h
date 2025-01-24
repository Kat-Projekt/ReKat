#ifndef INSTANCE_H
#define INSTANCE_H

#include "../../graphik_debugger.hpp"

// instances a series of objekts with Data as the instance data
// in partivular it manages a buffer of type Data from where you can instance objekts
// by default it renders Quads
template < typename Data >
class Instance : public Behaviour {
private:
	std::string _path;

	int _instances;
    unsigned int _quad;
	
    std::string _shader = ""; 
    std::string _texture = "";
    std::string _camera = "";

	vec4 _color = {1,1,1,1};
    struct Memeory_
    {
        /* data */
    };
    

    // every istance is {pos.xyz}{size.xyz}{rot.xyz}
    std::vector < Data > istances_data;
    std::vector <
public:
    void Add_Data ( vec3 pos, vec3 size, vec3 rot ) { }
    void Update_Data ( int index, vec3 pos, vec3 size, vec3 rot ) { }
    void Update_Buffer ( ) { }

    void Start ( ) {
        DEBUG ( 4,"Starting instance sprite");

		_instances = istances_data.size ( ) / 9;
        DEBUG ( 6, "instances: ", _instances );

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

        DEBUG ( 5,"Started tilemap");
    }

	void Update ( ) {
		// prepare transformations
		if ( _shader == "" || _texture == "" || _camera == "" ) { return; }
        auto shader = Manager::Shader_Get ( _shader );
		shader->setMat4  ( "projection", Manager::Camera_Get( _camera )->Projkection ( ) );
 		
		mat4 model = obj->Get_Model_Mat ( );

		shader->setMat4 ( "model", model );
		
		shader->setVec4  ( "spriteColor", _color );
        shader->setFloat ( "SPRITE_COLUMNS", _tile_set.x );
        shader->setFloat ( "SPRITE_ROWS", _tile_set.y );
        shader->setFloat ( "NUM_OF_SPRITES", (int)(_tile_set.x * _tile_set.y) );

		Manager::Texture_Get( _texture )->Use();

		glBindVertexArray(_quad);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, _instances);
        glBindVertexArray(0);
	}

	/* Tilemap* Set ( std::string path, Texture* tile_map, Shader* shader, Camera* camera, vec2 tile_set = {1,1}, vec4 color = {1,1,1,1} ) 
	{ _path = path; _texture = tile_map; _shader = shader; _camera = camera; _tile_set = tile_set; _color = color; return this; } */

    Tilemap* Set ( std::string path, std::string texture, std::string shader, std::string camera, vec2 tile_set = {1,1}, vec4 color = {1,1,1,1} ) 
	{ _path = path; _texture = texture; _shader = shader; _camera = camera; _tile_set = tile_set; _color = color; return this; }
};
#endif