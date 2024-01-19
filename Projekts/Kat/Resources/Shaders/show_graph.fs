#version 330 core
in vec2 Tex;

uniform vec2 freq[100];
uniform float time;

out vec4 fragColor;

void main (void) {
	float p = 0;
	for ( int i=0;i<100;++i ) {
		if ( freq[i].x != 0 ) 
		{ p += sin(freq[i].y*3.1415) * sin(Tex.x*freq[i].x*3.1415 + time*10)/10; }
	}
	p += 0.5;
	vec4 color1 = vec4(0.0,1.0,1.0,1.0);
	vec4 color2 = vec4(0.0,0.0,0.0,1.0);
	if ( p >= 0.5 && Tex.y >= 0.5 && Tex.y <= p ) { fragColor = color1; }
	else if ( p <= 0.5 && Tex.y <= 0.5 && Tex.y >= p ) { fragColor = color1; }
	else { fragColor = color2; }
	if ( Tex.y >= 0.499 && Tex.y <= 0.501 ) { fragColor = color1; }
}