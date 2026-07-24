#ifndef TIMER
#define TIMER

namespace Timer {
    void Update ( );
    void Fixed_Update ( );

    double Get_Time_d ( );
    float Get_Time ( );
    float Get_Delta ( );
    float Get_Fixed_Delta ( );
}

#endif