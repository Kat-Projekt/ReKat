#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

const float offset = 1.0 / 200.0;

vec2 offsets[9] = vec2[](
    vec2(-offset,  offset), // top-left
    vec2( 0.0f,    offset), // top-center
    vec2( offset,  offset), // top-right
    vec2(-offset,  0.0f),   // center-left
    vec2( 0.0f,    0.0f),   // center-center
    vec2( offset,  0.0f),   // center-right
    vec2(-offset, -offset), // bottom-left
    vec2( 0.0f,   -offset), // bottom-center
    vec2( offset, -offset)  // bottom-right    
);

float kernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
);

void main() {
    // aut frame
    if ( pow( abs( 2*(TexCoords.x-0.5) ), 4) + pow( abs( 2*(TexCoords.y-0.5) ), 4) > 1 ) 
    { FragColor = vec4(1); return; }

    // bend coordiantes
    vec2 Texture_Bent = TexCoords;



    
    vec4 sampleTex[9];
    for(int i = 0; i < 9; i++) {
        sampleTex[i] = texture(screenTexture, vec2 ( Texture_Bent.x, -Texture_Bent.y ) + offsets[i]);
    }

    vec4 col = vec4(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    
    FragColor = col;
} 