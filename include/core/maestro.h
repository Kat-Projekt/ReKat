#pragma once
#ifndef MAESTRO_H
#define MAESTRO_H

#include <boost/dll/import.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>
#include <functional>
#include <memory>
#include <unordered_map>
#include <string>

#include "behaviour.h"
#include "reflection"

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
class Factory
{
public:
	typedef Behaviour * ( * constructor_t ) ( void );
	typedef void ( * deconstructor_t ) ( Behaviour * );
private:
	typedef struct {
		constructor_t constructor;
		deconstructor_t deconstructor;

		ComponentMetadata metadata;

		std::unique_ptr < boost::dll::shared_library > lifeline;
	} componentLibraryFunctions;

	static std::unordered_map
	<
		std::string,
		componentLibraryFunctions
	> constructors;

	static std::unordered_map
	<
		std::string,
		std::string
	> aliases;

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
	[[nodiscard]] static std::size_t _Register ( componentLibraryFunctions&& comp );
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
	[[nodiscard]] static std::size_t Register_Directory ( const boost::dll::fs::path& path );

	/*****************************************************
	 * \brief Registers the templated component
	 * 
	 * It uses the reflection sistem for component detalis
	 * 
	 * \tparam The simbol of the component
	 * \return 1 on registration, 0 on failed
	 ****************************************************/
	template < class C >
	[[nodiscard]] static std::size_t Register ( );
	/*****************************************************
	 * \brief Registers the component in the specied file
	 * 
	 * It uses the reflection sistem for component detalis
	 * If duplicate if registers the old one remains
	 * 
	 * \param path Where the component is located
	 * \return 1 on registration, 0 on failed
	 ****************************************************/
	[[nodiscard]] static std::size_t Register ( const boost::dll::fs::path& path );

	/********************************************************
	 * \brief Contructs the templated component
	 * 
	 * If the component is not registered it registers it
	 * If there is an error in costruction it returns nullptr
	 * 
	 * \tparam The simbol of the component
	 * \return An owning pointer to the Behaviour
	 *******************************************************/
	template < class C >
	[[nodiscard]] static std::unique_ptr < C, deconstructor_t > Construct ( );
	/***********************************************************
	 * \brief Contructs the templated component
	 * 
	 * If the component is not registered it registers it
	 * If there is an error in costruction it returns nullptr
	 * 
	 * \tparam The simbol of the component
	 * \param args the arguments in the ComponentArgments format
	 * \return An owning pointer to the Behaviour
	 **********************************************************/
	template < class C >
	[[nodiscard]] static std::unique_ptr < C, deconstructor_t > Construct ( const ComponentArguments& args );
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
	[[nodiscard]] static std::unique_ptr < Behaviour, deconstructor_t > Construct (
		const std::string& name,
		bool stable = true,
		ComponentVersion version = 0
	);
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
	 * \param args  the arguments in the
	 * 	ComponentArgments format
	 * \return An owning pointer to the Behaviour
	 ***************************************************/
	[[nodiscard]] static std::unique_ptr < Behaviour, deconstructor_t > Construct (
		const std::string& name,
		const ComponentArguments& args,
		bool stable = true,
		ComponentVersion version = 0
	);
};

#include "component_manager.tpp"
#endif
