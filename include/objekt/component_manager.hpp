#pragma once

#include "behaviour.hpp"
#include <utilities/debugger.hpp>

#include <boost/dll/import.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>
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
	extern int Register ( const boost::dll::fs::path& path );
	// -1 errors, n >= 0 numero di componenti trovati
	extern int Register_Directory ( const std::string& path );

	extern std::shared_ptr < Behaviour > Construct ( const std::string& name );
	template < class C >
	extern std::shared_ptr < Behaviour > Construct ( );
}

#include "component_manager.tpp"