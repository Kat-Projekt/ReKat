#ifndef ANIMATION
#define ANIMATION

#include "../../resource.hpp"
#include <vector>

enum PlayMode {
    LOOP,
    ONCE
};
template < typename T >
class Animation : public Resource {
    T* _parameter;

    struct frame {
        T initial_state;
        T final_state;

        float enter_time;
        float duration;
        // function that intrpolates between [T1,T2] and is given an input in [0,1]
        T ( *interpolator ) ( T, T, float ) = nullptr;
    };

    float current_time;
    float total_duration = 0;
    int current_frame = 0;

    std::vector < frame > frames;

    PlayMode _play;
    
public:
    Animation ( ) { }
    Animation ( T* parameter, PlayMode p ) { Make ( parameter, p ); }

	int Make ( T* parameter, PlayMode play ) {
        _parameter = parameter;
        _play = play;
		return 0;
	}

    Animation* Add_Frame ( T initial, T final, float duration, T ( *interpolator ) ( T, T, float ) = Lerp ) {
        if ( duration <= 0 ) { DEBUG (2, "the duration is not correct" ); return this; }
        frame nframe;
        nframe.initial_state = initial;
        nframe.final_state = final;
        nframe.duration = duration;
        nframe.interpolator = interpolator;

        total_duration += duration;

        if ( frames.size ( ) == 0 ) {
            nframe.enter_time = 0;
            frames.push_back ( nframe );
            DEBUG ( 4, "added first animation frame" );
            return this;
        }

        frame lframe = * ( -- frames.end ( ) );
        nframe.enter_time = lframe.enter_time + lframe.duration;
        frames.push_back ( nframe );
            DEBUG ( 4, "added animation frame, total frames: ", frames.size ( ) );
        return this;
    }

    void Use ( float _time ) {
        if ( frames.size() == 0 ) { DEBUG (2, "trying to animate an empty animation" ); }
        if ( _time < 0 ) { DEBUG (2, "NEGATIVE TIME"); return; }
        if ( _play == ONCE ) { 
            if ( _time > total_duration ) {
                DEBUG (4, "end of animation ", _time ); 
                return;
            } 
        }

        _time = std::fmod ( _time, total_duration );

        DEBUG ( 3, "animating ", current_frame, " time: ", _time, " frames: ", frames.size() );

        // get frame
        auto F = frames[current_frame];
        while ( ! ( F.enter_time < _time && _time < F.enter_time + F.duration ) ) {
            if ( _time < F.enter_time ) { current_frame --; F = frames[current_frame]; continue; }
            current_frame ++;
            F = frames[current_frame];
        }

        // begin interpolation
        _time = ( _time - F.enter_time ) / F.duration;
        if ( _parameter != nullptr ) 
        { *_parameter = F.interpolator ( F.initial_state, F.final_state, _time ); }

        DEBUG ( 3, "animating ", current_frame, " time: ", _time );
    }

	void End ( )
	{ frames.clear ( ); }
};

#endif