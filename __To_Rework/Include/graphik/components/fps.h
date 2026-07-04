#ifndef FPS_H
#define FPS_H

#include "../../objekt.hpp"
#include "../graphik.hpp"
#include "../../phisiks/phisiks.hpp"

class Fps : public Behaviour {
private:
	float _beatween_frames_time;
	float _t;
	int _fps;
	float _delta;
public:
	float instant_fps = 0;
	int MAX_FPS = -1;

    void Start ( ) {
		_t = Timer::Get_Time ( );
		_beatween_frames_time = _t;
		_fps = 0;
		_delta = 1/(float)MAX_FPS;
    }

	void Update ( ) {
		float _G = Timer::Get_Time ( );
		instant_fps = 1 / ( _G - _beatween_frames_time );
		_beatween_frames_time = _G;
		DEBUG ( 5, "Insant FPS: ", instant_fps );


		_fps++;
		if ( MAX_FPS > 0 ) 
		{ while ( _t + _delta*(_fps + 1) > Timer::Get_Time ( ) ) { } }

		if ( _t + 1 < Timer::Get_Time ( ) ) {
			DEBUG ( 5, "FPS: ", _fps );
			DEBUG ( 6, "Rendered ", obj->Count_Childrens (), " objekts" );
			_t = Timer::Get_Time(); 
			_fps = 0;			
		}
	}
};

#endif