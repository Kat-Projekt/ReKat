#include <engine.hpp>

class NewComponent : public Behaviour {
	void Update ( )
	{
		DEBUG ( 2, 'We are so back' );
	}
};

extern "C++" BOOST_SYMBOL_EXPORT NewComponent NewComp;
NewComponent NewComp;