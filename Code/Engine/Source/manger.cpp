#include "objekt/manager.hpp"

namespace Manager
{
	List < std::shared_ptr < Objekt > > objekts;
	std::shared_ptr < Objekt > _current_scene;

	extern void Start ( )
	{
		if ( _current_scene != nullptr )
		{ _current_scene->Start ( ); }
	}

	extern void Update ( )
	{
		if ( _current_scene != nullptr )
		{ _current_scene->Update ( ); }
	}

	extern std::shared_ptr < Objekt > Objekt_Get ( std::string name )
	{
		for ( auto S : objekts )
		{
			if ( S->Get_Name ( ) == name )
			{ return S; }
		}

		DEBUG ( 1, "Cannot find objekt ", name );
		return nullptr;
	}

	extern std::shared_ptr < Objekt > Objekt_Load
	(
		std::string name,
		vec3 pos,
		vec3 size,
		vec3 rot_pivot
	)
	{
		auto o = std::make_shared < Objekt > ( name, pos, size, rot_pivot );
		objekts.append ( o );
		return o;
	}

	extern std::shared_ptr < Objekt > Objekt_Load ( std::shared_ptr < Objekt > o )
	{
		objekts.append ( o );
		return o;
	}

	extern void Set_Active_Scene ( std::shared_ptr < Objekt > o )
	{
		DEBUG ( 3,"adding active scene" );

		objekts.append ( o );
		_current_scene = o;

		DEBUG ( 5,"added main scene, Starding main scene" );

		Start ( );
	}

	extern void Set_Active_Scene ( std::string s )
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

	extern std::shared_ptr < Objekt > Get_Active_Scene ( )
	{
		return _current_scene;
	}

	extern void Check_Resource_Integrity ( )
	{
		DEBUG ( 3, "STARTING INTEGRITY CHECK" );

		DEBUG ( 4, "Current Scene: '", _current_scene->Get_Name ( ) );

		for ( auto C : objekts )
		{ DEBUG ( 4,*C ); }

		DEBUG ( 3, "ENDED INTEGRITY CHECK" );
	}

	extern void Free_Objekt ( std::string name )
	{
		auto D = Objekt_Get ( name );

		objekts.remove ( D );

		if ( D == _current_scene )
		{ _current_scene = nullptr; }

		DEBUG ( 4, "derefencing objekt remaining copies: ", D.use_count ( ) - 1 );
		D.reset ( );
	}
}