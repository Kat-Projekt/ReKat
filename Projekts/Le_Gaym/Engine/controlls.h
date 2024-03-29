#include "../global.h"
#define _USE_MATH_DEFINES
#include <math.h>

static void __Keyboard ( GLFWwindow* window, int key, int scancode, int action, int mode ) {
    if ( ( action == GLFW_PRESS || action == GLFW_REPEAT ) ) {
        if ( key == GLFW_KEY_ESCAPE ) { ReKat::grapik::End(); }
    }
}
static void __Mouse_pos ( GLFWwindow* window, double xposIn, double yposIn ) {
    // update UI
    Manager::Update_Mouse_Position ( );

    }
static void __Mouse_key ( GLFWwindow *window, int button, int action, int mods ) {
    if ( button == GLFW_MOUSE_BUTTON_LEFT ) {
        if ( action == GLFW_PRESS )   { Manager::Update_Mouse_Status(Scene::Pressed); }
        if ( action == GLFW_RELEASE ) { Manager::Update_Mouse_Status(Scene::Release); }
    }
}
static void __ScrollWell ( GLFWwindow* window, double xoffset, double yoffset ) {
    Manager::Active_Scene->cam.DScale(-yoffset*0.1f);
    // if (start_line < 0 ) { start_line = 0; }
}
static void __FreamBufferResize ( GLFWwindow* window, int width, int height ) {
    glViewport ( 0, 0, ReKat::grapik::Internal::SCR_WIDTH = width, ReKat::grapik::Internal::SCR_HEIGTH = height );
}
static void __Caracters ( GLFWwindow* window, unsigned int codepoint ) {
    Manager::UI_Object_Get("input")->Add_text((char)codepoint);
}
