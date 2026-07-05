#define DIAGNOSTIC
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

extern "C" BOOST_SYMBOL_VISIBLE
std::shared_ptr < Behaviour > _Factory ( )
{
	DEBUG ( 5, "creating NewComponent" );
	return std::make_shared < NewComponent > ( );
}
