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
    int new_frame = 0;
    vec2 TC = TexCoords;
    if ( int(frame) == 0 ) { discard; return; } // empty
    else if ( int(frame) == 1 ) {  } // apple
    else if ( int(frame) < 16 ) { // special
        switch ( int (frame) ) { // 16 + 32
            case 2: new_frame = 3; break;
            case 3: new_frame = 4; break;
                
            case 4: new_frame = 9; break;
            case 5: new_frame = 10; break;
                
            case 6: new_frame = 15; break;
            case 7: new_frame = 16; break;
        
            case 8: new_frame = 21; break;
            case 9: new_frame = 22; break;
        }
    } else if ( int(frame) > 16 ) { // snake
        if ( frame > 64 ) { // tail
            new_frame = 13;
            float Tx = TC.x;
            switch ( int (frame) - 80 ) { // 16 + 32
                case 2: TC.x = 1 - TC.x; break;
                case 1: break;
                case 8: TC.x = TC.y;
                        TC.y = Tx; break;
                case 4: TC.x = 1 - TC.y;
                        TC.y = Tx; break;
            }
        }
        else if ( frame > 32 ) { // head
            new_frame = 14;
            float Tx = TC.x;
            switch ( int (frame) - 48 ) { // 16 + 32
                case 2: TC.x = 1 - TC.x; break;
                case 1: break;
                case 8: TC.x = TC.y;
                        TC.y = Tx; break;
                case 4: TC.x = 1 - TC.y;
                        TC.y = Tx; break;
            }
        }
        else { // body
            new_frame = int(frame);
            // extrapolate direction
            switch ( int(frame) - 16 ) {
                case 3: new_frame = 18; break;
                case 5: new_frame = 7; break;
                case 6: new_frame = 8; break;
                case 9: new_frame = 1; break;
                case 12: new_frame = 12; break;
            }
        }
    }

    uint sprite_idx = uint(new_frame) % uint(NUM_OF_SPRITES);
    vec2 pos = vec2(int(sprite_idx) % int(SPRITE_COLUMNS), int(sprite_idx / SPRITE_COLUMNS));

    vec4 fragColor1 = spriteColor * texture(Image, vec2((TC.x / SPRITE_COLUMNS) + pos.x * (1.0 / SPRITE_COLUMNS),
        (TC.y / SPRITE_ROWS) + pos.y * (1.0 / SPRITE_ROWS)));
    // if(fragColor1.a < 0.1)
    //     discard;
    fragColor = fragColor1;
}
