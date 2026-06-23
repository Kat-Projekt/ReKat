#include "objekt/manager.hpp"

namespace Manager
{
	List < Objekt* > objekts;
	Objekt* _current_scene = nullptr;

	void Start ( )
	{
		if ( _current_scene != nullptr )
		{ _current_scene->Start ( ); }
	}

	void Update ( )
	{
		if ( _current_scene != nullptr )
		{ _current_scene->Update ( ); }
	}

	Objekt* Objekt_Get ( std::string name )
	{
		for ( auto S : objekts )
		{
			if ( S->Get_Name ( ) == name )
			{ return S; }
		}

		DEBUG ( 1, "Cannot find objekt ", name );
		return nullptr;
	}

	Objekt* Objekt_Load
	(
		std::string name,
		vec3 pos,
		vec3 size,
		vec3 rot_pivot
	)
	{
		auto o = new Objekt ( name, pos, size, rot_pivot );
		objekts.append ( o );
		return o;
	}

	Objekt* Objekt_Load ( Objekt * o )
	{
		objekts.append ( o );
		return o;
	}

	void Set_Active_Scene ( Objekt * o )
	{
		DEBUG ( 3,"adding active scene" );

		objekts.append ( o );
		_current_scene = o;

		Start ( );

		DEBUG ( 5,"added main scene" );
	}

	void Set_Active_Scene ( std::string s )
	{
		DEBUG ( 3,"adding active scene" );

		for ( auto S : objekts )
		{
			if ( S->Get_Name ( ) == s )
			{
				_current_scene = S;
				Start ( );
				return;
			}
		}
	}

	Objekt* Get_Active_Scene ( )
	{
		return _current_scene;
	}

	void Check_Resource_Integrity ( )
	{
		DEBUG ( 3, "STARTING INTEGRITY CHECK" );

		DEBUG ( 4, "Current Scene: '", _current_scene->Get_Name ( ) );

		for ( auto C : objekts )
		{ DEBUG ( 4,*C ); }
	}

	void Free_Objekt ( std::string name )
	{
		auto D = Objekt_Get ( name );

		D->Delete ( );

		objekts.remove ( D );

		if ( D == _current_scene )
		{ _current_scene = nullptr; }

		delete D;
	}
}