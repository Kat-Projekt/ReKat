#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H
#include <iostream>

#include <string>
class Behaviour {
public:
    std::string name;
    Behaviour ( ) { }
    virtual void Start ( ) { }
    virtual void Update ( ) { }

    virtual void Collision ( std::string obj ) { }
    virtual void CollisionTrigger ( std::string obj ) { }
	virtual void CollisionEnter ( std::string obj ) { }
	virtual void CollisionExit ( std::string obj ) { }
	virtual void CollisionTriggerEnter ( std::string obj ) { }
	virtual void CollisionTriggerExit ( std::string obj ) { }
};

#endif