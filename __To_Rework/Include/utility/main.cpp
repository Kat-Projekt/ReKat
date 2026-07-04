#include "list.h"

#include <iostream>

class Giorgio {
private:
    int cok = 0;
public:
    long gion = 0;
    Giorgio ( ) { }
    ~Giorgio ( ) { }
    friend std::ostream& operator << ( std::ostream& os, Giorgio& list ) {
		os << "cok " << list.cok << " gion " << list.gion;
		return os;
	}
};

#include <list>

int main ( ) {
    List < Giorgio * > CaccaPupu;
    
    for ( long i = 0; i < 1000; i++ ) {
        Giorgio * C = new Giorgio;
        C->gion = i;
        CaccaPupu.append ( C );
    }

    std::cout << "Reading\n";
    for ( auto C : CaccaPupu ) 
    { std::cout << *(C) << '\n'; }
}