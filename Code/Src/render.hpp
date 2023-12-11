#include "load.hpp"

// Initilalize renderer
void Grapik_Init ( ) {
    // start window
    ReKat::grapik::Start ( "Kat Kave", SCR_WIDTH, SCR_HEIGHT, false, false, true );
    std::cout << "icon set: " << ReKat::grapik::SetIcon ( "favicon.png" ) << '\n';

    Load_resources ( );
    
    Create_Objs ( );
}

uint64_t spash_start = Get_Time();
float duration = 5000.0f;
int Render_splash ( ) {
    float alpha = ( Get_Time() - spash_start ) / duration;
    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    Manager::Shader_Get("text")->setMat4("projection", cam.Get_UI_Wiew());                              
    Manager::Draw_Text ( "death_record", "made with", {0.0f, 100.0f },
                         {SCR_WIDTH-50.0f,200.0f}, 1.0f, glm::vec4(1.0, 1.0f, 1.0f,alpha));
    Manager::Draw_Text ( "death_record", "4 Real Engine 5", {-95.0f, -5.0f },
                                                            {500.0f,500.0f}, 2.0f, glm::vec4(green_fl,alpha/2));
    Manager::Draw_Text ( "death_record", "4 Real Engine 5", {-100.0f, 0.0f },
                                                            {500.0f,500.0f}, 2.0f, glm::vec4(violet_fl,alpha));
    if ( alpha > 1 ) { return 1; } 
    else { return 0;}
}

float duration_2 = 500.0f;
int Render_splash_fade ( ) {
    float alpha = ( duration_2 - ( Get_Time() - spash_start ) ) / duration_2;
    Manager::Shader_Get("text")->setMat4("projection", cam.Get_UI_Wiew());

    Manager::Sprite_Get ( "empty_sprite" )->Draw_frame( 0, {0, 0}, {SCR_WIDTH,SCR_HEIGHT}, 0, glm::vec4{0.0f,0.0f,0.0f,alpha} );

    Manager::Draw_Text ( "death_record", "made with", {0.0f, 100.0f },
                         {SCR_WIDTH-50.0f,200.0f}, 1.0f, glm::vec4(1.0, 1.0f, 1.0f,alpha));
    Manager::Draw_Text ( "death_record", "4 Real Engine 5", {-95.0f, -5.0f },
                                                            {500.0f,500.0f}, 2.0f, glm::vec4(green_fl,alpha/2));
    Manager::Draw_Text ( "death_record", "4 Real Engine 5", {-100.0f, 0.0f },
                                                            {500.0f,500.0f}, 2.0f, glm::vec4(violet_fl,alpha));
    if ( alpha < 0 ) { return 1; }
    else { return 0;} 
}

// Draws a new frame
int frame = 0;
int fps = 0;
int _time = time(0);
float scale = 1.0f;
bool dis = true;
int scrool = 0;
int old_out = 0;
float text_alpha = 0.5f;
void Render_text ( ) {
    // command out; 
    if ( text_mode ) { text_alpha = 1.0f; } else { text_alpha = 0.5; }
    int out = Manager::Draw_Text ( "death_record", output.str(), {-500.0f,500.0f}, { 1000.0f, 1000.0f}, 1.0f, 
                                  glm::vec4(1-BGG, 1-BGR, 1-BGB,text_alpha), scrool );
    int max_rows = Manager::Text_Get ( "death_record" )->Get_Max_Rows ( 1000.0f );

    scrool = ( out >= max_rows ? out - max_rows : 0 );

    if ( old_out == out ) { scrool += start_line;
    } else { start_line = 0; }
    if ( scrool < 0 ) // lower limit
    { scrool = 0; start_line = old_start_line; }
    if ( scrool > out ) // upper limit
    { scrool = out; start_line = out;}
    old_out = out;

    if ( text_mode ) {
        // command box
        Manager::Text_Get ( "death_record" )->RenderText ( "$ " + history[current_command] + ( dis ? '/' : '\\' ), {25.0f,100.0f},
                                                            {SCR_WIDTH-50.0f,200.0f}, 0.75f, glm::vec4(0.0, 0.0f, 0.0f,text_alpha));
    }
}

void Render ( ) {
    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    if ( !text_mode ) { Render_text(); }

    fps++;
    //if ( _time < time (0) ) { _time = time (0); std::cout << "\nfps: " << fps << " time: " << _time; fps = 0; dis = !dis; }
    if ( fps%20 == 0 ) { frame ++; }

    // updating camera projections
    cam.Move( Manager::Object_Get("Player")->Get_pos() );

    Manager::Shader_Get( "sprite_1x1"  )->setMat4("projection", cam.Get_Wiew());
    Manager::Shader_Get( "sprite_2x2"  )->setMat4("projection", cam.Get_Wiew());
    Manager::Shader_Get( "tilemap" )->setMat4("projection", cam.Get_Wiew());
    Manager::Shader_Get( "text" )->setMat4("projection", cam.Get_UI_Wiew());
    Manager::Shader_Get( "UI" )->setMat4("projection", cam.Get_UI_Wiew());
    
    
    // Manager::Sprite_Get( "sprite" )->Draw_frame( frame, {300,100},{100,100},0 );
    // Manager::Text_Get ( "death_record" )->RenderText ( "ciao", {25.0f,SCR_HEIGHT-75.0f}, { SCR_WIDTH-50.0f, 200.0f}, 1.0f, glm::vec3(0.21875f, 0, 0.21875f));
    // Manager::Tilemap_Get ( "tilemap" )->Draw({100,300},{50,50});

    /// ------------------------------------------------------------------------- ///
    // -------------------------------------- UI --------------------------------- //
    /// ------------------------------------------------------------------------- ///
    
    if ( text_mode ) { Render_text(); }

    Manager::Update ( );
    Manager::Draw ( );
}
