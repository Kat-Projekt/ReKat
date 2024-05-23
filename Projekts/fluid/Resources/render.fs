#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform float particles[1024];

// texture sampler

void main() {
	int x = int((TexCoord.x)*32) % 32;
	int y = int((TexCoord.y)*32) % 32;

	FragColor = vec4 (particles[x*32+y]);
}