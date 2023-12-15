#include "connection.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

Camera cam ({0,0},{SCR_WIDTH, SCR_HEIGHT});
bool press = false;
double m_x,m_y;
int old_start_line = 0;
int start_line;

float angle ( glm::vec2 v ) {
    float angle = 0;
    float X = v.x / sqrt( v.x * v.x + v.y *v.y );
    if ( v.y > 0 ) { angle = M_PI-std::asin ( X ); } 
    else { angle = std::asin ( X ); }
    return angle;
}

static void __Keyboard ( GLFWwindow* window, int key, int scancode, int action, int mode ) {
    if ( ( action == GLFW_PRESS || action == GLFW_REPEAT ) ) {
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
static void __Mouse_pos ( GLFWwindow* window, double xposIn, double yposIn ) { 
    if ( press && !text_mode ) {
        // cam.DMove ( {(m_x - xposIn) * cam.GetScale(),  (m_y - yposIn) * cam.GetScale()}  );
    }
    m_x = xposIn;
    m_y = yposIn;

    // update UI
    Manager::Update_Mouse_Position ( {xposIn, yposIn} );

    // spada rotation
    Manager::Object_Get("Player")->Get_Sub_Object("Spada")->Rotate( angle( { xposIn - SCR_WIDTH/2, yposIn - SCR_HEIGHT/2 } ) );
}
static void __Mouse_key ( GLFWwindow *window, int button, int action, int mods ) {
    if ( button == GLFW_MOUSE_BUTTON_LEFT ) {
        if ( action == GLFW_PRESS )   { press = true;  Manager::Update_Mouse_Status(Manager::Pressed); }
        if ( action == GLFW_RELEASE ) { press = false; Manager::Update_Mouse_Status(Manager::Release); }
    }
}
static void __ScrollWell ( GLFWwindow* window, double xoffset, double yoffset ) {
    if ( text_mode ) {
        if ( yoffset <= 0 ) { old_start_line = start_line; start_line ++; }
        if ( yoffset >= 0 ) { old_start_line = start_line; start_line --; }
    } else {
        cam.DScale(-yoffset*0.1f);
    }
    // if (start_line < 0 ) { start_line = 0; }
}
static void __FreamBufferResize ( GLFWwindow* window, int width, int height ) {
    glViewport ( 0, 0, SCR_WIDTH = width, SCR_HEIGHT = height );
    cam.Resize({width,height});
}
static void __Caracters ( GLFWwindow* window, unsigned int codepoint ) {
    if ( text_mode ) 
    { history[current_command] += (char)(codepoint); }
}

void Setup_Input () {
    ReKat::grapik::Input::_Keyboard = &__Keyboard;
    ReKat::grapik::Input::_Mouse_pos = &__Mouse_pos;
    ReKat::grapik::Input::_Mouse_key = &__Mouse_key;
    ReKat::grapik::Input::_ScrollWell = &__ScrollWell;
    ReKat::grapik::Input::_FreamBufferResize = &__FreamBufferResize;
    ReKat::grapik::Input::_Caracters = &__Caracters;
}