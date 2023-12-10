#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H
#include <iostream>

class Behaviour {
public:
    Behaviour ( ) { }
    virtual void Start ( ) { }
    virtual void Update ( ) { std::cout << " d"; }
};

#endif