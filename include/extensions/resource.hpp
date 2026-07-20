#pragma once

#include "utilities/debugger.hpp"
#include <unordered_map>
#include <string>

class Resource {
public:
	Resource ( void );
	virtual ~Resource ( void );
	virtual int Make ( );
	virtual void End ( void );
	virtual void Use ( void );
};

namespace Manager {
	extern std::unordered_map < std::string, std::shared_ptr < Resource > > _resources;

	template < class R > 
	extern inline std::shared_ptr < R > Get ( std::string name );
	template < class R >
	extern inline std::string Format_Name ( std::string name );

	// Pass the R.Make ( args )
	template < class R, typename ... Args >
	extern inline int Make ( std::string name, Args&&... args );	

	template < class R >
	inline void Register ( std::string name, std::shared_ptr < R > resource );
	
	extern void Free ( void );
}

#include "resource.tpp"