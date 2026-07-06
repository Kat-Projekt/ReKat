#include <engine.hpp>

class NewComponent : public Behaviour {
public:
	NewComponent ( )
	{
		Informations = { "NewComponent", 1.3, "It updates: We are so back" };
	}

	void Update ( ) override
	{
		DEBUG ( 3, "We are so back 2?" );
	}
};