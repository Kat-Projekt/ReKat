const char* framebuffer_shader_vs = "#version 330 core\n\
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

const char* framebuffer_shader_fs = "#version 330 core\n\
out vec4 FragColor;\n\
in vec2 TexCoords;\n\
\n\
uniform sampler2D screenTexture;\n\
\n\
void main() {\n\
    FragColor = texture ( screenTexture, TexCoords );\n\
}";