#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>
layout (location = 1) in vec4 instace; // <index, width, x, y>

out vec2 TexCoords;
out float frame; 

uniform mat4 model;
uniform mat4 projection;

void main()
{
    TexCoords = vertex.zw;
	frame = instace.x;
    vec4 tempo = projection * model * vec4(vertex.x + instace.z/100, vertex.y, gl_InstanceID*0.01, 1.0);
    gl_Position = vec4(tempo.x, tempo.y, tempo.z, tempo.w);
}