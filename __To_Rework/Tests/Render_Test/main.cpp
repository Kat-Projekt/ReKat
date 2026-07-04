#define DIAGNOSTIC
#include <graphik/components.hpp>

int main ( ) {
    std::cout << "La librerie e' caricata correttamente\n";
    ReKat::grapik::Start ( "Render_Test", 600, 400 );

    Objekt main ("franco");
    
    Texture texture;
    texture.Make ( "sprite.png" );
    Shader shader;
    shader.Make ( "sprite.vs", "sprite.fs" );
    Camera camera;

    main.Add_Component < Sprite > ( )->Set ( &texture, &shader, &camera, {2,1} );
    main.Add_Component ( &camera );
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
    main.Free ( );
    texture.End ( );
    shader.End ( );
    return 0;
}