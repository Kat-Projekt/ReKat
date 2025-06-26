#define DIAGNOSTIC
#include "objekt.hpp"

class Ciao
: public Behaviour 
{
    void Start ( ) {
        for (size_t i = 0; i < 2; i++)
        {
            auto O = new Objekt ( "okoioni" );
            obj->Add_Child ( O );
        }
    }
    void Update ( ) { DEBUG (4,"Update"); }
};

class Inizialize
: public Behaviour 
{
    void Start ( ) {
        for (size_t i = 0; i < 2; i++)
        {
            auto O = new Objekt ( "oko" );
            O->Add_Component < Ciao > ( );
            obj->Add_Child ( O );
        }
    }
    void Update ( ) {
        for (size_t i = 0; i < 2; i++)
        {
            auto O = new Objekt ( "oko" );
            O->Add_Component < Ciao > ( );
            obj->Add_Child ( O );
        }
    }
};

int
main ( void )
{
    for 
    ( int i = 0; i < 4; i++ ) 
    {
        auto pippo = Manager::Objekt_Load ( "franco" );
        pippo->Add_Component < Inizialize > ();
        DEBUG (3, "-----------------------------------------------------");
        DEBUG (3, "-----------------------CONFIGURED--------------------");
        DEBUG (3, "-----------------------------------------------------");

        Manager::Set_Active_Scene ( pippo );

        DEBUG (3, "-----------------------------------------------------");
        DEBUG (3, "-----------------------STARTED-----------------------");
        DEBUG (3, "-----------------------------------------------------");

        Manager::Update ( );
        Manager::Update ( );

        DEBUG (3, "-----------------------------------------------------");
        DEBUG (3, "-----------------------UPDATED-----------------------");
        DEBUG (3, "-----------------------------------------------------");
       
        Manager::Get_Active_Scene ( )->Print_Tree ( );
        
        Manager::Free_Objekt ( "franco" );
        DEBUG ( 3, "Acrive Scene: ",
            Manager::Get_Active_Scene ( )
        );
        
        DEBUG (3, "-----------------------------------------------------");
        DEBUG (3, "-----------------------DELETED-----------------------");
        DEBUG (3, "-----------------------------------------------------");

    }
    return 0;
}

