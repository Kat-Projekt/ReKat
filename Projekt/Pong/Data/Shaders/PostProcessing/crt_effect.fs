#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D noiseTexture;
uniform float time;

const float offset = 1.0 / 800.0;
const vec4 DDGreen = vec4(0.05859375,0.21875,0.05859375,1);
const vec4  DGreen = vec4(0.1875,0.3828125,0.1875,1);
const vec4  LGreen = vec4(0.54296875,0.671875,0.05859375,1);
const vec4 LLGreen = vec4(0.60546875,0.734375,0.05859375,1);

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

int sgn ( float t ) {
    if ( t > 0 ) { return 1; }
    return -1;
}

void main() {
    // aut frame
    

    // bend coordiantes
    vec2 TC = TexCoords;
    TC.x = 2*(TexCoords.x-0.5);
    TC.y = -2*(TexCoords.y-0.5);

    TC.x = ( TC.x + TC.x * pow(abs(TC.y/4),2) );
    TC.y = ( TC.y + TC.y * pow(abs(TC.x/4),2) );
    
    if ( abs(TC.x) > 1 || abs(TC.y) > 1 ) 
    { FragColor = vec4(0); return; }

    TC.x = (TC.x+1) * 0.5;
    TC.y = (TC.y+1) * 0.5;

    // applicate blur
    vec4 sampleTex[9];
    for(int i = 0; i < 9; i++) {
        // scanlines
        if ( int( ( TC.y + offsets[i].y )* 1000 + time*4) % 9 < 1 )
        { sampleTex[i] = vec4(0); } 
        else 
        { sampleTex[i] = texture(screenTexture, TC + offsets[i]); }
        
    }

    FragColor = vec4(0.0);
    for(int i = 0; i < 9; i++)
        FragColor += sampleTex[i] * kernel[i];
    
    // no blur
    // FragColor = texture( screenTexture, TC );

    // posterization

    float luminance = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    if ( luminance < 0.25 ) 
    { FragColor = DDGreen; }
    else if ( luminance < 0.5 ) 
    { FragColor = DGreen; }
    else if ( luminance < 0.75 ) 
    { FragColor = LGreen; }
    else 
    { FragColor = LLGreen; }
    
    // add scann error
    float start = TC.y + time/3;
    if ( int( start * 1000 ) % 4000 < 100 ) {
        float coeeficient = float( int( start * 1000 ) % 4000 ) / 100;
        if ( coeeficient > 0.85 ) 
        { FragColor = FragColor - vec4(0.09) * ( 1.5 - coeeficient * 2 ); } 
        else 
        { FragColor = FragColor - vec4(0.09) * coeeficient; }
    }

    FragColor += texture(noiseTexture, vec2 (TC.x, TC.y+time*0.01)) * 0.01;
} 