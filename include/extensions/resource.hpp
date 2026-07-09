#pragma once

#include "utilities/debugger.hpp"
#include <unordered_map>
#include <string>

class Resource {
public:
	Resource ( void );
	virtual int Make ( );
	virtual void End ( void );
	virtual void Use ( void );
};

namespace Manager {
	template < class R > 
	extern inline std::shared_ptr < R > Get ( std::string name );
	
	extern void Free ( void );    
}