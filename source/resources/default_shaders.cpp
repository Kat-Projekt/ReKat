#include "extensions/graphik/resources/default_shaders.h"

const char* ReKat::Graphik::text_shader_vs = "#version 330 core\n\
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>\n\
layout (location = 1) in vec4 instace; // <index, width, x, y>\n\
\n\
out vec2 TexCoords;\n\
out float frame;\n\
\n\
uniform mat4 model;\n\
uniform mat4 projection;\n\
\n\
void main()\n\
{\n\
    TexCoords = vertex.zw;\n\
	frame = instace.x;\n\
    vec4 tempo = projection * model * vec4(vertex.x + instace.z/100, vertex.y + instace.w/100, 0.01, 1.0);\n\
    gl_Position = vec4(tempo.x, tempo.y, tempo.z, tempo.w);\n\
}";

const char* ReKat::Graphik::text_shader_fs = "#version 330 core\n\
in vec2 TexCoords;\n\
in float frame;\n\
\n\
uniform sampler2D Image;\n\
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
    float glyph_alpha = texture(Image, vec2((TexCoords.x / SPRITE_COLUMNS) + pos.x * (1.0 / SPRITE_COLUMNS),\n\
        (TexCoords.y / SPRITE_ROWS) + pos.y * (1.0 / SPRITE_ROWS))).r;\n\
\n\
	if( glyph_alpha < 0.1)\n\
        discard;\n\
    fragColor = vec4 (spriteColor.rgb, spriteColor.a * glyph_alpha);\n\
}";

const char* ReKat::Graphik::sprite_shader_fs = "#version 330 core\n\
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

const char* ReKat::Graphik::sprite_shader_vs = "#version 330 core\n\
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

const char* ReKat::Graphik::framebuffer_shader_vs = "#version 330 core\n\
layout (location = 0) in vec2 aPos;\n\
layout (location = 1) in vec2 aTexCoords;\n\
\n\
out vec2 TexCoords;\n\
\n\
uniform mat4 model;\n\
uniform mat4 projection;\n\
\n\
void main() {\n\
    TexCoords = vec2 ( aTexCoords.x, -aTexCoords.y );\n\
    vec4 tempo = projection * model * vec4(aPos.xy, 0.0, 1.0);\n\
    gl_Position = vec4(tempo.x, tempo.y, 0, 1);\n\
}";

const char* ReKat::Graphik::framebuffer_shader_fs = "#version 330 core\n\
out vec4 FragColor;\n\
in vec2 TexCoords;\n\
\n\
uniform sampler2D screenTexture;\n\
\n\
void main() {\n\
    FragColor = texture ( screenTexture, TexCoords );\n\
}";
