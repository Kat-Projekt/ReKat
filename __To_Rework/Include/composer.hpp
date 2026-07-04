#include "objekt.hpp"

// creates simulated objekt hierachy
// stores objekts in a list that resembles a tree structure with insersion time o(n)

class Composer {
	// table for objekts relation
	List < std::shared_ptr < Objekt > > Objekts = {};
	List < std::shared_ptr < Behaviour > > Components = {};
	Behaviour empty;

	Composer * Add_Objekt ( std::shared_ptr < Objekt > obj, std::shared_ptr < Objekt > parent )
	{
		
	}

	// table for components - objekt relation
};

int main ( ) {
}