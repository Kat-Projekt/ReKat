#define DIAGNOSTIC
#define DEBUG_LEVEL 5
#include <graphik/components.hpp>

class Interupt : public Behaviour {
private:
    /* data */
public:
    void Terminate ( ) { std::cout <<  3, "hello maremma hane"	; }
};

int main ( ) {
    ReKat::grapik::Start ( "Button_Test", 800, 600 );

    Manager::Texture_Load ( "logo", "Logo.png" );
    Manager::Shader_Load ( "sprite", "sprite.vs", "sprite.fs" );
    Camera camera;

    Objekt main ( "giov" );
    Objekt butt ( "pier", {0,0,0},{500,500,500} );
    main.Add_Child ( &butt );

    Interupt ii;
    main.Add_Component ( &camera );
    butt.Add_Component < Button > ( )->OnHover ( &ii, &Interupt::Terminate );
	butt.Add_Component < Sprite > ( )->Set ( "logo", "sprite", &camera );
    
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

/*
#define DIAGNOSTIC
#include <graphik/components.hpp>

void pippo ( ) {
	DEBUG ( 1, "fine" );
}

int main ( ) {
    std::cout << "La librerie e' caricata correttamente\n";
    ReKat::grapik::Start ( "Render_Test", 600, 400 );

    Objekt main ("pier");
    
    Manager::Texture_Load ( "logo", "Logo.png" );
    Manager::Font_Load ( "font", "Font.ttf",80 );
    Manager::Shader_Load ( "sprite", "sprite.vs", "sprite.fs" );
    Manager::Shader_Load ( "text", "text.vs", "text.fs" );
    Camera camera;

    main.Add_Component < Sprite > ( )->Set ( "logo", "sprite", &camera );
    main.Add_Component < Text > ( )->Set ( "font", "text", &camera, {1,0,1,1} );
    main.Add_Component ( &camera );
	main.Add_Component < Button > ( )->OnClick ( pippo );
    main.Set_Size ( {300,300,100} );
    main.Set_Pos ( {0,300,0} );
    Manager::Set_Active_Scene ( &main );
    main.template Get_Component < Text >()->Set ( "l'unico e vero" );
   
    color ( "Start Success\n", FOREGROUND_GREEN );

    for ( int i = 0; i < 10; i-- ) {
		glClearColor(0.0, 1.0, 1.0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
        Manager::Update ( );
        color ( "Update Success\n", FOREGROUND_BLUE | FOREGROUND_INTENSITY );
        ReKat::grapik::Update ( );
    }
    
    ReKat::grapik::Terminate ( );
    Manager::Free_Video ( );
    main.Free ( );
    return 0;
}*/