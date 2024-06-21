#ifndef TIMER
#define TIMER

#include <chrono>

namespace Timer {
    auto start = std::chrono::system_clock::now();
    float delta_time = 0;
    float fixed_delta_time = 0;
    double current_time = 0;
    double current_fixed_time = 0;
    double Get_Time_d ( ) 
    { return (double)( std::chrono::system_clock::now ( ) - start ).count( ) / 10000000.0; }
    float Get_Time ( ) 
	{ return (float)(Get_Time_d()); }
    void Update ( ) {
		auto t = Get_Time_d ( );
        delta_time = t - current_time;
        current_time = t;
    }
	void Fixed_Update ( ) {
		auto t = Get_Time_d ( );
        fixed_delta_time = t - current_fixed_time;
        current_fixed_time = t;
    }
}

#endif