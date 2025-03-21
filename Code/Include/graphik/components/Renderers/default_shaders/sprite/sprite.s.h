const char* sprite_shader_fs = "#version 330 core\n\
in vec2 TexCoords;\n\
\n\
uniform sampler2D Image;\n\
uniform int frame;\n\
uniform vec4 spriteColor;\n\
\n\
out vec4 fragColor;\n\
\n\
uniform float SPRITE_COLUMNS;\n\
uniform float SPRITE_ROWS;\n\
uniform float NUM_OF_SPRITES;\n\
\n\
void main(void) {\n\
    uint sprite_idx = uint(frame) % uint(NUM_OF_SPRITES);\n\
    vec2 pos = vec2(int(sprite_idx) % int(SPRITE_COLUMNS), int(sprite_idx / SPRITE_COLUMNS));\n\
\n\
    vec4 fragColor1 = spriteColor * texture(Image, vec2((TexCoords.x / SPRITE_COLUMNS) + pos.x * (1.0 / SPRITE_COLUMNS),\n\
        (TexCoords.y / SPRITE_ROWS) + pos.y * (1.0 / SPRITE_ROWS)));\n\
    if(fragColor1.a < 0.1)\n\
        discard;\n\
    fragColor = fragColor1;\n\
}";

const char* sprite_shader_vs = "#version 330 core\n\
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>\n\
\n\
out vec2 TexCoords;\n\
\n\
uniform mat4 model;\n\
uniform mat4 projection;\n\
\n\
void main()\n\
{\n\
    TexCoords = vertex.zw;\n\
    vec4 tempo = projection * model * vec4(vertex.xy, 0.0, 1.0);\n\
    gl_Position = vec4(tempo.x, tempo.y, 0, 1);\n\
}";