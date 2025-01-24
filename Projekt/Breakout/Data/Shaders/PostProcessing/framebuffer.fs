#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
const float step = 1.0f/600.0f;

vec2 offsets[9] = vec2[](
    vec2( -1,  1 ), // top-left
    vec2(  0,  1 ), // top-center
    vec2(  1,  1 ), // top-right
    vec2( -1,  0 ),   // center-left
    vec2(  0,  0 ),   // center-center
    vec2(  1,  0 ),   // center-right
    vec2( -1, -1 ), // bottom-left
    vec2(  0, -1 ), // bottom-center
    vec2(  1, -1 )  // bottom-right    
);

void main() {
    // signed distance function
    if ( texture ( screenTexture, TexCoords ) == vec4(1) ) {
        FragColor = vec4(1);
        return;
    }

    float distance = 0;
    while ( distance > 1 ) {
        float fullness = 0;
        for(int i = 0; i < 9; i++) 
        { fullness += texture(screenTexture, TexCoords + offsets[i]).x; }

        if ( fullness >= 1 ) { break; }
        distance += step;
    }

    FragColor = vec4 ( sin(distance*10) );
} 