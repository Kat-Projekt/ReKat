#ifndef OBJECT_UI_H
#define OBJECT_UI_H

#include "object.h"
#include "unordered_map"
#include "text.h"
#include "ReKat.hpp"
#include <memory>

class UI_Object : public Object {
private:
	glm::vec2 pivot = {0,0};
	float ration = 1;

    bool hower = false;
    bool click = false;

    std::string text;

    Sprite *sprite;
    Text *text_renderer;
    int start_frame = 0;

    void (* _call ) ();

    std::unordered_map < std::string, UI_Object* > Sub_Objects;
public:
    UI_Object ( ) { }
    // sprite frames: (relative to base sprite) +0(idle), +1(hower), +2(pressed)
    UI_Object ( std::string _name, std::string _text, Sprite *_sprite, Text *_text_rendere, glm::vec2 _pos, glm::vec2 _size, void(*click_callback)( ), int start_frame = 0 )
    : text(_text), sprite(_sprite), text_renderer(_text_rendere), _call(click_callback), start_frame(start_frame) {
		Move (_pos);
		old_pos = _pos;
		name = _name;
		size = _size; 
		PMove ({0,0});
		altitude = 0.9f;
	}
    ~UI_Object ( ) { }

    void Add_Sub_Object ( std::string name, UI_Object* _sub_obj ) 
    { Sub_Objects.insert ( {name, _sub_obj} ); }
    UI_Object* Get_Sub_Object ( std::string name ) 
    { return Sub_Objects[name]; }

    void Draw ( ) {
		if ( ration != ReKat::grapik::Input::screen_ration ) 
		{ Update_screen_ratio(); }
        if ( !Active ) { return; }
		if ( frame == start_frame ) {
			if ( hower && !click ) { frame = start_frame+1; }
        	else if ( click ) { frame = start_frame+2; }
        	else { frame = start_frame; } 
		}
        
        
        for ( auto o : Sub_Objects )
        { o.second->Draw( ); }

        if ( sprite != nullptr ) 
		{ sprite->Draw_frame ( frame, { pos.x+parent_pos.x-size.x/2, pos.y+parent_pos.y-size.y/2, altitude }, size, rot, color, pivot ); }
        if ( text_renderer != nullptr ) 
		{ text_renderer->RenderText ( text,pos - glm::vec2 (size.x/2, -size.y/2) + parent_pos,size,1 ); }
		frame = start_frame;
    };
    
    void Update_mause_pos ( glm::vec2 _pos ) {        
        // calculate hover
        if ( ( _pos.x - pos.x > -size.x/2 && _pos.x - pos.x < size.x/2 ) && 
             ( _pos.y - pos.y > -size.y/2 && _pos.y - pos.y < size.y/2 ) ) 
        { hower = true; } else { hower = false; }
        old_pos = _pos;
    }
    void Update_mause_action ( bool pressed ) {
        if ( pressed ) {
            if ( hower ) { 
                if (_call != nullptr) 
                { _call(); } 
                click = true; 
            } else { click = false; }
            Update_mause_pos ( old_pos );
        } else {
            click = false;
        }
    }

	// pivot goes from {-100,-100} (bottom rigth) to {100,100} (top left), {0,0} is defualt and points to center
	void Set_postion ( glm::vec2 _pos, glm::vec2 _pivot = {0,0} ) {
		// convert pos to center pivot
		pos = glm::vec2 { _pos.x + _pivot.x * ReKat::grapik::Input::screen_ration, _pos.y + _pivot.y };
		ration = ReKat::grapik::Input::screen_ration;
		pivot = _pivot;
	}
	void Update_screen_ratio ( ) {
		//std::cout << "ration " << ration << " x " << ReKat::grapik::Input::screen_ration << '\n';
		//std::cout << " op: " << pos.x << " x " << pos.y <<'\n';
		pos.x = pos.x - pivot.x * ration + pivot.x * ReKat::grapik::Input::screen_ration;
		ration = ReKat::grapik::Input::screen_ration;
		// std::cout << " np: " << pos.x << " x " << pos.y <<'\n';
	}

    std::string Get_text ( ) { return text; }
    void Add_text ( char _t ) { text+=_t; }
    void Add_text ( std::string _t ) { text+=_t; }
    void Set_text ( std::string _t ) { text=_t; }
};


#endif