#ifndef PHISIKS
#define PHISIKS

#include "../objekt.hpp"
#include "components/collision.h"

#include "timer.hpp"

class Hash_Map {
private:
    float _spacing;
    std::vector < std::vector < Collider * > * > _particle_map;
	struct indexed_collider	{
		Collider* collider;
		int hash;
	};
    List < indexed_collider > indexed_colliders;
    int max = 0;

    // uses and arkimedes spiral for mapping used for 3d spaces instad of 3d
    int Normalize_and_Hash ( vec3 _point ) {
        vec3 norm = _point * _spacing;
        norm.x = floor ( norm.x );
        norm.y = floor ( norm.y );
        norm.z = floor ( norm.z );
        norm.z = 0; // ognore third dimension

        // parte centrale
        int coordinata_max = ( norm.x > norm.y ) ? ( ( norm.z > norm.x ) ? norm.z : norm.x ) : ( ( norm.z > norm.y ) ? norm.z : norm.y );
        int index = pow ( 2 * coordinata_max - 1, 2 ); // 2-> 3 in tree dimensions
		
		// find quadrant
		// if + + continue;
		// if + - add coordinata_max * 2 + 1
		// if - - add 2 * ( coordinata_max * 2 + 1 )
		// if - + add 3 * ( coordinata_max * 2 + 1 )
		
        index += ( norm.x <= norm.y ) ? ( 1 + norm.x ) : ( 1 + 2 * norm.x - norm.y ); // for 2 dimensions
        return index;
    }

	struct Neibours {
		int number;
		int* hash;
	};

	Neibours Get_Neibours ( int hash ) {
		
	}

public:
    // creates a spacial hashmap
    Hash_Map ( float spacing ) : _spacing ( 1 / spacing ) { }

    void Set_Colliders ( List < Collider* > colliders ) {
        // itereate and add to hash map        
        auto C = colliders.begin ( );

		while ( C != nullptr ) {
            auto Hash = Normalize_and_Hash ( C->data->obj->Get_Pos ( ) );
            if ( Hash > max ) { max = Hash; } // get max hash
            indexed_colliders.append ( { C->data, Hash } );
			C = C->next;
		}

		_particle_map.resize ( max );

		for ( auto C = indexed_colliders.begin ( ); C != nullptr; C = C->next ) {
			auto vect = _particle_map [ C->data.hash ];
			if ( vect == nullptr ) {
				vect = new std::vector < Collider* >;
				_particle_map [ C->data.hash ] = vect; 
			}
			vect->push_back ( C->data.collider );
		}
    }

	struct collision_check {
		Collider * collider1;
		Collider * collider2;
	};
	
	List < collision_check > Get_collisions_to_check ( ) {
		List < collision_check > checks;
		
		for ( auto C = indexed_colliders.begin ( ); C != nullptr; C = C->next ) {
			// get neibours
			// check if emply
			// if not add { C1.collider, C2.collider } to checks to make
			// ordered so that C.collider < C2.collider 
			// but first check that { c1,c2 } are not already considered


		}

		// remove duplicate cheks

		return checks;
	}

    ~Hash_Map ( );
};



namespace ReKat {
namespace phisiks {
    static float _last_phisik_update;
    static float _phisik_update_ratio;
    static int _phisik_fps;
    static List < Collider* > Colliders;
    static List < Rigidbody* > Rigidbodys;
    static void Resolve_Collision ( );

    template < class C1, class C2 >
    static void Check_Collision ( C1 collider1, C2 collider2 );

    static void Check_Collisions ( );

    static void Start ( int phisik_fps ) { 
        _phisik_fps = phisik_fps; 
        _phisik_update_ratio = 1 / _phisik_fps;

        Timer::Update ( );
        _last_phisik_update = Timer::Get_Time ( ) - _phisik_update_ratio;
    }

    static void Update ( ) {
        DEBUG ( 4, "Updating Phisiks" );
        // get active scene;
        auto Active = Manager::Get_Active_Scene ( );

        DEBUG ( 5, "Updating Fixed Updates" );
        Timer::Update ( );
        if ( _phisik_fps > 0 ) { 
        if ( _last_phisik_update + _phisik_update_ratio > Timer::Get_Time ( ) ) {
            _last_phisik_update = Timer::Get_Time ( );
            Active->Fixed_Update ( );
        } else { return; } }

        // this code is only run durung Fixed_Updates;

        // check if every collider is active
        DEBUG ( 5, "Getting Active Colliders" );
        List < Collider > active_colliders;
        auto C = Colliders.begin ( );
		while ( C != nullptr ) {
            if ( Active->Has_Children ( C->data->obj ) )
            { active_colliders.append ( *C->data ); }
			C = C->next;
		}

        DEBUG ( 5, "Inizializing Spacial Map" );


        // rigidbodies are rendered by the objekt
    }

    template < class C >
    static void Add_Collider ( C collider ) {
        DEBUG ( 4,"Adding Colider: ", std::string(typeid(*collider).name()), " from ", collider.obj.Get_Name () );
		if ( std::is_base_of<Collider, C>::value ) {
			Colliders.append ( ( Collider * ) ( collider ) );
        	return;
		}
		DEBUG ( 2, "Wrong Collider type" );
    }
} // namespace phisiks
} // namespace ReKat 

#endif
