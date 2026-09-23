#pragma once

#include <functional>
#include <memory>
#include <vector>
#include <boost/config.hpp>

#include "utility/array_view.h"
#include "utility/debugger.h"
#include "reflection.h"

class Objekt;

/**
 * \defgroup Components The Component system
 */
class BOOST_SYMBOL_VISIBLE Behaviour
{
protected:
	bool _active = true;
	bool _started = false;

	// for chashed and status changes
	mutable bool _modified = true;
public:
	Objekt * obj = nullptr;

	Behaviour ( ) { }
	Behaviour ( const ComponentArguments& args )
	{ this->Set ( args ); }

	// deleting copy constructors because of instances must be unique
	Behaviour ( const Behaviour& ) = delete;
	Behaviour & operator = ( const Behaviour& ) = delete;

	// this removes the objekt from the manager
	virtual ~Behaviour ( ) = default;

	void _Start ( );
	void _Early_Update ( );
	void _Update ( );
	void _Late_Update ( );
	void _Fixed_Update ( );

	virtual void Start ( ) { }
	virtual void Early_Update ( ) { }
	virtual void Update ( ) { }
	virtual void Late_Update ( ) { }
	virtual void Fixed_Update ( ) { }

	virtual void Collision ( Objekt * ) { }
	virtual void Collision_Exit ( Objekt * ) { }
	virtual void Collision_Enter ( Objekt * ) { }

	virtual void Collision_Trigger ( Objekt* ) { }
	virtual void Collision_Trigger_Exit ( Objekt* ) { }
	virtual void Collision_Trigger_Enter ( Objekt* ) { }

	void Set_Active ( bool active ) noexcept
	{ _active = active; _Start ( ); }
	[[nodiscard]] bool Get_Active ( ) const noexcept
	{ return _active; }

	void Modify ( ) const noexcept
	{ _modified = true; }
	[[nodiscard]] bool Is_Modified ( ) const noexcept
	{ return _modified; }
	void Reset_Modify ( ) const noexcept
	{ _modified = false; }

	[[nodiscard]] virtual const ComponentMetadata& Get_Metadata ( ) const = 0;
	[[nodiscard]] virtual const char* Get_Type ( ) const = 0;
	[[nodiscard]] virtual const ArrayView < ParameterMetadata > Get_Parameters ( ) const
	{ return { nullptr, 0 }; }
	[[nodiscard]] virtual const ArrayView < MethodMetadata > Get_Methods ( ) const
	{ return { nullptr, 0 }; }

	Behaviour * Set ( const ComponentArguments& args );
	[[nodiscard]] const ComponentArguments Get ( ) const;
};
