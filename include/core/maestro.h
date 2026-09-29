#pragma once
#ifndef MAESTRO
#define MAESTRO

#include <boost/dll.hpp>
#include <boost/dll/import.hpp>
#include <boost/filesystem.hpp>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

#include "behaviour.h"
#include "reflection/reflection"

/**********************************************************************************************
 * \brief This class is used for registering and creating components
 * 
 * The components Hinherit the Behaviour class.
 * The Factory maintains process-lifetime static registration state.
 * The executable and all component DLLs must link against the same ReKat.dll.
 * After a component is registerd it cannot be unloaded untill end of program
 * 
 * The adding of new components follows the following table
 * 
 * |      Type       | comp-stable | comp-latest | comp-version  |
 * | --------------- | - | - | - |
 * | first register  | v | v | v |
 * | with stable tag | v | x | v |
 * | gratest version | x | v | v |
 * | old version     | x | x | v |
 * 
 * If there is a combination of factors the boolean or will be used to decide:
 * if a component is stable + grater version everythin will be set.
 * 
 * CA.dll
 * exposed a component "Pino" of version 1.0
 * 
 * CB.dll
 * exposes a component "Pino" of version 2.1 stable
 * 
 * CC.dll
 * exposes a component "Pino" of version 2.3
 * 
 * main.cpp
 * -> Register ( "CA.dll" ); -> (
 * 	registers "Pino" of version 1.0 as 
 * 		"Pino-latest",
 * 		"Pino-stable" and
 * 		"Pino-1.0" )
 * -> Construct ( "Pino" ); -> ( construct a "Pino" component of version 1.0 )
 * -> Register ( "CB.dll" ); -> (
 * 	registers "Pino" of version 2.0 as "Pino-latest" and "Pino-stable"
 * 	renames: "Pino" version 1.0 to "Pino_1.0"
 * )
 * -> Construct ( "Pino" ); -> ( construct a "Pino" component of version 2.0 )
 * -> Construct ( "Pino_1.0" ); -> ( construct a "Pino" component of version 1.0 )
 * -> Register ( "CC.dll" ); -> (
 * 	registers "Pino" of version 2.3 only as "Pino-latest"
 *	the other "Pino_*" remain untuched
 * )
 * 
 * This apporoched is used for sharing components via networks. so you can be on the stable
 * or experimental branch ( the default is stable ).
 **********************************************************************************************/
class Maestro
{
private:
	// component container Factory struct
	typedef struct {
		std::function < Behaviour * ( void ) > constructor;
		std::function < void ( Behaviour * ) > deconstructor;

		Reflection::Metadata metadata;

		std::shared_ptr < boost::dll::shared_library > lifeline;
	} componentLibraryFunctions;
	// unique Behviour pointer with custom deconstructor
	typedef std::unique_ptr < Behaviour, std::function < void ( Behaviour * ) > > uniqueBehaviour;

	// Factory container for containers
	std::unordered_map
	<
		std::string,
		componentLibraryFunctions
	> _factories;
	// aslias table for versioning
	std::unordered_map
	<
		std::string,
		std::string
	> _aliases;

	/*******************************************
	 * \brief [internal] Registers the component
	 * 
	 * Registers the provided component
	 * inplements the naming conventions
	 * copies the provvided component
	 * 
	 * \tparam The simbol of the component
	 * \return 1 on registration, 0 on failed
	 ******************************************/
	[[nodiscard]] std::size_t _Register ( componentLibraryFunctions comp );
public:
	/*************************************************
	 * \brief Registers every component in a directory
	 * 
	 * It ignores every non library file.
	 * Duplicate registration count as 1 registration
	 * and follow the rules explaind before.
	 * Failed registration counts as 0
	 * 
	 * \param path The path of the directory
	 * \return The number of registered components
	 ************************************************/
	[[nodiscard]] std::size_t Register_Directory ( const boost::dll::fs::path& path );
	/*****************************************************
	 * \brief Registers the component in the specied file
	 * 
	 * It uses the reflection sistem for component detalis
	 * If duplicate if registers the old one remains
	 * 
	 * \param path Where the component is located
	 * \return 1 on registration, 0 on failed
	 ****************************************************/
	[[nodiscard]] std::size_t Register ( const boost::dll::fs::path& path );


	/****************************************************
	 * \brief Contructs the specified component
	 * 
	 * It Throws if there is an error in costruction,
	 * only in debugging. Else it returns nullptr
	 * It Throws if the version is not stored
	 * If a specific version is mentioned ( version != 0 )
	 * the specified version will be constuced
	 * 
	 * \param name The registerd name of the component
	 * \param stable Choose the stable branch of latest
	 * \param version The version string, used in case
	 * 	you need to use a specific one.
	 * \return An owning pointer to the Behaviour
	 ***************************************************/
	[[nodiscard]] uniqueBehaviour Construct (
		const std::string& name,
		bool stable = true,
		uint64_t major = 0,	
		uint64_t minor = 0,
		uint64_t patch = 0
	);


	/*******************************************************
	 * \brief Gets the metadata of the registered components
	 * 
	 * Resturs a vector containing the components metadata
	 * used for inspecting components registration
	 * 
	 * \return The metadata vector
	 ******************************************************/
	[[nodiscard]] std::vector < Reflection::Metadata >
	Get_Registered_Components ( );
	/********************************
	 * \brief Gets the aliasses table
	 * 
	 * \return The aliasses table ref
	 *******************************/
	[[nodiscard]] const std::unordered_map < std::string, std::string > &
	Get_Registered_Components_Aliases ( );
};

#endif
