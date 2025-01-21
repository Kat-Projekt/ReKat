#version 330 core
in vec2 TexCoords;
in float frame;

uniform sampler2D Image;
uniform vec4 spriteColor;

out vec4 fragColor;

uniform float SPRITE_COLUMNS;
uniform float SPRITE_ROWS;
uniform float NUM_OF_SPRITES;

void main(void) {
    uint sprite_idx = uint(frame) % uint(NUM_OF_SPRITES);
    vec2 pos = vec2(int(sprite_idx) % int(SPRITE_COLUMNS), int(sprite_idx / SPRITE_COLUMNS));
    float glyph_alpha = texture(Image, vec2((TexCoords.x / SPRITE_COLUMNS) + pos.x * (1.0 / SPRITE_COLUMNS),
        (TexCoords.y / SPRITE_ROWS) + pos.y * (1.0 / SPRITE_ROWS))).r;
	
	if( glyph_alpha < 0.1)
        discard;
    fragColor = vec4 (spriteColor.rgb, spriteColor.a * glyph_alpha);
}