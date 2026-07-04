#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>
layout (location = 1) in vec3 instance;

out vec2 TexCoords;
out float frame;

uniform mat4 model;
uniform mat4 projection;

void main ( ) {
    TexCoords = vertex.zw;
    vec4 tempo = projection * model * vec4(vertex.xy + instance.xy, 0.0, 1.0);
    gl_Position = vec4(tempo.x, tempo.y, 0, 1);
    frame = instance.z;
}