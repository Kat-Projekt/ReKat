#include "extensions/phisiks/phisiks.hpp"

namespace ReKat {
namespace phisiks {
	float _last_phisik_update;
	float _phisik_update_ratio;
	int _phisik_fps;
	// gets colliders every Fixed_Update
	std::string _active = "";
	// trace old collision for specific interactions like exit and enter
	std::unordered_map < collision_check, collision_result, collision_hash > _collision_history;

	extern void Start ( int phisik_fps ) {
		_phisik_fps = phisik_fps; 
		_phisik_update_ratio = 1 / _phisik_fps;

		Timer::Update ( );
		_last_phisik_update = Timer::Get_Time ( ) - _phisik_update_ratio;
	}

	extern void Fixed_Update ( )
	{
		auto Active = Manager::Objekt_Get ( _active );

		if ( !Active )
		{ return; }

		_last_phisik_update = Timer::Get_Time ( );
		Timer::Fixed_Update ( );
		Active->Fixed_Update ( );

		DEBUG ( 5, "Getting Active Colliders" );

		List < Collider* > colliders = Active->Get_Component_Recursive < Collider > ( );
		List < Collider* > active_colliders;

		for ( auto coll : colliders )
		{
			if ( coll->Get_Active ( ) && coll->obj->Get_Active ( ) )
			{
				active_colliders.append ( coll );
			}
		}

		DEBUG ( 4, " Colliders to check: ", active_colliders );

		Brute_Force map;
		map.Set_Colliders ( active_colliders );
		auto checks = map.Get_Collisions_To_Check ( );

		DEBUG ( 4, "checks: ", checks );


		for ( auto check : checks ) {
			// convert colliders
			collision_result result;
			
			DEBUG (4, "Checking collision between: ", check );

			switch ( check.collider1->Collider_Type ( ) )
			{
			case COLLIDER_TYPE::BOX:
				switch ( check.collider2->Collider_Type ( ) )
				{
				case COLLIDER_TYPE::BOX:
					result = Check_Collision ( ( Box_Collider* ) check.collider1, ( Box_Collider* ) check.collider2 );
					DEBUG ( 4, "Collision BOX - BOX" );
				break;

				case COLLIDER_TYPE::SPHERE:
					result = Check_Collision ( ( Box_Collider* ) check.collider1, ( Sphere_Collider* ) check.collider2 );
					DEBUG ( 4, "Collision BOX - SPHERE" );
				break;

				case COLLIDER_TYPE::TILEMAP:
					result = Check_Collision ( ( Tilemap_Collider* ) check.collider2, ( Box_Collider* ) check.collider1 );
					result.triggered *= -1.0f;
					DEBUG ( 4, "Collision TILEMAP - BOX" );
				break;

				default: break;
				}
			break;


			case COLLIDER_TYPE::SPHERE:
				switch ( check.collider2->Collider_Type ( ) )
				{
				case COLLIDER_TYPE::BOX:
					result = Check_Collision ( ( Box_Collider* ) check.collider2, ( Sphere_Collider* ) check.collider1 );
					result.triggered *= -1.0f;
					DEBUG ( 4, "Collision BOX - SPHERE" );
				break;

				case COLLIDER_TYPE::SPHERE:
					result = Check_Collision ( ( Sphere_Collider* ) check.collider1, ( Sphere_Collider* ) check.collider2 );
					DEBUG ( 4, "Collision SPHERE - SPHERE" );
				break;

				case COLLIDER_TYPE::TILEMAP:
					result = Check_Collision ( ( Tilemap_Collider* ) check.collider1, ( Sphere_Collider* ) check.collider2 );
					result.triggered *= -1.0f;
					DEBUG ( 4, "Collision TILEMAP - SPHERE" );
				break;

				default: break;
				}
			break;


			case COLLIDER_TYPE::TILEMAP:
				switch ( check.collider2->Collider_Type ( ) )
				{
				case COLLIDER_TYPE::BOX:
					result = Check_Collision ( ( Tilemap_Collider* ) check.collider1, ( Box_Collider* ) check.collider2 );
					DEBUG ( 4, "Collision TILEMAP - BOX" );
				break;

				case COLLIDER_TYPE::SPHERE:
					result = Check_Collision ( ( Tilemap_Collider* ) check.collider1, ( Sphere_Collider* ) check.collider2 );
					DEBUG ( 4, "Collision TILEMAP - SPHERE" );
				break;

				case COLLIDER_TYPE::TILEMAP:
					result = Check_Collision ( ( Tilemap_Collider* ) check.collider1, ( Tilemap_Collider* ) check.collider2 );
					DEBUG ( 4, "Collision TILEMAP - TILEMAP" );
				break;

				default: break;
				}
			break;
			}

			DEBUG ( 5, "Collison Result: ", result );

			collision_type type_of_collison = collision_type::Stay;

			auto old_check = _collision_history.find ( check );

			// new event
			if ( old_check == _collision_history.end ( ) )
			{
				if ( result.triggered ) {
					type_of_collison = collision_type::Enter;
				} else {
					// no collision appened
					_collision_history[check] = result;
					continue;
				}
			}
			else // old event
			{
				// both true
				if ( old_check->second.triggered && result.triggered )
				{
					type_of_collison = collision_type::Stay;
				}
				
				// old true new false
				if ( old_check->second.triggered && !result.triggered )
				{
					type_of_collison = collision_type::Exit;
				}

				// old false new true
				if ( !old_check->second.triggered && result.triggered )
				{
					type_of_collison = collision_type::Enter;
				}

				// old false new false -> no collision apened recently
				if ( !old_check->second.triggered && !result.triggered )
				{
					_collision_history[check] = result;
					continue;
				}
			}

			// check is is trigger
			bool is_a_trigger_collsion = false;
			if ( check.collider1->Is_Trigger ( ) || check.collider2->Is_Trigger ( ) )
			{
				is_a_trigger_collsion = true;
			}

			switch ( type_of_collison )
			{
				case collision_type::Enter:
				DEBUG ( 5, "Entering collision, ", 
					( is_a_trigger_collsion ? "[triggered]" : "[solid]"));
				break;

				case collision_type::Stay:
				DEBUG ( 5, "Staing collision, ", 
					( is_a_trigger_collsion ? "[triggered]" : "[solid]"));
				break;

				case collision_type::Exit:
				DEBUG ( 5, "Exiting collision, ", 
					( is_a_trigger_collsion ? "[triggered]" : "[solid]"));
				break;
			}

			// send collision check to objekts involved
			check.collider1->obj->Handle_Collisions ( check.collider2->obj, is_a_trigger_collsion, type_of_collison );
			check.collider2->obj->Handle_Collisions ( check.collider1->obj, is_a_trigger_collsion, type_of_collison );

			// collision finished to be handled
			if ( is_a_trigger_collsion )
			{
				DEBUG ( 6, "TRIGGER COLLISION" );
				_collision_history[check] = result;
				continue;
			}

			// both static notthin appened
			if ( check.collider1->Is_Static( ) && check.collider2->Is_Static( ) )
			{
				DEBUG ( 6, "STATIC COLLISION" );
				_collision_history[check] = result;
				continue;
			}

			// no compenetration
			if ( result.exit_direction == vec3{0,0,0} )
			{
				DEBUG ( 6, "NO COLLISION" );
				_collision_history[check] = result;
				continue;
			}

			// first is dinamic
			if ( !check.collider1->Is_Static( ) && check.collider2->Is_Static( )  )
			{
				DEBUG ( 6, "FIRST DINAMIC" );
				check.collider1->obj->Inc_Pos ( result.exit_direction );
				vec3 normalized_exit = normalize ( result.exit_direction );
				check.collider1->obj->Get_Component < Rigidbody > ( )->Vincolar_Reaction ( normalized_exit );
			}

			// second is dinamic
			if ( check.collider1->Is_Static( ) && !check.collider2->Is_Static( )  )
			{
				DEBUG ( 6, "SECOND DINAMIC" );
				check.collider2->obj->Inc_Pos ( result.exit_direction );
				vec3 normalized_exit = normalize ( result.exit_direction );
				check.collider2->obj->Get_Component < Rigidbody > ( )->Vincolar_Reaction ( normalized_exit );
			}

			// both dinamic
			if ( !check.collider1->Is_Static( ) && !check.collider2->Is_Static( )  )
			{
				DEBUG ( 6, "BOTH DINAMIC" );
				float M1 = check.collider1->obj->Get_Component < Rigidbody > ( )->mass;
				float M2 = check.collider2->obj->Get_Component < Rigidbody > ( )->mass;
				float M = M1 + M2;
				M1 = M1 / M;
				M2 = M2 / M;

				check.collider1->obj->Inc_Pos ( result.exit_direction * ( 1 - M1 ) );
				check.collider2->obj->Inc_Pos ( - result.exit_direction * ( 1 - M2 ) );

				// vincolar reaction
				vec3 normalize_exit = normalize(result.exit_direction);

				check.collider1->obj->Get_Component < Rigidbody > ( )->Vincolar_Reaction( normalize_exit );
				check.collider2->obj->Get_Component < Rigidbody > ( )->Vincolar_Reaction( -normalize_exit );
			}

			_collision_history[check] = result;
		}
	}

	extern void Update ( )
	{
	        Timer::Update ( );

		if ( _active == "" )
		{ return; }

		if ( _phisik_fps > 0 )
		{ 
			if ( _last_phisik_update + _phisik_update_ratio <= Timer::Get_Time ( ) )
			{
				Fixed_Update ( );
			}
		}
	}

	extern void Set_Active
	( Objekt& new_Active ) 
	{ _active = new_Active.Get_Name ( ); }
	extern void Set_Active
	( std::string new_Active ) 
	{ _active = new_Active; }
} // namespace phisiks
} // namespace ReKat 

