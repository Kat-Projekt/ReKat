#ifndef ANIMATION
#define ANIMATION

#include "../../resource.hpp"
#include <vector>
#include <utilities/math.h>

enum class PlayMode {
    LOOP,
    ONCE,
    REVERSE
};
template < typename T >
class Animation : public Resource {

	struct frame {
		T initial_state;
		T final_state;

		float enter_time;
		float duration;
		// function that intrpolates between [T1,T2] and is given an input in [0,1]
		T ( *interpolator ) ( T, T, float ) = nullptr;
	};

	std::vector < frame > _frames;

	T* _parameter = nullptr;
	float _total_duration = 0;
	PlayMode _play = PlayMode::ONCE;
public:
	Animation ( ) { }
	Animation ( T* parameter, PlayMode p ) { Make ( parameter, p ); }

	int Make ( T* parameter, PlayMode play ) {
		_parameter = parameter;
		_play = play;
		_total_duration = 0;
		return 0;
	}

	Animation* Add_Frame (
		T initial,
		T final,
		float duration,
		T ( *interpolator ) ( T, T, float ) = Lerp < T >
	) {
		if ( duration <= 0 )
		{
			DEBUG (2, "frame duration is not correct" );
			return this;
		}

		// configure new frame
		frame nframe;
		nframe.initial_state = initial;
		nframe.final_state = final;

		nframe.enter_time = _total_duration;
		nframe.duration = duration;

		nframe.interpolator = interpolator;

		// new total duration
		_total_duration += duration;

		_frames.push_back ( nframe );

		return this;
	}

	void Use ( float _time ) override
	{
		if ( _frames.size() == 0 )
		{
			DEBUG (2, "NO ANIMATION FRAMES");
			return;
		}
		if ( _time < 0 )
		{
			DEBUG (2, "NEGATIVE TIME");
			return;
		}
		if ( _parameter == nullptr )
		{
			DEBUG (2, "PARAMETER NOT SETTED");
			return;
		}

		// getting animation time
		float when_to_animate = 0;
		switch ( _play )
		{
			case PlayMode::ONCE:
				if (  _time > _total_duration ) { return; }
				else { when_to_animate = _time; }
			break;
		
			case PlayMode::LOOP:
				when_to_animate = std::fmod ( _time, _total_duration );
			break;

			case PlayMode::REVERSE:
				when_to_animate = std::fmod ( _time, _total_duration * 2 );
				// reverse in case on the second part
				if (  when_to_animate > _total_duration )
				{ when_to_animate -= _total_duration; }
			break;
		}

		// getting current_frame_index
		size_t current_frame_index = 0;
		for ( auto& _frame : _frames )
		{
			if ( _frame.enter_time > when_to_animate )
			{ break; }
			else
			{ current_frame_index ++; }
		}

		DEBUG ( 3, "animating ", current_frame_index, " time: ", when_to_animate );

		// get interpolation parameters
		frame current_frame = _frames[current_frame_index];
		float interpolator_value  = ( when_to_animate - current_frame.enter_time ) / current_frame.duration;

		// begin interpolation
		
		*_parameter = current_frame.interpolator
		(
			current_frame.initial_state,
			current_frame.final_state,
			_time
		);
	}

	void End ( ) override
	{
		_frames.clear ( );
		_total_duration = 0;
	}
};

#endif