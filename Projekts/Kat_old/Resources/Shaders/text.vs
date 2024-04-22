#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    vec4 tempo = projection * vec4(vertex.xy, 1.0, 1.0);
    gl_Position = vec4(tempo.x, tempo.y, tempo.z, tempo.w);
    TexCoords = vertex.zw;
}  