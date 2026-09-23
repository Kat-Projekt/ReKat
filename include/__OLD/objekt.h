#pragma once
#ifndef OBJEKT_H
#define OBJEKT_H

#include <list>
#include <string>
#include <memory>
#include <functional>
#include <glm/glm.hpp>

#include "behaviour.h"
#include "component_manager.h"
#include "transform.h"

#include <scene.h>
#include <utility/debugger.h>

/************************************************************************************************
 * @file objekt.h
 * \defgroup Objekt The Objekt system
 * @brief Describes the works of the Objekt class
 * # How the Objekt system works 
 * The objekt system describes the hierarchy of objekts and which objekt own witch component.
 *
 * ## Objekt hierachy
 * An objekt owns it's childerns and when deleted it's childens are also deleted if a new parent is no specified.
 * The following are the methods interact with the Hierarchy
 * - **Add_Child** to add ore move a children
 * - **Get_Child** to get a non owing poter to a child
 * - **Has_Child** to check ownership
 * - **Rem_Child** to remove a children
 * - **Count_Children** to get how many objekt depends on this
 * 
 * ## Components
 * The following are the methods interact with the Componets
 * - **Add_Component** to add a new component
 * - **Get_Component** to get a non owing pointer to a component
 * - **Get_Component_Recursive** also gets components in children
 * - **Get_Component_In Children** gets components only in children
 * - **Has_Component** to know if a component of the specified type is present
 * - **Rem_Component** to deallocate a component
 * 
 * ## Engine Calls
 * The following methods are called by the engine on specific events
 * - **Start** only once after the objekt is started or set as active scene
 * - **Update** once every frame
 * - **Early_Update** once before Update
 * - **Late_Update** once after Update
 * - **Recursive_Caller** calls the specified function on every Get_Component_Recursive
 * - **Caller** calls the specified function on every attached component
 * 
 * note that Recursive_Caller is used for Fixed_Update ( for isolation porpuses )
 * 
 * note that Caller is used for Collisions updates ( for isolation porpuses )
 * 
 * this methods can be used for future applications
 * 
 * @see Behaviour
 * @see Transform
 * 
 ***********************************************************************************************/

/**
 * \brief The Objekt class
 * 
 * Objekt instances are uniquely owned and are intended to live on the heap.
 * They should be created through the Manager or std::make_unique ( ).
 * Functions returning Objekt * return non owning pointers.
 * 
 * @see Manager
 */
class Objekt
{
protected:
	identifier id = Unidentified;
	identifier name_id = Unidentified;
	bool _active = true;
	bool _started = false;

	Transform _transform;

	Objekt * _father = nullptr;
	std::list < identifier > _children = { };
	std::list < std::unique_ptr < Behaviour, Factory::deconstructor_t > > _components = { };
		
	/*************************************************************
	 * \brief [internal] Sets the Father of the current objekt and
	 * 	does not check for ownership
	 * 
	 * Will only be called by Add_Child, Rem_Child, and Mov_Child.
	 * \ingroup ObjHier
	 ************************************************************/
	void _Set_Father ( Objekt * father ) noexcept;

	// objekts are only constructed by scenes
	Objekt ( );
	Objekt (
		glm::vec3 pos = {0,0,0},
		glm::vec3 size = {100,100,100},
		glm::vec3 rot = {0,0,0},
		glm::vec3 rot_pivot = {0,0,0}
	);
public:
	
	// deleting copy constructors
	Objekt ( const Objekt& ) = delete;
	Objekt & operator = ( const Objekt& ) = delete;

	// moving is disabled to preserve the validity of parent and child pointers.
	Objekt ( Objekt&& ) = delete;
	Objekt & operator = ( Objekt&& ) = delete;

	// this removes the objekt from the scene
	~Objekt ( );

	Objekt * Set_Active ( bool active ) noexcept
	{ _active = active; return this; }

	bool Get_Active ( ) const noexcept
	{ return _active; }
	const std::string& Get_Name ( ) const noexcept;

	/*****************************************************************************
	 * \defgroup ObjHier Hierarchy functions
	 * \brief The objekt - objekt interface
	 * 
	 * This functions are used to define the Tree like structure of the hierarchy.
	 * Every objekt as exacly one parent and many childrens.
	 * Objekts can be moved between parents
	 * \ingroup Objekt
	 * @{
	 ****************************************************************************/

	/***********************************
	 * \brief Returns the father pointer
	 **********************************/
	[[nodiscard]]
	Objekt * Get_Father ( ) const noexcept
	{ return _father; }
	/**************************************************
	 * \brief Returns true if this Objekt has no parent.
	 *************************************************/
	bool Is_Root() const noexcept
	{ return _father == nullptr; }

	/************************************************************
	 * \brief Moves the named child ownership to new_father
	 * 
	 * This will be called by Add_Child if the father in not null
	 ***********************************************************/
	void	 Mov_Child ( const std::string& name, Objekt * new_father );
	/***********************************************************
	 * \brief Moves the ideed child ownership to new_father
	 * 
	 * This will be called by Add_Child if the father in not null
	 ***********************************************************/
	void	 Mov_Child ( identifier id, Objekt * new_father );
	/*******************************************************
	 * \brief Adds the child
	 * 
	 * Since you are passing the ownership of the child only 
	 * Set_Father will be called on the passed objekt and
	 * no ownership checks will be performed
	 ******************************************************/
	Objekt * Add_Child ( identifier child );
	/*********************************************************
	 * \brief Adds the named child
	 * 
	 * First this finds the owner then this askes for owneship
	 * after that ownership is passed and 
	 * Set_Father will be called on the passed objekt
	 ********************************************************/
	Objekt * Add_Child ( const std::string& name );
	/*****************************
	 * \brief Gets the named child
	 ****************************/
	[[nodiscard]]
	Objekt * Get_Child ( const std::string& name ) const;
	/***************************************************
	 * \brief Checks if a child with the name is present
	 **************************************************/
	[[nodiscard]]
	bool 	 Has_Child ( const std::string& name ) const;
	/**************************************
	 * \brief Removes the first named child
	 * 
	 * The named child will be deallocated
	 *************************************/
	void 	 Rem_Child ( const std::string& name );
	/***************************************
	 * \brief Removes all the named children
	 *
	 * Said children will be deallocated
	 **************************************/
	void 	 Rem_Children ( const std::string& name );
	/***********************************
	 * \brief Removes the child ideed
	 *
	 * Said children will be deallocated
	 **********************************/
	void 	 Rem_Child ( identifier id );

	/***************************************************
	 * \brief Counts the children with the passed name
	 * 
	 * if the name is empty it will count every children
	 **************************************************/
	[[nodiscard]]
	size_t Count_Children ( const std::string& name = "" ) const;
	/** @} */

	/**
	 * \defgroup Component_Manipulation Components functions
	 * \brief The component - objekt interface
	 * 
	 * This functions are used to manage the Components.
	 * Components are owned by exaxly one Objekt
	 * 
	 * You will notice that there are two ways to work with  componets:
	 * - by **Symbol** specified in the template
	 * - by **Name** specified as a string
	 * 
	 * This is because when you need to dinamically load a component you
	 * cannot inizialize it by it's symbol because at complie it won't be
	 * present, so you use the name.
	 * 
	 * @see Factory
	 * \ingroup Objekt
	 * @{
	 */

	/*****************************************************************
	 * \brief Adds the C component constructed with args
	 * 
	 * First costructs the component with args C ( args )
	 * Then it starts the component if the objekt is started
	 * 
	 * \param args the list of arguments passed to the constructor
	 * \return a non owning pointer to the newly constructed component
	 ****************************************************************/
	template < class C >
		C * Add_Component ( const ComponentArguments& args );
	/*******************************************************
	 * \brief Gets ownership of the passed componets
	 * 
	 * \return a non ownging pointer to the passed component
	 ******************************************************/
	template < class C > 
		C * Add_Component ( std::unique_ptr < C > c );
	/*******************************************************
	 * \brief Creates a new component of the specified name
	 * 
	 * This uses Factory::Costruct ( "name" ) to create the component
	 * Then Starts the component
	 * 
	 * \param type the type name of the component ( the name of the class )
	 * \return a non ownging pointer to the newly constructed component
	 ******************************************************/
	Behaviour * Add_Component ( const std::string& type );
	/*******************************************************
	 * \brief Creates a new component of the specified name
	 * 
	 * This uses Factory::Costruct ( "name" ) to create the component
	 * Then calles comp->Set ( args ) to set the parameters passed
	 * Then Starts the component
	 * 
	 * This will be used like
	 * 
	 * ```cpp
	 * obj->Add_Component (
	 * 	"Sprite",
	 * 	ComponentArguments { }
	 * 		.Set ( "texture", "town" )
	 * 		.Set ( "shader", "funky")
	 *	);
	 * ```
	 * 
	 * \param type the type name of the component ( the name of the class )
	 * \param args the list of passed parameters
	 * \return a non ownging pointer to the newly constructed component
	 * 
	 * @see ComponentArguments
	 ******************************************************/
	Behaviour * Add_Component ( const std::string& type, ComponentArguments args );

	/*********************************************************************
	 * \brief Gets the first instance of a component of the specified type
	 ********************************************************************/
	[[nodiscard]]
	template < class C >
		C * Get_Component ( ) const;
	/** \copydoc Get_Component */
	[[nodiscard]]
	Behaviour * Get_Component ( const std::string& type ) const;
	/****************************************************************
	 * \brief Gets every instance of components of the specified type
	 ***************************************************************/
	[[nodiscard]]
	template < class C >
		std::list < C * > Get_Components ( ) const;
	/** \copydoc Get_Components */
	[[nodiscard]]
	std::list < Behaviour * > Get_Components ( const std::string& type ) const;
	/**************************************************************
	 * \brief Combines Get_Components and Get_Component_In_Children
	 *************************************************************/
	[[nodiscard]]
	template < class C >
		std::list < C * > Get_Component_Recursive ( ) const;
	/** \copydoc Get_Component_Recursive */
	[[nodiscard]]
	std::list < Behaviour * > Get_Component_Recursive ( const std::string& type ) const;
	/*****************************************************************************
	 * \brief Gets every instance of components of the specified type in childrens
	 ****************************************************************************/
	[[nodiscard]]
	template < class C >
		std::list < C * > Get_Component_In_Children ( ) const;
	/** \copydoc Get_Component_In_Children */
	[[nodiscard]]
	std::list < Behaviour * > Get_Component_In_Children ( const std::string& type ) const;

	/*******************************************************************************
	 * \brief Removes the first Component from the list of componets
	 * 
	 * \return the ownership of the component so that it can be deallocated or moved
	 ******************************************************************************/
	template < class C >
		std::unique_ptr < C > Rem_Component ( );
	/** \copydoc Rem_Component */
	std::unique_ptr < Behaviour > Rem_Component ( const std::string& type );
	/**********************************************************************************
	 * \brief Removes the all Components from the list of componets
	 * 
	 * if the return is not addressed the components will be freed
	 * 
	 * \return the ownership of the components so that they can be deallocated or moved
	 *********************************************************************************/
	template < class C >
		std::list < std::unique_ptr < C > > Rem_Components ( );
	/** \copydoc Rem_Components */
	std::list < std::unique_ptr < Behaviour > > Rem_Components ( );

	/*********************************************************************
	 * \brief Checks if this Objekt owns a component of the specified name
	 ********************************************************************/
	template < class C > bool Has_Component ( ) const;
	/** \copydoc Has_Component */
	bool Has_Component ( const std::string& type ) const;

	/** @} */

	/***************************************************************************
	 * \defgroup Engine Engine function calls
	 * \brief This are functions called by the engine at specific points in time
	 * 
	 * You are not supposed to call this functions
	 * 
	 * \ingroup Objekt
	 * @{
	 **************************************************************************/
	void Start ( const char* ind = "" );
	void Early_Update ( const char* ind = "" );
	void Update ( const char* ind = "" );
	void Late_Update ( const char* ind = "" );

	template < typename Component_Call >
		void Recursive_Caller (
			const char* ind,
			const char* message,
			Component_Call component_call
		);
	template < typename Component_Call >
		void Caller (
			const char* ind,
			const char* message,
			Component_Call component_call
		);
	/** @} */

	/**************************************************************************
	 * \defgroup Transform Transformation functions
	 * \brief This are the passthough functions for the Transform component
	 * 
	 * Since every objekt has a Transform this functions point to the Transorm
	 * 
	 * So their documentation can be found in the Transform class.
	 * \ingroup Objekt
	 * @{
	 *************************************************************************/

	/************************************************
	 * \brief gets a ponter to the Transform componet
	 ***********************************************/
	[[nodiscard]] const Transform & Get_Transform ( ) const noexcept
		{ return _transform; }
	/** \copydoc Get_Transform */
	[[nodiscard]] Transform & Get_Transform ( ) noexcept
		{ return _transform; }

	Objekt* Set_Pos ( glm::vec3 pos ) noexcept
		{ _transform.Set_Pos ( pos ); return this; }
	Objekt* Set_Depth ( float z ) noexcept
		{ _transform.Set_Depth ( z ); return this; }
	Objekt* Translate ( glm::vec3 pos ) noexcept
		{ _transform.Translate ( pos ); return this; }
	[[nodiscard]] const glm::vec3 Get_Pos ( ) const noexcept
		{ return _transform.Get_Pos ( ); }
	[[nodiscard]] const glm::vec3& Get_Local_Pos ( ) const noexcept
		{ return _transform.Get_Local_Pos ( ); }

	Objekt* Set_Size ( glm::vec3 size ) noexcept
		{ _transform.Set_Size ( size ); return this; }
	Objekt* Scale ( glm::vec3 scale_factors ) noexcept
		{ _transform.Scale ( scale_factors ); return this; }
	Objekt* Scale ( float scale_factor ) noexcept
		{ _transform.Scale ( scale_factor ); return this; }
	[[nodiscard]] const glm::vec3 Get_Size ( ) const noexcept
		{ return _transform.Get_Size ( ); }
	[[nodiscard]] const glm::vec3& Get_Local_Size ( ) const noexcept
		{ return _transform.Get_Local_Size ( ); }

	Objekt* Set_Rot ( glm::vec3 rot ) noexcept
		{ _transform.Set_Rot ( rot ); return this; }
	Objekt* Set_Rot ( glm::quat rot ) noexcept
		{ _transform.Set_Rot ( rot ); return this; }
	Objekt* Set_2D_Rot ( float rot ) noexcept
		{ _transform.Set_2D_Rot ( rot ); return this; }
	Objekt* Rotate ( glm::vec3 rot ) noexcept
		{ _transform.Rotate ( rot ); return this; }
	Objekt* Rotate_2D ( float rot ) noexcept
		{ _transform.Rotate_2D ( rot ); return this; }
	[[nodiscard]] const glm::quat& Get_Local_Rotation ( ) const noexcept
		{ return _transform.Get_Local_Rotation ( ); }
	[[nodiscard]] const Transform::mono_axis_rotation Get_Rot_Mono ( ) const noexcept
		{ return _transform.Get_Rot_Mono ( ); }
	[[nodiscard]] float Get_2D_Rot ( ) const noexcept
		{ return _transform.Get_2D_Rot ( ); }

	Objekt* Set_Rot_Pivot ( glm::vec3 rot_pivot ) noexcept
		{ _transform.Set_Rot_Pivot ( rot_pivot ); return this; }
	[[nodiscard]] const glm::vec3 Get_Rot_Pivot ( ) const noexcept
		{ return _transform.Get_Rot_Pivot ( ); }

	const glm::mat4 Get_Model_Mat ( ) const noexcept
		{ return _transform.Get_Model_Mat ( ); }
	const const glm::mat4& Get_Local_Mat ( ) const noexcept
		{ return _transform.Get_Local_Mat ( ); }
	/** @} */

	/********************************************************
	 * \brief Prints the hieratchy of this Objekt
	 * 
	 * if the prameter is empty it will print something like:
	 * ```cpp
	 * Pino [ready] < Sprite [+] BoxCollider [-] >
	 * - Gino [sleep]
	 * - Nino [ready] < NinosCuston [+] >
	 * ```
	 * 
	 * \param level used to print something before a line
	 *******************************************************/
	void Print_Tree ( std::string level = "" ) const;
	friend std::ostream& operator << ( std::ostream& os, const Objekt& n );

	friend class Scene;
};

#include "objekt.tpp"
#endif