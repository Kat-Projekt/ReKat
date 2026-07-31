#ifndef TIMER
#define TIMER

#include <chrono>

namespace Timer {
	extern std::chrono::steady_clock::time_point start;
	extern double delta_time;
	extern double fixed_delta_time;
	extern double current_time;
	extern double current_fixed_time;

	extern void Update ( );
	extern void Fixed_Update ( );

	extern double Get_Time_d ( );
	extern float Get_Time ( );
	extern float Get_Delta ( );
	extern float Get_Fixed_Delta ( );
}

#endif