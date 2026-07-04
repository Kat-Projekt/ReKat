#define DIAGNOSTIC
#include <engine.hpp>

class NewComponent : public Behaviour {
	void Update ( ) override
	{
		DEBUG ( 3, "We are so back" );
	}
};

extern "C" BOOST_SYMBOL_EXPORT
std::shared_ptr < Behaviour > _Factory ( )
{
	DEBUG ( 5, "Calling Factory for NewComponent" );
	return std::make_shared < NewComponent > ( );
}