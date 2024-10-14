#define DIAGNOSTIC
#include <graphik/components.hpp>

class Interupt : public Behaviour {
private:
    /* data */
public:
    void Terminate ( ) { throw; }
};

int main ( ) {
    ReKat::grapik::Start ( "Button_Test", 800, 600 );

    Objekt main ( "giov" );
    Objekt butt ( "pier" );
    main.Add_Child ( &butt );

    Interupt ii;
    butt.Add_Component < Button > ( )->Set ( &ii, &Interupt::Terminate );
    
    Manager::Set_Active_Scene ( &main );

    while ( ReKat::grapik::IsEnd ( ) ) {
        glClearColor(0.0, 1.0, 1.0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

        Manager::Update ( );
        ReKat::grapik::Update ( );
    }
    
    
    ReKat::grapik::Terminate ( );
}

