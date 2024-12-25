#ifndef TIMER
#define TIMER

#include <chrono>

#if (defined (LINUX) || defined (__linux__) || defined (__APPLE__)) // unix implementation
	#define TIME_SCALING 1/1000000000.0
#elif (defined (_WIN32) || defined (_WIN64)) // windows implementaion
	#define TIME_SCALING 1/10000000.0
#endif

namespace Timer {
    auto start = std::chrono::system_clock::now();
    float delta_time = 0;
    float fixed_delta_time = 0;
    double current_time = 0;
    double current_fixed_time = 0;
    double Get_Time_d ( ) 
    { return (double)( std::chrono::system_clock::now ( ) - start ).count( ) * TIME_SCALING; }
    float Get_Time ( ) 
	{ return (float)(Get_Time_d()); }
    void Update ( ) {
		auto t = Get_Time_d ( );
        delta_time = (float)(t - current_time);
        current_time = t;
        DEBUG ( 3, "Time up to date" );
    }
	void Fixed_Update ( ) {
		auto t = Get_Time_d ( );
        fixed_delta_time = (float)(t - current_fixed_time);
        current_fixed_time = t;
    }
}

#endif