#ifndef COLLISION_H
#define COLLISION_H

#include "object.h"
#include "behaviour.h"
#include "object.h"
#include "map"

#define CLAMP(n,a,b) ( ( n > b ) ? b : ( ( n < a ) ? a : n ) )
#define OUT(n,a,b) ( ( n > b ) ? true : ( ( n < a ) ? true : false ) )

class Collider : public Behaviour {
private:
	Object *obj = nullptr;
    glm::vec2 size;
	bool is_trigger = false;
	bool is_movable = true;
public:
    void Start ( );
	void Update ( ) { }
	Collider *Set_size ( glm::vec2 _size ) { size = _size; return this; }
	glm::vec2 Get_pos ( ) { return obj->Get_pos( ); }
	glm::vec2 Get_size ( ) { return size; }
	bool Is_trigger ( ) { return is_trigger; }
	bool Is_movable ( ) { return is_movable; }
	Collider *Set_trigger ( bool _trigger ) { is_trigger = _trigger; return this; }
	Collider *Set_movable ( bool _movable ) { is_movable = _movable; return this; }
	Collider *Set_Obj ( Object * _obj ) { obj = _obj; return this; }
	Object* Get_Obj ( ) { return obj; }
};

namespace Collision {
	static std::vector < Collider* > Colliders;
	enum Status {
		COLLIDING,
		ENTER,
		EXIT,
		NONE
	};
	static std::vector < Status > collision_Status;
	// this map contains every collider grouped by position
	// /// 100 200 300 400
	// 100 co1 co2 /// co4
	// 200 /// co3 /// ///
	// 300 /// /// /// ///
	// 400 co5 co6 /// co7
	// when checking collisions only check the ones in neibouns cells;
	// if we nead to verify co1 collisons we only verifiy: co2 and co3
    static std::map < std::pair <int,int>, std::vector < int > > Collision_Map;
	// Max number of colliders per Box if another collider tryes to enter it is verboten
	int Max_Colliders = 10;
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
		// std::cout << "added collider: " << o->name << '\n';
	}
	// checks if o is in collision with another collider if it is:
	// case is_trigger : send a collision trigger call to both colliders
	// case is_block : send a collison call to both colliders and moves the o collider to a non colliding position
	// the is_block collision call is called also on contact
	
	static glm::vec2 Check_collision ( Collider* O1, Collider* O2 ) { 
		// check collision between obj1 e obj2 
		// collision x-axis?
		float Dx = abs(O1->Get_pos().x - O2->Get_pos().x) - ( O1->Get_size ().x + O2->Get_size().x ) / 2;
		float Dy = abs(O1->Get_pos().y - O2->Get_pos().y) - ( O1->Get_size ().y + O2->Get_size().y ) / 2;
		if ( Dy > 0 || Dx > 0 ) { return {0,0}; }
		
		glm::vec2 move_vector;
		if ( abs(Dx) < abs(Dy) ) {
			int Sing = ( O2->Get_pos().x - O1->Get_pos().x >= 0 ? 1 : -1 );
			move_vector = { Dx * Sing, 0 };
		} else {
			int Sing = ( O2->Get_pos().y - O1->Get_pos().y >= 0 ? 1 : -1 );
			move_vector = { 0, Dy * Sing };
		}

		if ( ! ( O1->Is_trigger( ) || O2->Is_trigger( ) ) ) {
			if ( O1->Is_movable( ) && O2->Is_movable( ) ) {
				O1->Get_Obj()->DMove( move_vector * 0.5f );
				O2->Get_Obj()->DMove( - move_vector * 0.5f );
				return move_vector;
			}
			if ( O1->name == "Player" || O2->name == "Player" ) { 
				if ( O1->Is_movable( ) ) { O1->Get_Obj()->DMove( move_vector );}
				if ( O2->Is_movable( ) ) { O2->Get_Obj()->DMove( move_vector );}
			} else {
				if ( O1->Is_movable( ) ) { O1->Get_Obj()->DMove( - move_vector );}
				if ( O2->Is_movable( ) ) { O2->Get_Obj()->DMove( - move_vector );}
			}
		}
		return move_vector;
	}

	static void Check_collisions ( ) {
		if ( Colliders.size () < 2 ) { return; }
		// ciclo dal primo al penultimo
        for ( size_t primo = 0; primo < Colliders.size() - 1; primo ++ ) {
			Collider * _cp = Colliders[primo];
			if ( ! _cp->Get_Obj()->Get_Active() ) { continue; }
            // ciclo per le coppie non viste e diverse dall'identità
            for ( size_t secondo = 1 + primo; secondo < Colliders.size(); secondo ++ ) {
				Collider * _cs = Colliders[secondo];
				if ( ! _cs->Get_Obj()->Get_Active() ) { continue; }
				if ( !_cp->Is_movable() && !_cs->Is_movable() ) { continue; };
				glm::vec2 C = Check_collision ( _cp,  _cs );
                // std::cout << "check " << _cp->name << " & " << _cs->name << " = " << ( C==glm::vec2{0,0} ? "si" : "no" ) << '\n';

				// inside collision
				if ( C != glm::vec2 (0,0) ) {
					if ( ! _cp->Is_trigger( ) && ! _cs->Is_trigger( ) ) {
						//std::cout << "both solid";
						// mantain collider out of other collider;
						_cp->Get_Obj()->Andle_Collsions( _cs->name, false );
						_cs->Get_Obj()->Andle_Collsions( _cp->name, false );
					} else {
						//std::cout << "both trigger";
						_cp->Get_Obj()->Andle_Collsions( _cs->name, true );
						_cs->Get_Obj()->Andle_Collsions( _cp->name, true );
					}
				}
            }
        }
        //std::cout <<'\n'; 
	}

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