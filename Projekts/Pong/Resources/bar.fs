#version 330 core
in vec2 TexCoords;

uniform vec4 spriteColor;
out vec4 fragColor;

void main(void) {
    fragColor = spriteColor;
}