#define DIAGNOSTIC
#include <engine.hpp>

int main ( ) {
    std::cout << "La librerie e' caricata correttamente\n";
    ReKat::grapik::Start ( "Render_Test_1", 600, 400 );
    ReKat::grapik::Start ( "Render_Test", 600, 400 );

    Objekt scene_1 ( "franco" );
    Objekt scene_2 ( "pippo" );
    
    Manager::Camera_Load  ( "cam1", &scene_1 );
    Manager::Camera_Load  ( "cam2", &scene_2 );

    scene_1.Add_Component < Sprite > ( )->Set ( "sprite", "sprite", "cam1", {2,1}, 0 );
    scene_2.Add_Component < Sprite > ( )->Set ( "sprite_1", "sprite_1", "cam2", {2,1}, 1 );

    ReKat::grapik::Bound_Window ( "Render_Test" );
    Manager::Texture_Load ( "sprite", "sprite.png" );
    Manager::Shader_Load  ( "sprite", "sprite.vs", "sprite.fs" );
    scene_1.Start ( );
    ReKat::grapik::Bound_Window ( "Render_Test_1" );
    Manager::Texture_Load ( "sprite_1", "sprite.png" );
    Manager::Shader_Load  ( "sprite_1", "sprite.vs", "sprite.fs" );
    scene_2.Start ( );

    color ( "Start Success\n", FOREGROUND_GREEN );

    while ( ReKat::grapik::IsEnd( ) ) {
        ReKat::grapik::Bound_Window ( "Render_Test" );
		glClearColor(0.0, 0.0, 0.0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
        scene_1.Update ( );

        ReKat::grapik::Bound_Window ( "Render_Test_1" );
		glClearColor(0.0, 0.0, 0.0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
        scene_2.Update ( );

        // pool
        ReKat::grapik::Update ( );
    }
    
    ReKat::grapik::Terminate ( );
    return 0;
}