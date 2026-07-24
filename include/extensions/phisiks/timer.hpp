#ifndef TIMER
#define TIMER

#include <chrono>

#if (defined (LINUX) || defined (__linux__)) // unix implementation
	#define TIME_SCALING 1/1000000000.0
#elif (defined (_WIN32) || defined (_WIN64)) // windows implementaion
	#define TIME_SCALING 1/10000000.0
#elif (defined (__APPLE__))
    #define TIME_SCALING 1/1000000.0
#endif

namespace Timer {
	auto start = std::chrono::system_clock::now ( );
	double inline delta_time = 0;
	double inline fixed_delta_time = 0;
	double inline current_time = 0;
	double inline current_fixed_time = 0;

	inline double Get_Time_d
	( ) {
		return (double)
		( std::chrono::system_clock::now ( ) - start ).count( )
		* TIME_SCALING;
	}
	inline float Get_Time ( ) 
	{ return (float)(Get_Time_d ( )); }

	inline float Get_Delta ( )
	{
		return delta_time;
	}
	
	inline void Update ( )
	{
		auto t = Get_Time_d ( );
		delta_time = (t - current_time);
		current_time = t;
	}
	inline void Fixed_Update ( )
	{
		auto t = Get_Time_d ( );
		fixed_delta_time = (t - current_fixed_time);
		current_fixed_time = t;
	}
}

#endif