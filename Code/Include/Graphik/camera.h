#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  2.0f;
const float ZOOM        =  45.0f;

class Camera {
private:
    glm::vec2 pos;
    glm::vec2 size;
    float scale = 1.0f;

public:
    Camera ( glm::vec2 new_pos, glm::vec2 size ) : pos(new_pos), size(size) { };
    ~Camera ( ) { };

    void Move  ( glm::vec2 new_pos ) { pos = new_pos; }
    void DMove ( glm::vec2 del_pos ) { pos += del_pos; }

    void Resize ( glm::vec2 new_size ) { size = new_size; }

    void Scale  ( float new_scale ) { new_scale > 0 ? scale = new_scale : 0; }
    void DScale ( float del_scale ) { scale+del_scale > 0 ? scale += del_scale : 0; }

    float GetScale ( ) { return scale; }

    glm::mat4 GetWiew ( ) {
        return glm::ortho ( pos.x + (size.x - size.x * scale) * 0.5f, pos.x + (size.x + size.x * scale) * 0.5f, 
                            pos.y + (size.y + size.y * scale) * 0.5f, pos.y + (size.y - size.y * scale) * 0.5f,
                            -1.0f, 1.0f );
    }
};

#endif
