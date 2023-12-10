#ifndef OBJECT_H
#define OBJECT_H

#include "sprite.h"
#include "unordered_map"
#include "behaviour.h"

class Object {
protected:
    bool Active = true;
    std::string name;

    // transform
    // the position is the center of the sprite
    glm::vec2 pos;
    glm::vec2 size;
    Sprite* sprite = nullptr;
    std::unordered_map < std::string, Object* > Sub_Objects;
    int frame;
    glm::vec2 parent_pos = {0,0};
    float rot = 0;
    glm::vec2 pivot = {0.5,0.5};

    std::vector < std::shared_ptr<Behaviour> > components;

public:
    Object ( ) { }
    Object ( std::string _name, Sprite *sprite, glm::vec2 _pos, glm::vec2 _size ) : name(_name), sprite(sprite), pos(_pos), size(_size) { }

    void Set_sprite ( Sprite * _sprite ) 
    { sprite = _sprite; }

    void Add_Sub_Object ( std::string name, Object * _sub_obj ) 
    { Sub_Objects.insert ( {name, _sub_obj} ); }
    Object* Get_Sub_Object ( std::string name ) 
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
    char* packpos ( ) 
    { return (char*) & pos; }

    glm::vec2 Get_pos ( ) { return pos - glm::vec2{-size.x*0.5,size.y*0.5}; }
    glm::vec2 Get_size ( ) { return size; }

    void Rotate ( float rotation ) { rot = rotation; }
    void Set_Rotation_Pivot ( glm::vec2 pivot ) { this->pivot = pivot; }
    
    void Change_frame ( int _frame ) { frame = _frame; }
    void DChange_frame ( int _frame ) { frame += _frame; }
    void Draw  ( ) { 
        if ( !Active ) { return; }
        if ( sprite != nullptr ) { sprite->Draw_frame ( frame, pos+parent_pos - glm::vec2(size.x/2, size.y/2), size, rot, {1,1,1,1}, pivot ); }
        for ( auto o : Sub_Objects ) 
        { o.second->Draw( ); }
    }
    
    template < class C > 
    C* Add_component ( ) {
        C* c = new C ( );
        if ( std::is_base_of<Behaviour, C>::value ) 
        { components.push_back( std::shared_ptr < Behaviour > ( c ) ); }
        return c;
    }

    template < class C > 
    C* Add_component ( C* c ) {
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
};



#endif