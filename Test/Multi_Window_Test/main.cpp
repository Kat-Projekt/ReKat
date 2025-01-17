#define DIAGNOSTIC
#include <engine.hpp>

int main ( ) {
    std::cout << "La librerie e' caricata correttamente\n";
    ReKat::grapik::Start ( "Render_Test", 600, 400 );
    ReKat::grapik::Start ( "Render_Test_1", 600, 400 );

    Objekt main ("franco");
    
    Manager::Texture_Load ( "sprite", "sprite.png" );
    Manager::Shader_Load  ( "sprite", "sprite.vs", "sprite.fs" );
    Manager::Camera_Load  ( "cam", &main );

    main.Add_Component < Sprite > ( )->Set ( "sprite", "sprite", "cam", {2,1} );
    Manager::Set_Active_Scene ( &main );
    color ( "Start Success\n", FOREGROUND_GREEN );

    while ( ReKat::grapik::IsEnd( ) ) {
		glClearColor(0.0, 0.0, 0.0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
        Manager::Update ( );
        color ( "Update Success\n", FOREGROUND_BLUE | FOREGROUND_INTENSITY );
        ReKat::grapik::Update ( );
    }
    
    ReKat::grapik::Terminate ( );
    return 0;
}