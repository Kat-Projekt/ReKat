#ifndef COLLISION_H
#define COLLISION_H

#include "object.h"
#include "behaviour.h"
#include "manager.h"

#define CLAMP(n,a,b) ( ( n > b ) ? b : ( ( n < a ) ? a : n ) )
#define OUT(n,a,b) ( ( n > b ) ? true : ( ( n < a ) ? true : false ) )

class Collider : public Behaviour {
private:
	glm::vec2 pos;
    glm::vec2 size;
	bool is_trigger;
public:
    void Start ( );
	void Update ( ) { }
	void Set_size ( glm::vec2 _size ) { size = _size; }
	glm::vec2 Get_pos ( ) { return Manager::Object_Get(name)->Get_pos(); }
	glm::vec2 Get_size ( ) { return size; }
};

namespace Collision {
	static std::vector < Collider* > Colliders;
	// this map contains every collider grouped by position
	// /// 100 200 300 400
	// 100 co1 co2 /// co4
	// 200 /// co3 /// ///
	// 300 /// /// /// ///
	// 400 co5 co6 /// co7
	// when checking collisions only check the ones in neibouns cells;
	// if we nead to verify co1 collisons we only verifiy: co2 and co3
    static std::vector < std::vector < int > > Collision_Map;
	// Max number of colliders per Box if another collider tryes to enter it is verboten
	int Max_Colliders = 2;
	// Dimension of the collion cell
	static int Box_Size = 100;
	// verify if the collider moved the move it
	static void Update_Collision_Map ( ) { }
	inline static std::vector < int > Get_Colliders ( glm::vec2 pos ) { 
		// quantiza la posizione
		int X = pos.x / Box_Size;
		int Y = pos.y / Box_Size;
		// get all neibourn colliders
		std::vector < int > Colliders;
		// (x-1,y+1) (x+0,y+1) (x+1,y+1)
		// (x-1,y+0) (x+0,y+0) (x+1,y)
		// (x-1,y-1) (x+0,y-1) (x+1,y-1)
		for ( int Dx = -1; Dx <= 1; Dx++) {
			for (int Dy = -1; Dy <= 1; Dy++) 
			{ true; }
		}
		return Colliders;
	}
	static int Get_collider_Position ( ) { } 
    static void Add_Collider ( Collider* o ) {
		Colliders.push_back( o );
		int collider = Colliders.size()-1;
		o->Get_pos();
	}
	// checks if o is in collision with another collider if it is:
	// case is_trigger : send a collision trigger call to both colliders
	// case is_block : send a collison call to both colliders and moves the o collider to a non colliding position
	// the is_block collision call is called also on contact
	static void Check_collision ( Collider* o ) { }

    /*static void Check_collisons ( ) {
        // ciclo dal primo al penultimo
        for ( size_t primo = 0; primo < Objs.size() - 1; primo ++ ) {
            // ciclo per le coppie non viste e diverse dall'identità
            for ( size_t secondo = 1 + primo; secondo < Objs.size() - 1; secondo ++ ) {
                //std::cout << "check " << Objs[primo]->Get_Name() << " & " << Objs[secondo]->Get_Name() << '\n';
                // collision x-axis?
                bool collisionX = Objs[primo]->Get_pos().x + Objs[primo]->Get_size().x >= Objs[secondo]->Get_pos().x &&
                    Objs[secondo]->Get_pos().x + Objs[secondo]->Get_size().x >= Objs[primo]->Get_pos().x;
                // collision y-axis?
                bool collisionY = Objs[primo]->Get_pos().y + Objs[primo]->Get_size().y >= Objs[secondo]->Get_pos().y &&
                    Objs[secondo]->Get_pos().y + Objs[secondo]->Get_size().y >= Objs[primo]->Get_pos().y;
                
                // send collision call
                if ( collisionX && collisionY ) {
                    Objs[primo]->Andle_Collsions( Objs[secondo]->name, true );
                    Objs[secondo]->Andle_Collsions( Objs[primo]->name, true );
                }
            }
        }
        //std::cout <<'\n';        
    }
    // check for collisions in a direction return position
    static void Move ( Object *o, glm::vec2 _new_pos ) {
        // create a new_position that can't collide with the objs
        for ( auto _o : Objs ) {
            if ( o == _o ) { continue; }
            // collision x-axis?
            bool collisionX = o->Get_pos().x +  o->Get_size().x >= _o->Get_pos().x &&
                             _o->Get_pos().x + _o->Get_size().x >=  o->Get_pos().x;
            // collision y-axis?
            bool collisionY = o->Get_pos().y +  o->Get_size().y >= _o->Get_pos().y &&
                             _o->Get_pos().y + _o->Get_size().y >=  o->Get_pos().y;
                
                // send collision call
                if ( collisionX && collisionY ) {
                    o->Andle_Collsions( _o->name, true );
                    _o->Andle_Collsions( o->name, true );
                }
        }
        
    }*/
};

void Collider::Start ( ) { // add this collider to Collision manager
	Collision::Add_Collider ( this );
}

#endif