#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

/*	manifesto
Behaves like a separate render envoirement
renders the objekts "_to_render" inside the canvas
and saves it in a texture
than it behaves like a sprite Behaviour
*/

// #include <stb_image_write.h>
#include "objekt/objekt.hpp"

class Framebuffer : public Behaviour {
private:
	void Create_Frame_Buffer ( );
	void Create_Sprite ( );

	// where to do the main render call 
	// this objekt must be exluded from the other callers: like manager
	Objekt * _to_render;

	// risolution
	unsigned int _width = 0;
	unsigned int _heigth = 0;

	// sprite part
	std::string _shader = "";
	std::string _camera = "";

	// render links
	unsigned int FBO = -1;
	unsigned int TEX = -1;
	unsigned int VAO = -1;
	unsigned int VBO = -1;
	unsigned int RBO = -1;

	bool _resize = false;
	bool _ready_to_render = false;
	bool _UI_render = true;
public:
	float _aspect_ratio = -1;

	void Start ( );
	void Update ( );
	void Fixed_Update ( );
	void Delete ( );
	void Save ( std::string file );

	// set resolution of the buffer
	Framebuffer& Set (
		unsigned int width,
		unsigned int heigth
	);
	// set parametes for the sprite part
	// if left empty it behaves like a texture
	Framebuffer& Set (
		std::string shader,
		std::string camera = "",
		bool UI_sprite = true
	);
	// sender render target
	Framebuffer& Set ( Objekt* to_render );
};

#endif