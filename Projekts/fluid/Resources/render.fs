#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform float particles[100];

// texture sampler

void main() {
	int x = int(TexCoord.x*10);
	int y = int((TexCoord.y+TexCoord.x)*10);

	FragColor = vec4 (particles[x*10+y]);
}