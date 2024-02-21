#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>
layout (location = 1) in vec3 instace; // <vec2 pos, float frame>

out vec2 TexCoords;
out float frame; 

uniform mat4 projection;
uniform mat4 model;

void main() {
    TexCoords = vertex.zw;
	frame = instace.z;
    vec4 pos = projection * model * vec4(vertex.x+instace.x, vertex.y-instace.y, 0.0, 1.0);
    gl_Position = pos;
}