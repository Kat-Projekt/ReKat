#pragma once

#include <boost/config.hpp>

class Objekt;

class BOOST_SYMBOL_VISIBLE Behaviour
{
protected:
	bool _active = true;
	bool _started = false;

public:
	Objekt* obj = nullptr;

	Behaviour ( );
	virtual ~Behaviour ( );

	void _Start ( );
	void _Update ( );
	void _Fixed_Update ( );

	virtual void Start ( );
	virtual void Update ( );
	virtual void Fixed_Update ( );

	virtual void Collision ( Objekt* _obj );
	virtual void Collision_Exit ( Objekt* _obj );
	virtual void Collision_Enter ( Objekt* _obj );

	virtual void Collision_Trigger ( Objekt* _obj );
	virtual void Collision_Trigger_Exit ( Objekt* _obj );
	virtual void Collision_Trigger_Enter ( Objekt* _obj );

	void Set_Active ( bool active );
	bool Get_Active ( );

	virtual void Delete ( );

	virtual Behaviour* Set ( ... );
};