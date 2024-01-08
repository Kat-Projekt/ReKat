#define OPENGL
#include <ReKat.hpp>
#include "Graphik/collision.h"

class Test : public Behaviour {
    void Collision ( std::string obj ) {
        std::cout << "\t Obj: " << name << " collided whit " << obj << '\n';
    }
};

int main ( ) {
    // creating objs
    Object a ( "A", nullptr,  {0,0}, {10,10} );
    Object b ( "B", nullptr,  {0,0}, {10,10} );
    Object c ( "C", nullptr,  {0,11}, {10,10} );
    Object d ( "D", nullptr,  {0,0}, {10,10} );

    a.Add_component<Test>();

    Collision::Add_Object(&a);
    Collision::Add_Object(&b);
    Collision::Add_Object(&c);
    Collision::Add_Object(&d);
    
    Collision::Check_collisons( );
} 