#include <engine.hpp>

class Giorgio : public Behaviour {
public:
	Giorgio ( )
	{
		Informations = { "Giorgio", 2.0, "Nulla" };
	}

	void Update ( )
	{
		DEBUG ( 3, "We are so back 2?" );
	}
};