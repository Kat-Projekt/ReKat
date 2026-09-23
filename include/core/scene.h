#include "direktor.h"
#include <list>

class Scene
{
	std::list < Aktor* > aktors;
public:
	void Start ( );
	void Update ( );
	
	void Add ( Aktor * );
	void Rem ( Aktor * );
};
