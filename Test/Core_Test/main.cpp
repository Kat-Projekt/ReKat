#define DIAGNOSTIC
#include "objekt.hpp"

class Ciao : public Behaviour {
    void Start ( ) { DEBUG (3,"Start"); }
    void Update ( ) { DEBUG (4,"Update"); }
};

int main ( ) {
    Objekt pippo ( "franco" );
    pippo.Add_Component < Ciao > ();
    Manager::Set_Active_Scene ( &pippo );

    for ( int i = 0; i < 10; i++ ) {
        Manager::Update ( );
    }
}