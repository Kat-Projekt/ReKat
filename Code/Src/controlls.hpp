#include "connection.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

Camera cam ({0,0},{SCR_WIDTH, SCR_HEIGHT});
bool press = false;
double m_x,m_y;
int old_start_line = 0;
int start_line;

float angle ( glm::vec2 vector ) {
    // normalize
   // float magnetude = sqrt(vector.x*vector.x + vector.y*vector.y);
   // vector = { vector.x / magnetude , vector.y / magnetude };
    // find angle
    float angle = 0;
    if ( vector.x > 0 ) {
        if ( vector.y > 0) { angle = M_PI_4; }
        else { angle = - M_PI_4; }
    } else {
        if ( vector.y > 0) { angle = M_PI - M_PI_4; }
        else { angle = M_PI + M_PI_4; }
    }
    std::cout << "vector.x " << vector.x << " vector.y " << vector.y << '\n';
    return angle - M_PI_2;
}

static void ReKat::grapik::Input::Keyboard ( GLFWwindow* window, int key, int scancode, int action, int mode ) {
    // shift
    if ( key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT ) {
        if ( action == GLFW_PRESS ) { shift = true; } 
        else { shift = false; }
    }
    // ctrl
    if ( key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL ) {
        if ( action == GLFW_PRESS ) { ctrl = true; } 
        else { ctrl = false; }
    }

    if ( ( action == GLFW_PRESS || action == GLFW_REPEAT ) ) {
        // player movement
        if ( key == GLFW_KEY_A ) { Manager::Object_Get("Player")->DMove ( glm::vec2(-10,0) ); Send_positon (); }
        if ( key == GLFW_KEY_W ) { Manager::Object_Get("Player")->DMove ( glm::vec2(0,-10) ); Send_positon (); }
        if ( key == GLFW_KEY_D ) { Manager::Object_Get("Player")->DMove ( glm::vec2(10,0) );  Send_positon (); }
        if ( key == GLFW_KEY_S ) { Manager::Object_Get("Player")->DMove ( glm::vec2(0,10) );  Send_positon (); }


        if ( key == GLFW_KEY_ESCAPE ) { grapik::End(); }
        if ( key == GLFW_KEY_ENTER ) { execute_command (); return; }
        if ( key == GLFW_KEY_DELETE || key == GLFW_KEY_BACKSPACE ) 
        { if ( history[current_command].size() > 0 ) { history[current_command].pop_back (); } return; }
        if ( key == GLFW_KEY_DOWN ) 
        { if ( current_command < history.size( ) - 1 ) { current_command ++; } return; }
        if ( key == GLFW_KEY_UP ) 
        { if ( current_command > 0) { current_command --; }  return; }
        if ( key == GLFW_KEY_T && !text_mode ) { text_mode = true; std::cout << "enter text mode\n"; }
    }
}
static void ReKat::grapik::Input::Mouse_pos ( GLFWwindow* window, double xposIn, double yposIn ) { 
    if ( press && !text_mode ) {
        // cam.DMove ( {(m_x - xposIn) * cam.GetScale(),  (m_y - yposIn) * cam.GetScale()}  );
    }
    m_x = xposIn;
    m_y = yposIn;

    // update UI
    Manager::Update_Mouse_Position ( {xposIn, yposIn} );

    // spada rotation
    Manager::Object_Get("Player")->Get_Sub_Object("Spada")->Rotate( angle( { SCR_WIDTH/2- xposIn, SCR_HEIGHT/2 - yposIn } ) );
}
static void ReKat::grapik::Input::Mouse_key ( GLFWwindow *window, int button, int action, int mods ) {
    if ( button == GLFW_MOUSE_BUTTON_LEFT ) {
        if ( action == GLFW_PRESS )   { press = true;  Manager::Update_Mouse_Status(Manager::Pressed); }
        if ( action == GLFW_RELEASE ) { press = false; Manager::Update_Mouse_Status(Manager::Release); }
    }
}
static void ReKat::grapik::Input::ScrollWell ( GLFWwindow* window, double xoffset, double yoffset ) {
    if ( text_mode ) {
        if ( yoffset <= 0 ) { old_start_line = start_line; start_line ++; }
        if ( yoffset >= 0 ) { old_start_line = start_line; start_line --; }
    } else {
        cam.DScale(-yoffset*0.1f);
    }
    // if (start_line < 0 ) { start_line = 0; }
}
static void ReKat::grapik::Input::FreamBufferResize ( GLFWwindow* window, int width, int height ) {
    glViewport ( 0, 0, SCR_WIDTH = width, SCR_HEIGHT = height );
    cam.Resize({width,height});
}
static void ReKat::grapik::Input::Caracters ( GLFWwindow* window, unsigned int codepoint ) {
    if ( text_mode ) 
    { history[current_command] += (char)(codepoint); }
}