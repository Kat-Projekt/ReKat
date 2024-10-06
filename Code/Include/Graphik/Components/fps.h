#ifndef FPS_H
#define FPS_H

#include "../../objekt.hpp"
#include "../graphik.hpp"
#include "../../phisiks/phisiks.hpp"

class Fps : public Behaviour {
private:
	float _t;
	int _fps;
	float _delta;
public:
	int MAX_FPS = -1;

    void Start ( ) {
		_t = Timer::Get_Time ( );
		_fps = 0;
		_delta = 1/(float)MAX_FPS;
    }

	void Update ( ) {
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