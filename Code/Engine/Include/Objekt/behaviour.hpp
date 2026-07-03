#pragma once

#include <memory>
#include <vector>
#include <string>
#include <boost/config.hpp>

class Objekt;

class BOOST_SYMBOL_VISIBLE Behaviour
{
protected:
	bool _active = true;
	bool _started = false;

public:
	std::shared_ptr < Objekt > obj = nullptr;

	Behaviour ( );
	virtual ~Behaviour ( );

	void _Start ( );
	void _Update ( );
	void _Fixed_Update ( );

	virtual void Start ( );
	virtual void Update ( );
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
	// using Behaviour for semplicity
	virtual Behaviour * Set ( const std::vector < std::string > &Args = {} );
};