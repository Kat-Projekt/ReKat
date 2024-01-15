#ifndef OBJECT_H
#define OBJECT_H

#include "sprite.h"
#include "unordered_map"
#include "behaviour.h"
#include <memory>
#include <typeinfo>

class Object {
protected:
    bool Active = true;
    bool Is_Solid = true;
    std::string name;

    // transform
    // the position is the center of the sprite
    glm::vec2 parent_pos = {0,0};
	glm::vec2 old_p_pos;
    glm::vec2 pivot = {0.5,0.5};
    glm::vec2 pos = {0,0};
    glm::vec2 _pos = {0,0};
	glm::vec2 old_pos;
    float altitude = 0;
    glm::vec2 size;
	float scale = 1;
    float rot = 0;
	float old_rot = 0;
	glm::vec2 rot_vector = {0,0};

    Sprite* sprite = nullptr;
    glm::vec4 color = {1,1,1,1};
    int frame;

    std::unordered_map < std::string, Object* > Sub_Objects;
    std::vector < Behaviour* > components;

public:
    Object ( ) { }
    Object ( std::string _name, Sprite *sprite, glm::vec2 _pos, glm::vec2 _size ) : name(_name), sprite(sprite), size(_size) {
		Move (_pos);
		old_pos = _pos;
		PMove ({0,0});
	}

    void Set_sprite ( Sprite * _sprite ) 
    { sprite = _sprite; }

    void Add_Sub_Object ( std::string name, Object * _sub_obj ) { 
		Sub_Objects.insert ( {name, _sub_obj} ); _sub_obj->PMove(pos); 
		Sub_Objects[name]->Set_Altitude(altitude*1000.0f);
	}
    Object* Get_Sub_Object ( std::string name ) 
    { return Sub_Objects[name]; }

    void PMove ( glm::vec2 move ) { old_p_pos = parent_pos; parent_pos = move; }
    void PDMove ( glm::vec2 move ) { old_p_pos = parent_pos; parent_pos += move; }
    void DMove ( glm::vec2 move ) {
		old_pos = pos;
        pos += move;
        for ( auto o : Sub_Objects ) 
        { o.second->PDMove(move); }
    }
    void Move  ( glm::vec2 move ) {
		old_pos = pos;
        pos = move;
        for ( auto o : Sub_Objects ) 
        { o.second->PMove(move); }
    }
	Object * Revent_Move ( ) { 
		pos = old_pos; 
		parent_pos = old_p_pos;
        for ( auto o : Sub_Objects ) 
        { o.second->Revent_Move(); }
		return this;
	}
    char* packpos ( ) 
    { return (char*) & pos; }

	void Update_pos ( ) { 
		_pos = pos; 
		// add parent
		_pos += parent_pos;
		// center to sprite
		glm::vec2 p = pivot - glm::vec2{0.5,0.5};
		if ( old_rot != rot ) 
		{ rot_vector = glm::vec2{ size.x * ( p.x*cos(rot) - p.y*sin(rot) ), size.y * ( p.x*sin(rot) + p.y * cos(rot) ) }; }
		old_rot = rot;
		_pos -= rot_vector;
		// std::cout << name << " : " << _pos.x << " x " << _pos.y << '\n';
	}
    glm::vec2 Get_pos ( ) {
		Update_pos();
		return _pos; 
	}
    glm::vec2 Get_size ( ) { return size; }

    void Rotate ( float rotation ) { rot = rotation; }
    void Set_Rotation_Pivot ( glm::vec2 pivot ) { this->pivot = pivot; }
    
    void Change_frame ( int _frame ) { frame = _frame; }
    void DChange_frame ( int _frame ) { frame += _frame; }
    virtual void Draw  ( ) { 
        if ( !Active ) { return; }
        for ( auto o : Sub_Objects ) 
        { o.second->Draw( ); }

        if ( sprite != nullptr ) { sprite->Draw_frame ( frame, glm::vec3{ pos+parent_pos - glm::vec2{ size.x, 0}, altitude }, size * scale, rot, color, pivot ); }
    }
    
    template < class C > 
    C* Add_component ( ) {
        C* c = new C ( );
        c->name = name;
        if ( std::is_base_of<Behaviour, C>::value ) 
        { components.push_back( ( Behaviour * ) ( c ) ); }
        return c;
    }
    template < class C > 
    C* Add_component ( C* c ) {
        c->name = name;
        if ( std::is_base_of<Behaviour, C>::value ) 
        { components.push_back( ( Behaviour * ) ( c ) ); }
        return c;
    }
	
    template < class C > 
	C* Get_component ( ) {
		for ( auto c : components ) 
		{ if ( typeid(*c) == typeid(C)) { return (C*)c; } }
		return nullptr;
	}
	template < class C > 
	std::vector  < C* > Get_components ( ) {
		std::vector < C* > cc;
		for ( auto c : components ) 
		{ if ( typeid(*c) == typeid(C)) { cc.push_back((C*)c); } }
		return cc;
	}

    virtual void Update ( ) {
        for ( auto o : Sub_Objects ) 
        { o.second->Update ( ); }
        for ( auto c : components ) 
        { c->Update(); } 
    }
    
    virtual void Start ( ) {
        for ( auto o : Sub_Objects ) 
        { o.second->Start ( ); }
        for ( auto c : components ) 
        { c->Start(); } 
    }

    Object * Set_Active ( bool active ) { Active = active; return this; }
    bool Get_Active ( ) { return Active; }
    std::string Get_Name ( ) { return name; }

    void Andle_Collsions ( std::string obj, bool is_trigger ) {
		if ( is_trigger ) {
			for ( auto c : components ) 
			{ c->CollisionTrigger( obj ); } 
			return;
		}
        for ( auto c : components ) 
        { c->Collision( obj ); } 
    };

    Object * Set_Color ( glm::vec4 _color ) {
        color = _color;
		return this;
	}

    Object * Set_Altitude ( float a ) {
        altitude = a / 1000.0f;
        for ( auto o : Sub_Objects ) 
        { o.second->Set_Altitude(a); }
		return this;
    }

	Object * Set_Scale ( float s ) 
	{ scale = ( s >= 0 ? s : 0 ); return this; }
};



#endif