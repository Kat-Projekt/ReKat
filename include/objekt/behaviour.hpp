#pragma once

#include <functional>
#include <memory>
#include <vector>
#include <string>
#include <boost/config.hpp>
#include <utilities/debugger.hpp>

class Objekt;

class BOOST_SYMBOL_VISIBLE Behaviour : public std::enable_shared_from_this<Behaviour>
{
protected:
	bool _active = true;
	bool _started = false;

public:
	std::shared_ptr < Objekt > obj = nullptr;

	// override the Constructor for adding metadata
	Behaviour ( );
	// override the Delete function not this
	virtual ~Behaviour ( );

	void _Start ( );
	void _Early_Update ( );
	void _Update ( );
	void _Late_Update ( );
	void _Fixed_Update ( );

	virtual void Start ( );
	virtual void Early_Update ( );
	virtual void Update ( );
	virtual void Late_Update ( );
	virtual void Fixed_Update ( );

	virtual void Collision ( std::shared_ptr < Objekt > _obj );
	virtual void Collision_Exit ( std::shared_ptr < Objekt > _obj );
	virtual void Collision_Enter ( std::shared_ptr < Objekt > _obj );

	virtual void Collision_Trigger ( std::shared_ptr < Objekt > _obj );
	virtual void Collision_Trigger_Exit ( std::shared_ptr < Objekt > _obj );
	virtual void Collision_Trigger_Enter ( std::shared_ptr < Objekt > _obj );

	void Set_Active ( bool active );
	bool Get_Active ( );

	virtual void Delete ( );

	// used for calling the specific Set functions from the interpreter
	// when calling normaly override the Set function and specify your arguments
	// when call the arguments will bee ['arg1','<value>','arg2',...]
	virtual std::shared_ptr < Behaviour > Set ( const std::vector < std::string > &Args = {} );

	struct Component_Metadata {
		const char* name = "Behaviour";
		float version = 0;
		const char* description = "This is an empty Component";

		Component_Metadata ( );
		Component_Metadata ( const char* _name, float _version, const char* _description );
	};

	Component_Metadata Informations;
	Component_Metadata Get_Info ( );
};
