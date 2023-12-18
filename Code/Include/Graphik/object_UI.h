#ifndef OBJECT_UI_H
#define OBJECT_UI_H

#include "sprite.h"
#include "unordered_map"
#include "behaviour.h"
#include "text.h"
#include <memory>

class UI_Object {
private:
    bool Active = true;
    bool hower = false;
    bool click = false;

    glm::vec2 parent_pos = {0,0};
    glm::vec2 pivot = {0.5,0.5};
    float altitude = 0;
    glm::vec2 pos;
    glm::vec2 size;
    float rot = 0;

    std::string name;
    std::string text;

    Sprite *sprite;
    glm::vec4 color = {1,1,1,1};
    Text *text_renderer;
    int start_frame = 0;
    int frame = 0;

    void (* _call ) ();
    glm::vec2 old_pos;

    std::unordered_map < std::string, UI_Object* > Sub_Objects;
    std::vector < std::shared_ptr<Behaviour> > components;
public:
    UI_Object ( ) { }
    // sprite frames: (relative to base sprite) +0(idle), +1(hower), +2(pressed)
    UI_Object ( std::string _name, std::string _text, Sprite *_sprite, Text *_text_rendere, glm::vec2 _pos, glm::vec2 _size, void(*click_callback)( ), int start_frame = 0 )
    : name(_name), text(_text), sprite(_sprite), text_renderer(_text_rendere), pos(_pos), size(_size), _call(click_callback), start_frame(start_frame) { }
    ~UI_Object ( ) { }

    void Add_Sub_Object ( std::string name, UI_Object* _sub_obj ) 
    { Sub_Objects.insert ( {name, _sub_obj} ); }
    UI_Object* Get_Sub_Object ( std::string name ) 
    { return Sub_Objects[name]; }

    void PMove ( glm::vec2 move ) { parent_pos = move; }
    void PDMove ( glm::vec2 move ) { parent_pos += move; }
    void DMove ( glm::vec2 move ) { 
        pos += move;
        for ( auto o : Sub_Objects ) 
        { o.second->PDMove(move); }
    }
    void Move  ( glm::vec2 move ) { 
        pos = move;
        for ( auto o : Sub_Objects ) 
        { o.second->PMove(move); }
    }

    void Rotate ( float rotation ) { rot = rotation; }
    void Set_Rotation_Pivot ( glm::vec2 pivot ) { this->pivot = pivot; }

    glm::vec2 Get_pos ( ) { return pos - glm::vec2{-size.x*0.5,size.y*0.5}; }
    glm::vec2 Get_size ( ) { return size; }

    void Draw ( ) {
        if ( !Active ) { return; }
        if ( hower && !click ) { frame = start_frame+1; }
        else if ( click ) { frame = start_frame+2; }
        else { frame = start_frame; }
        
        for ( auto o : Sub_Objects )
        { o.second->Draw( ); }

        if ( sprite != nullptr ) { sprite->Draw_frame ( frame, { pos.x+parent_pos.x-size.x, pos.y+parent_pos.y, altitude }, size, rot, color, pivot ); }
        if ( text_renderer != nullptr ) { 
            text_renderer->RenderText(text,glm::vec2(pos.x, -pos.y) - glm::vec2 (size.x/2, -size.y/2),size,1); 
            std::cout << "text: " << text << " " << pos.x << " x " << pos.y << " " << size.x << " x " << size.y << '\n';
        }
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

    template < class C > 
    C* Add_component ( ) {
        C* c = new C ( );
        c->name = name;
        if ( std::is_base_of<Behaviour, C>::value ) 
        { components.push_back( std::shared_ptr < Behaviour > ( c ) ); }
        return c;
    }

    template < class C > 
    C* Add_component ( C* c ) {
        c->name = name;
        if ( std::is_base_of<Behaviour, C>::value ) 
        { components.push_back( std::shared_ptr < Behaviour > ( c ) ); }
        return c;
    }

    void Update ( ) { 
        for ( auto c : components ) 
        { c->Update(); }
    }
    
    void Start ( ) {
        for ( auto c : components ) 
        { c->Start(); } 
    }

    void Set_Active ( bool active ) { Active = active; }
    std::string Get_Name ( ) { return name; }

    void Set_Color ( glm::vec4 _color ) {
        color = _color;
    }

    std::string Get_text ( ) { return text; }
    void Add_text ( char _t ) { text+=_t; }
    void Add_text ( std::string _t ) { text+=_t; }
    void Set_text ( std::string _t ) { text=_t; }
};


#endif