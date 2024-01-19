#ifndef COLLISION_H
#define COLLISION_H

#include "object.h"

class Collider : public Behaviour {
private:
	/* data */
public:

};

// the collisions are between near objects
namespace Collision {
	void Setup ( std::vector < Object* > );
	void Check_collisions ( );
}

#endif