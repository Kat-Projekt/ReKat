#define DIAGNOSTIC
#include <phisiks/components.hpp>



void Point ( vec3 pos ) {
    Hash_Map map ( 1 );
    int p = map.Normalize_and_Hash ( pos );
    DEBUG ( 4, p );
}

void Neibours ( vec3 pos, int max = 50 ) {
    Hash_Map map ( 1 );
    map.TEST_set_max ( max );
    auto n = map.Get_Neibours ( pos );
    DEBUG ( 4, n );
}

int main ( ) {
    Point ( {0,0,0} ); // expected 0
    Point ( {1,0,0} ); // expected 3
    Point ( {-1,0,0} ); // expected 5
    Point ( {2,0,0} ); // expected 13
    Point ( {0,3,0} ); // expected 25
    Point ( {1,3,0} ); // expected 26
    Point ( {2,3,0} ); // expected 27
    Point ( {3,3,0} ); // expected 28
    Point ( {3,2,0} ); // expected 29
    Point ( {3,1,0} ); // expected 30
    Point ( {3,0,0} ); // expected 31
    Point ( {3,-1,0} ); // expected 32
    Point ( {3,-2,0} ); // expected 33
    Point ( {3,-3,0} ); // expected 34
    Point ( {2,-3,0} ); // expected 35
    Point ( {1,-3,0} ); // expected 36
    Point ( {0,-3,0} ); // expected 37
    Point ( {-1,-3,0} ); // expected 38
    Point ( {-2,-3,0} ); // expected 39
    Point ( {-3,-3,0} ); // expected 40
    Point ( {-3,-2,0} ); // expected 41
    Point ( {-3,-1,0} ); // expected 42
    Point ( {-3,0,0} ); // expected 43
    Point ( {-3,1,0} ); // expected 44
    Point ( {-3,2,0} ); // expected 45
    Point ( {-3,3,0} ); // expected 46
    Point ( {-2,3,0} ); // expected 47
    Point ( {-1,3,0} ); // expected 48
    Point ( {0,4,0} ); // expected 49

    Neibours ( { 0,0,0 } ); // expected 1, 2, 3, 4, 5, 6, 7, 8
    Neibours ( { 0,0,0 }, 4 ); // expected 1, 2, 3, 4
    Neibours ( {0,4,0} ); // expected 25, 26, 48, 50

}
