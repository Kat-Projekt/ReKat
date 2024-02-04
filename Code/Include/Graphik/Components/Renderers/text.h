#ifndef TEXT_H
#define TEXT_H

#include "../camera.h"
#include "../../Resources/font.h"

class Text : public Behaviour {
	std::string _text = "";

	Camera  *_camera = nullptr;
    Shader  *_shader = nullptr; 
	Font *_font = nullptr;

	vec4 _color = {1,1,1,1};
public:
    virtual void Start ( ) { }
    virtual void Update ( ) {
		
	}

	Text * Set ( Font* font, Shader* shader, Camera* camera, vec4 color = {1,1,1,1} ) 
	{ _font = font; _shader = shader; _camera = camera; _color = color; return this; }

	Text * Set ( std::string text ) 
	{ _text = text; return this; }
};

#endif