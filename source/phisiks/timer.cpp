#include "extensions/phisiks/timer.hpp"
#include <chrono>

namespace Timer {
    namespace {
        auto start = std::chrono::steady_clock::now();
        double delta_time = 0;
        double fixed_delta_time = 0;
        double current_time = 0;
        double current_fixed_time = 0;
    }

    double Get_Time_d ( ) {
        std::chrono::duration<double> elapsed = std::chrono::steady_clock::now() - start;
        return elapsed.count();
    }

    float Get_Time ( ) { return (float)Get_Time_d ( ); }
    float Get_Delta ( ) { return static_cast<float>(delta_time); }
    float Get_Fixed_Delta ( ) { return static_cast<float>(fixed_delta_time); }

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