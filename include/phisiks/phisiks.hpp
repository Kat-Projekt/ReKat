#ifndef PHISIKS
#define PHISIKS

#include "objekt/manager.hpp"
#include "components/box_collider.hpp"
#include "components/sphere_collider.hpp"
#include "collision_handle/brute_force.hpp"
#include "collision_handle/primitive_collision.hpp"
#include "collision_handle/tilemap_collider.h"
#include <unordered_map>

#include "timer.hpp"

#define MAX( a, b, c ) ( ( a > b ) ? ( ( a > c ) ? a : ( ( c > b ) ) ) )

namespace ReKat {
namespace phisiks {
	extern float _last_phisik_update;
	extern float _phisik_update_ratio;
	extern int _phisik_fps;
	// gets colliders every Fixed_Update
	extern std::string _active;
	// trace old collision for specific interactions like exit and enter
	extern std::unordered_map < collision_check, collision_result, collision_hash > _collision_history;

	extern void Start ( int phisik_fps );
	extern void Fixed_Update ( );
	extern void Update ( );

	extern void Set_Active ( Objekt& new_Active );
	extern void Set_Active ( std::string new_Active );
} // namespace phisiks
} // namespace ReKat 

#endif
