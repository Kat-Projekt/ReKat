#ifndef FPS_H
#define FPS_H

#include "../objekt.h"
#include "../graphik.hpp"

class Fps : public Behaviour {
private:
	float _t;
	int _fps;
	float _delta;
public:
	int MAX_FPS = 60;

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
			std::cout << "\n---- Fps: " << _fps << '\n';
			std::cout << "---- Rendered: " << obj->Get_Childrens ().size() << '\n';
			_t = Timer::Get_Time(); 
			_fps = 0;
		}
	}
};

#endif