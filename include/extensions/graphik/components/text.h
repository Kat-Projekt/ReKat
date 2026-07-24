#ifndef TEXT_H
#define TEXT_H

#include "camera.h"
#include "../manager.hpp"

class Text : public Behaviour {
public:
	enum class ALIGNMENT {
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
	ALIGNMENT _text_align_h = ALIGNMENT::CENTER;
	ALIGNMENT _text_align_v = ALIGNMENT::CENTER;
	bool _x_warp = false;
	int instances = 0;

	vec4 _color = {1,1,1,1};
	bool _is_up_to_date = false;

	std::string _camera = "";
	std::string _shader = ""; 
	std::string _font = "";

	void Update_Instance_Buffer ( );
public:
	Text ( );

	void Start ( );
	void Update ( );
	void Delete ( );

	Text * Set ( std::string font, std::string shader, std::string camera = "", vec4 color = {1,1,1,1} );
	Text * Set ( std::string text, ALIGNMENT al_horizontal, ALIGNMENT al_vertical = ALIGNMENT::CENTER, bool x_warp = false );
	Text * Set ( vec4 color );
	Text * Set ( std::string text );

	std::string * Get ( );
	vec4 * Expose_Color ( );
};

#endif