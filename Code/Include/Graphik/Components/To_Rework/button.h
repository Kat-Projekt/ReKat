#ifndef BUTTON
#define BUTTON
#include "object.h"

class Button {
private:
    std::string name;

    bool hower = false;
    bool click = false;
    int  start_frame = 0;
    void (* _call ) ();
    glm::vec2 old_pos;
    Object obj;
public:
    // sprite frames: (relative to base sprite) +0(idle), +1(hower), +2(pressed)
    Button ( ) { }
    Button ( std::string _name, std::string text, Sprite *sprite, glm::vec2 _pos, glm::vec2 _size, void(*click_callback)( ), int start_frame = 0 )
    : name(_name), _call(click_callback), start_frame(start_frame) {
        obj = Object ( _name, sprite, _pos, _size );
    }
    ~Button ( ) { }

    void Draw ( ) { 
             if ( hower && !click ) { obj.Change_frame (start_frame+1); }
        else if ( click ) { obj.Change_frame (start_frame+2); }
        else { obj.Change_frame(start_frame); }
        obj.Draw( ); 
    };
    void Update_mause ( glm::vec2 pos ) {
        // calculate hover
        if ( ( pos.x - obj.Get_pos( ).x > 0 && pos.x - obj.Get_pos( ).x < obj.Get_size().x ) && 
             ( pos.y - obj.Get_pos( ).y > 0 && pos.y - obj.Get_pos( ).y < obj.Get_size().y ) ) 
        { hower = true; } else { hower = false; }
        old_pos = pos;
    }
    void Pressed ( ) {
        if ( hower ) { _call(); click = true; } else { click = false; }
        Update_mause ( old_pos );
    }
    void Release ( ) 
    { click = false; }
};

#endif