#version 330 core
in vec2 TexCoords;

uniform int P1;
uniform int P2;
uniform vec4 spriteColor;

out vec4 fragColor;

void main(void) {
	vec4 color = vec4(1) - spriteColor;
	if ( TexCoords.x > .5 ) { 
		if ( TexCoords.y < P1 * 0.1 ) { color = spriteColor; }
	} else {
		if ( TexCoords.y < P2 * 0.1 ) { color = spriteColor; }
	}
	fragColor = color;
}