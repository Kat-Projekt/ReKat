#pragma once

#include "behaviour.hpp"
#include <utilities/debugger.hpp>

#include <boost/dll/import.hpp>
#include <boost/filesystem/path.hpp>
#include <functional>
#include <memory>
#include <unordered_map>
#include <string>

namespace Factory
{
	extern std::unordered_map
		<
			std::string,
			// This is the constructor function
			std::function < std::shared_ptr < Behaviour> ( ) >
		> constructors;

	template < class C >
	extern int Register ( );
	/*
		Remember that the dll must export this symbol

		extern "C" BOOST_SYMBOL_EXPORT
		std::shared_ptr < Behaviour > _Factory ( )
		{ return std::make_shared < Comp > ( ); }
	*/
	extern int Register ( const std::string& name, const std::string& path );

	extern std::shared_ptr < Behaviour > Construct ( const std::string& name );
	template < class C >
	extern std::shared_ptr < Behaviour > Construct ( );
}

#include "component_manager.tpp"