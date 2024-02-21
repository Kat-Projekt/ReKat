#version 330 core
in vec2 TexCoords;

uniform sampler2D Image;
uniform vec4 spriteColor;

out vec4 fragColor;

void main(void) {
	vec4 color = spriteColor * texture (Image, TexCoords);
	if ( color.a < 0.5 )  { discard; }
    fragColor = color;
}