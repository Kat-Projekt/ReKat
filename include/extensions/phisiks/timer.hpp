#ifndef TIMER
#define TIMER

#include <chrono>

namespace Timer {
	auto inline start = std::chrono::steady_clock::now();
	double inline delta_time = 0;
	double inline fixed_delta_time = 0;
	double inline current_time = 0;
	double inline current_fixed_time = 0;

	inline double Get_Time_d() 
	{
		auto now = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsed = now - start;
		return elapsed.count();
	}

	inline float Get_Time ( ) 
	{ return (float)(Get_Time_d ( )); }

	inline float Get_Delta ( )
	{
		return static_cast<float>(delta_time);
	}

	inline float Get_Fixed_Delta()
	{
		return static_cast<float>(fixed_delta_time);
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