#include "extensions/phisiks/timer.hpp"

namespace Timer {
	auto start = std::chrono::steady_clock::now();
	double delta_time = 0;
	double fixed_delta_time = 0;
	double current_time = 0;
	double current_fixed_time = 0;
    
	extern double Get_Time_d ( ) {
		std::chrono::duration<double> elapsed = std::chrono::steady_clock::now() - start;
		return elapsed.count();
	}

	extern float Get_Time ( ) { return static_cast<float>(current_time); }
	extern float Get_Delta ( ) { return static_cast<float>(delta_time); }
	extern float Get_Fixed_Delta ( ) { return static_cast<float>(fixed_delta_time); }

	extern void Update ( ) {
		auto t = Get_Time_d ( );
		delta_time = t - current_time;
		current_time = t;
	}

	extern void Fixed_Update ( ) {
		auto t = Get_Time_d ( );
		fixed_delta_time = t - current_fixed_time;
		current_fixed_time = t;
	}
}