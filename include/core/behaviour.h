#pragma once
#ifndef BEHAVIOUR
#define BEHAVIOUR

#include "reflection/reflection"
#include "export.h"

#include <atomic>
#include <mutex>

class Aktor;

/**
 * \defgroup Components The Component system
 */
class EXPORT Behaviour
{
private:
	/***********************************************************
	 * \brief [internal] helper function for reflection function
	 *
	 * Do not ovveride this function manually use the 
	 * METHODS(...) macro
	 **********************************************************/
	virtual Reflection::Value _Perform 
	( const std::string&, const Reflection::Values& )
	{ return Reflection::Value ( ); }
	/***********************************************************
	 * \brief [internal] helper function for reflection function
	 *
	 * Do not ovveride this function manually use the 
	 * METHODS(...) macro
	 **********************************************************/
	virtual std::vector < std::string > _Reflected_Methods
	( ) const
	{ return {}; }
	/***********************************************************
	 * \brief [internal] helper function for reflection function
	 *
	 * Do not ovveride this function manually use the 
	 * PARAMETERS(...) macro
	 **********************************************************/
	virtual void _Configure
	( const Reflection::Values& )
	{ }
	/***********************************************************
	 * \brief [internal] helper function for reflection function
	 *
	 * Do not ovveride this function manually use the 
	 * PARAMETERS(...) macro
	 **********************************************************/
	virtual Reflection::Value _Query
	( const std::string& ) const
	{ return Reflection::Value ( ); }
	/***********************************************************
	 * \brief [internal] helper function for reflection function
	 *
	 * Do not ovveride this function manually use the 
	 * PARAMETERS(...) macro
	 **********************************************************/
	virtual std::vector < std::string > _Reflected_Parameters
	( ) const
	{ return {}; }
	/***********************************************************
	 * \brief [internal] helper function for reflection function
	 *
	 * Do not ovveride this function manually use the 
	 * METADATA(...) macro
	 **********************************************************/
	virtual Reflection::Metadata _Metadata
	( ) const
	{ return Reflection::Metadata ( ); }

private:
	/***************************************
	 * \brief Is this Behaviour performable?
	 **************************************/
	std::atomic < bool > _active = true;
	/*****************************************
	 * \brief Has this Behaviour been Started?
	 ****************************************/
	std::atomic < bool > _started = false;
	/*******************************************************
	 * \brief Used for signaling chashed status invalidation
	 ******************************************************/
	mutable std::atomic < bool > _modified = true;
	/*************************************************
	 * \brief Used as a gate for preventing concurrent
	 * 	  modification of derived class properties
	 ************************************************/
	std::mutex executing;
	/**********************************************
	 * \brief Used for preventing mutable cash rush
	 *********************************************/
	mutable std::mutex quering;
protected:
	/**************************************
	 * \brief Reference to the binded Aktor
	 *
	 * Since the component life is binded
	 * to the aktor this pointer is const
	 *************************************/
	Aktor * akt = nullptr;

public:
	// default constructor 
	Behaviour ( ) = default;
	// default deconstructor
	virtual ~Behaviour ( ) = default;

	// deleting copy constructors because instances must be unique
	Behaviour ( const Behaviour& ) = delete;
	Behaviour & operator = ( const Behaviour& ) = delete;

	// publically callable update functions (Aktor->...) + _active / start gates
	void _Start ( ); 
	void _Early_Update ( ) { _Internal_Caller ( &Behaviour::Start ); }
	void _Update ( ) {_Internal_Caller ( &Behaviour::Update ); }
	void _Late_Update ( ) {_Internal_Caller ( &Behaviour::Late_Update ); }
	void _Fixed_Update ( ) {_Internal_Caller ( &Behaviour::Fixed_Update ); }

	// internal router for update class functions
	void _Internal_Caller ( void ( Behaviour::* function ) ( void ) );
	
	// Collision / Trigger routing + _start / _active gates
	void _Collsion_Router ( Aktor *, int mode, bool );
private:
	/**************************************************
	 * \brief Function used for settin up the behaviour
	 *
	 * Will be called before every Update / Collision.
	 * It is called exacly once per component
	 *************************************************/
	virtual void Start ( ) { }
	/*******************************************
	 * \brief This is called first on each frame
	 *
	 * See Update for more infos
	 ******************************************/
	virtual void Early_Update ( ) { }
	/**********************************************
	 * \brief Called every frame after Early Update
	 *
	 * Note that this is per Scene, so
	 *
	 * Scene                                        
	 * │                                            
	 * ├─ Early Update                              
	 * │  ├─ Aktor1                                 
	 * │  │  └─ Early Update                        
	 * │  │     ├── Behaviour1.Early_Update ( )     
	 * │  │     ├── Behaviour2.Early_Update ( )     
	 * │  │     └── ...                             
	 * │  ├─ Aktor2                                 
	 * │  │  └─ Early Update                        
	 * │  │     └── ...                             
	 * │  └─ ...                                    
	 * │                                            
	 * ├─ Update                                    
	 * │  ├─ Aktor1                                 
	 * │  │  └─ Update                              
	 * │  │     ├── Behaviour1.Update ( )     
	 * │  │     ├── Behaviour2.Update ( )     
	 * │  │     └── ...                             
	 * │  ├─ Aktor2                                 
	 * │  │  └─ Update                              
	 * │  │     └── ...                             
	 * │  └─ ...                                    
	 * │                                            
	 * └─ Late Update                              
	 *    ├─ Aktor1                                 
	 *    │  └─ Late Update                        
	 *    │     ├── Behaviour1.Late_Update ( )     
	 *    │     ├── Behaviour2.Late_Update ( )     
	 *    │     └── ...                             
	 *    ├─ Aktor2                                 
	 *    │  └─ Late Update                        
	 *    │     └── ...                             
	 *    └─ ...     
	 *
	 *********************************************/
	virtual void Update ( ) { }
	/**********************************************
	 * \brief This is called before the frame's end
	 *
	 * See Update for more infos
	 *********************************************/
	virtual void Late_Update ( ) { }
	/*******************************************************
	 * \brief This is called when the Phisiks system updates
	 *
	 * To activate the calling of this function you
	 * must have activated the Phisiks system.
	 * Then this function is called PHISIKS_FRAMES / s.
	 * This is a indipendent call from the Update calls
	 * So it can be run concurrently
	 ******************************************************/
	virtual void Fixed_Update ( ) { }
	
	
	/*********************************************************
	 * \brief Called when Entering collision with another aktor
	 *
	 * Requires the Phisiks system
	 * \param other The Aktor colliding with
	 ********************************************************/
	virtual void Collision_Enter ( Aktor * ) { }
	/***********************************************************
	 * \brief Called while activelly colliding with another aktor
	 *
	 * Requires the Phisiks system
	 * \param other The Aktor colliding with
	 **********************************************************/
	virtual void Collision ( Aktor * ) { }
	/********************************************************
	 * \brief Called when Exiting collision with another aktor
	 *
	 * Requires the Phisiks system
	 * \param other The Aktor colliding with
	 *******************************************************/
	virtual void Collision_Exit ( Aktor * ) { }


	/***********************************************
	 * \brief Called when Entering a trigger collider
	 *
	 * Requires the Phisiks system
	 * \param other The Aktor that triggered
	 **********************************************/
	virtual void Collision_Trigger_Enter ( Aktor* ) { }
	/*************************************************
	 * \brief Called while a trigger collider is active
	 *
	 * Requires the Phisiks system
	 * \param other The Aktor that triggered
	 ************************************************/
	virtual void Collision_Trigger ( Aktor* ) { }
	/**********************************************
	 * \brief Called when Exiting a trigger collider
	 *
	 * Requires the Phisiks system
	 * \param other The Aktor that triggered
	 *********************************************/
	virtual void Collision_Trigger_Exit ( Aktor* ) { }
public:
	/************************************************
	 * \brief Sets the active status of the Behaviour
	 *
	 * \param active The new status
	 ***********************************************/
	void Set_Active ( bool active ) noexcept
	{ _active = active; }
	/******************************
	 * \brief Get the active status
	 * 
	 * \return The Active flag value
	 ******************************/
	[[nodiscard]] bool Get_Active ( ) const noexcept
	{ return _active; }


	/*************************************************
	 * \brief Tells the component it has been modified
	 *
	 * Works also with const objekts
	 ************************************************/
	void Modify ( ) const noexcept
	{ _modified = true; }
	/**************************************************
	 * \brief Check if this component has been modified
	 *
	 * \return True if behaviour has been modified
	 *************************************************/
	[[nodiscard]] bool Is_Modified ( ) const noexcept
	{ return _modified; }
	/**************************************************
	 * \brief Tells the Behaviour that the modification
	 * 	has been addressed by the program
	 *************************************************/
	void Reset_Modify ( ) const noexcept
	{ _modified = false; }


	/*********************************************************************
	 * \brief Calles the named function with the specified arguments
	 *
	 * Note that the caller is responsible of providing the right
	 * arguments types, and function name.
	 *
	 * Examples:
	 * - The Behaviour reflects the Speak function
	 *   - void Speak ( std::string message, int intensity )
	 * - The Caller calles:
	 *   - beh.Perform ( "Speak", arg(message)="hello", arg(intensity)=1 );
	 *   - beh.Perform ( "Speak", arg(intensity)=1, arg(message)="hello" );
	 * - Both calls resolve to: Speak ( "hellp", 1 );
	 *
	 * - The Caller calles
	 *   - beh.Perform ( "Speasssk", ... ):
	 *   - beh.Perform ( "Speak", arg(banana)=4.2 )
	 *   - beh.Perform ( "Speak", arg(message)=1, arg(intensity)=1 );
	 * - The first call Throws with: missing function "Speasssk"
	 * - The second Throws with: missing argument "message"
	 * - The third Throws with: type mismatch for "message"
	 *
	 * \param name The function name
	 * \tparam args The function arguments: arg(name)=value, ...
	 ********************************************************************/
	template < typename ... Args >
	Reflection::Value Perform ( const std::string& name, Args&...args )
	{
		std::lock_guard < std::mutex > execution_lock ( executing );

		if ( _active && _started )
		{
			/* create empty state */
			Reflection::Values values;
			/* folds parameters */
			( values.operator, ( std::forward <Args> (args) ), ... );
		
			return _Perform ( name, values );

		} else {
			return Reflection::Value {};
		}
	}
	/*********************************************
	 * \brief Gets the reflected functions names
	 *
	 * Note that the parameter list is not exposed
	 * Only the function name is provvided
	 *
	 * \return A vector containg the names
	 ********************************************/
	std::vector < std::string >
	Reflected_Methods ( ) const
	{ return _Reflected_Methods ( ); }
	/*********************************************************
	 * \brief Configures the Behaviour parameters
	 *
	 * Note that the caller is resposible for type matching
	 *
	 * Examples:
	 * - The Behaviour reflects the pino and carlo parameters
	 *   - int pino; carameter
	 *   - std::string carlo;
	 * - The Caller calles
	 *   - beh.Configure ( arg(pino)=10, arg(carlo)="ciao" );
	 *   - beh.Configure ( arg(carlo)="ciao", arg(pino)=10 );
	 * - Both calls resolve to: pino=10 and carlo="ciao"
	 *
	 * - The Caller calles
	 *   - beh.Configure ( arg(pino)=10 );
	 *   - beh.Configure ( arg(carlo)="ciao" );
	 * - The first call sets pino=10
	 * - The second call sets carlo="ciao"
	 * - Note that they are set indipendently that means that
	 *   - (start) pino=default, carlo=default
	 *   - (I call) pino=10, carlo=default
	 *   - (II call) pino=10, carlo="ciao"
	 *
	 * - The Caller calles
	 *   - beh.Configure ( arg(pino)="10", arg(carlo)="ciao" );
	 *   - beh.Configure ( arg(pino) );
	 *   - beh.Configure ( arg(banana)=4.2 );
	 * - The first call Throws with: type mismatch for "pino"
	 * - The second call Throws with: type mismatch for "pino"
	 * - The third call Throws with: missing parameter "banana"
	 *
	 * \tparam params The passed parameters: arg(name)=value
	 ********************************************************/
	template < typename ... Args > 
	void Configure ( Args& ... params )
	{
		// same thread as the Update class functions
		std::lock_guard < std::mutex > execution_lock ( executing );
		
		/* create empty state */
		Reflection::Values values;
		/* folds parameters */
		( values.operator, ( std::forward <Args> (params) ), ... );

		_Configure ( values );
	}
	/***************************************************************
	 * \brief This function returns the value of the named parameter
	 *
	 * Note that the user is responsible for name correctnes
	 * and return type conversion.
	 *
	 * Examples:
	 * - The Behaviour reflects the pino and carlo parameters
	 *   - int pino = 10;
	 *   - std::string carlo = "ciao";
	 * - The Caller calles
	 *   - int pinoaaa = beh.Query ( "pino" );
	 *   - std::string carlo = beh.Query ( "carlo" );
	 * - Both work as expecte: pinoaaa=10, carlo="ciao"
	 *
	 * - THe Caller calles
	 *   - auto pino = beh.Query ( "pino" );
	 * - This is not what you expect the expression expands to
	 *   - Reflected::Value pino = {"pino", 10};
	 * - Do not use auto with Query
	 *
	 * - The Caller calles
	 *   - std::string pino = beh.Query ( "pino" );
	 *   - int aaaaa = beh.Query ( "aaaaa" );
	 * - The first call Throws with: type mismatch "pino"
	 * - The second call Throws with: missing parameter "aaaaa"
	 **************************************************************/
	Reflection::Value Query
	( const std::string& parameter ) const
	{
		// all querring appens on a separa thead than the Update class
		// functions since it is constant
		std::lock_guard < std::mutex > query_lock ( quering );
		
		return _Query ( parameter );
	}
	/********************************************
	 * \brief Gets the reflected parameters names
	 *
	 * Does not differentiate between Parameters
	 * and Properties.
	 *
	 * \return A vector containg the names
	 *******************************************/
	std::vector < std::string >
	Reflected_Parameters ( ) const
	{ return _Reflected_Parameters ( ); }
	/*******************************************************
	 * \brief This function return the metadata informations
	 ******************************************************/
	virtual Reflection::Metadata Metadata
	( ) const
	{ return _Metadata ( ); }
};

#endif
