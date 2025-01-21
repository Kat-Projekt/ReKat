#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D noiseTexture;
uniform float time;

const vec4 blue = vec4 ( 0,0,1, 0.5 );
const vec4 red  = vec4 ( 1,0,0, 0.5 );

const vec2 blue_dir = vec2 ( 2,-1.4 );
const vec2 red_dir = vec2 ( -2,1.4 );

void main() {
    vec2 TC = TexCoords;

    float distance = 0.01 * int( pow ( sin ( time * 0.3 ),2 ) * 1.7 ) / 2;

    if ( distance == 0 ) 
    { FragColor = texture ( screenTexture, TC ); return; }
    
    TC.x += 0.02 * ( TC.y - 0.3) ;
    FragColor = vec4 (0);
    if ( abs ( TC.x ) > 1 ) { return; }
    
    // random glitched lines
    float thichness = 0.002;
    float padding = -0.5;
    float distorcion = 0.03;
    if ( TC.y > sin ( time * 0.2) / 1.5 - thichness + padding && 
         TC.y < sin ( time * 0.2) / 1.5 + thichness + padding ) {
        FragColor += red;
        TC.x += distorcion;
    }

    if ( TC.y > cos ( time ) / 1.5 - thichness + padding && 
         TC.y < cos ( time ) / 1.5 + thichness + padding ) {
        FragColor += blue;
    }

    distance *= texture ( noiseTexture, ( TC + vec2 ( sin( time ), cos ( time ) ) * 0.2 ) * 10 ).x;
    FragColor += 
    texture ( screenTexture, TC ) +
    texture ( screenTexture, TC + blue_dir * distance ) * blue +
    texture ( screenTexture, TC + red_dir  * distance ) * red;
} 