#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;

void main() {
    TexCoords = aTexCoords;
    vec4 tempo = projection * model * vec4(aPos.xy, 0.0, 1.0);
    gl_Position = vec4(tempo.x, tempo.y, 0, 1);
}  
