#define DIAGNOSTIC
#define EXTEND
#include <graphik/components.hpp>

int main ( ) {
    std::cout << "La librerie e' caricata correttamente\n";
    ReKat::grapik::Start ( "Render_Test", 600, 400 );

    Objekt main ("pippo",{0,0,0});

    Objekt frame ("framebuffer",{0,0,0},{600,400,10});

    Objekt fran ("franco",{0,0,0},{500,500,100});
    Objekt gior ("pier", {300,0,-0.1});

    main.Add_Child ( &gior );
    main.Add_Child ( &frame );

    fran.Add_Child ( &gior );
    
    Texture texture;
    texture.Make ( "sprite.png" );
    Shader shader;
    shader.Make ( "sprite.vs", "sprite.fs" );
    Shader frameshader;
    frameshader.Make ( "framebuffers.vs", "framebuffers.fs" );
    Camera camera;

    gior.Add_Component < Sprite > ( )->Set ( &texture, &shader, &camera, {2,1}, 1 );
    fran.Add_Component < Sprite > ( )->Set ( &texture, &shader, &camera, {2,1} );
    fran.Add_Component ( &camera ); 
    frame.Add_Component < Framebuffer > ( )->Set(600,400)->Set(&frameshader)->Set(&fran);
    
    Manager::Set_Active_Scene ( &main );
    color ( "Start Success\n", FOREGROUND_GREEN );

    while ( ReKat::grapik::IsEnd( ) ) {
		glClearColor(1.0, 1.0, 0.0, 1.0f);
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