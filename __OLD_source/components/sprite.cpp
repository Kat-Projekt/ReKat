#include "extensions/graphik/components/sprite.h"

Sprite::Sprite
( ) {
	Informations = { "Sprite", 2.0, "Draws a sprite on screen. It uses the objekt transform proerties for position scale and rotation. The only inportant part to set is the Texture."};
}

void Sprite::Start
( ) {
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
		Manager::Get < Shader > ( _shader )->setInt ( "image", 0 );
	DEBUG ( 5,"Started Sprite");
	DEBUG ( 5, "VBO: ",VBO, " VAO: ", _quad );
}

void Sprite::Update
( ) {
	// prepare transformations
	DEBUG ( 5, "Staring Updating Sprite");
	DEBUG ( 6, "Shader: '", _shader, 
			"' Texture: '", _texture, 
			"' Camera: '", _camera, 
			"' UI rendered '", (_UI_render?"true":"false"), 
			"' Insancer: '", _instacer,
			"' Frame: '", _frame , '\'' );

	if ( _shader == "" || ( _camera == "" && !_UI_render ) )
	{ DEBUG ( 2, "Component not set Correctly" ); return; }

	auto shader = Manager::Get < Shader > ( _shader );

	shader->setMat4  ( "projection", ( _UI_render ? 
	Camera::UI_Projkection ( ) : Manager::Camera_Get ( _camera )->Projkection ( )) );
	DEBUG ( 6, "Updated Camera uniform");

	shader->setFloat ( "SPRITE_COLUMNS", (int)_frames.x );
	shader->setFloat ( "SPRITE_ROWS", (int)_frames.y );
	shader->setFloat ( "NUM_OF_SPRITES", (int)(_frames.x * _frames.y) );
	DEBUG ( 6, "Updated Sprite uniforms");

	mat4 model = obj->Get_Model_Mat ( );
	shader->setMat4 ( "model", model );
	DEBUG ( 6, "Updated Model uniform", model);

	shader->setVec4 ( "spriteColor", _color );
	shader->setInt  ( "frame", _frame );
	DEBUG ( 6, "Updated Frame uniforms");

	glBindVertexArray(_quad); GL_CHECK_ERROR;
	glBindBuffer(GL_ARRAY_BUFFER, VBO); GL_CHECK_ERROR;

	if ( _texture != "" ) 
	{ Manager::Get < Texture > ( _texture )->Use(); }

	if ( _instacer != "" ) {
		auto I = Manager::Get < Instance > ( _instacer );
		I->Use ( );
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, I->Instances ( ) ); GL_CHECK_ERROR;
		DEBUG ( 4,"Drawing ", I->Instances( ), " instances");
	} else {
		glDrawArrays(GL_TRIANGLES, 0, 6); GL_CHECK_ERROR;
	}

	glBindVertexArray(0); GL_CHECK_ERROR;
	
	DEBUG (5, "Drawn Sprite" );
}

void Sprite::Delete ( ) {
	glDeleteVertexArrays(1, &_quad);
	glDeleteBuffers(1, &VBO);
}

Sprite* Sprite::Set  (
	std::string texture,
	std::string shader,
	std::string camera,
	ivec2 frames,
	int frame,
	vec4 color,
	bool UI_sprite,
	std::string instancer
) {
	_texture = texture;
	_shader = shader;
	_camera = camera;
	_instacer = instancer;
	_frames = frames;
	_frame = frame;
	_color = color;
	_UI_render = UI_sprite;

	return this;
}

Sprite* Sprite::Set ( const char* texture ) {
	_texture = (std::string)texture;
	return this;
}

Sprite* Sprite::Set
( bool UI_sprite )
{ _UI_render = UI_sprite; return this; }
Sprite* Sprite::Set ( int frame )
{ this->_frame = frame; return this; }
Sprite* Sprite::Set ( vec4 color ) 
{ _color = color; return this; }

vec4 * Sprite::Expose_Color
( ) { return &_color; }