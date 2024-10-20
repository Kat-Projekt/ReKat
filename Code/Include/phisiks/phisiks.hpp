#ifndef PHISIKS
#define PHISIKS

#include "../objekt.hpp"
#include "components/collision.h"

#include "timer.hpp"

#define MAX( a, b, c ) ( ( a > b ) ? ( ( a > c ) ? a : ( ( c > b ) ) ) )

class Hash_Map {
private:
    float _spacing;
    std::vector < List < Collider * > * > _particle_map;
	struct indexed_collider	{
		Collider* collider;
		int hash;
	};
    List < indexed_collider > indexed_colliders;
    int max = 0;

public:
    // uses and arkimedes spiral for mapping used for 3d spaces instad of 3d
    int Hash ( vec3 norm ) {
        // parte centrale
        int coordinata_max = ( abs ( norm.x ) > abs ( norm.y ) ) ? ( ( abs ( norm.z ) > abs ( norm.x ) ) ? abs ( norm.z ) : abs ( norm.x ) ) : ( ( abs ( norm.z ) > abs ( norm.y ) ) ? abs ( norm.z ) : abs ( norm.y ) );
        int index = pow ( 2 * coordinata_max - 1, 2 ); // 2-> 3 in tree dimensions
		
		// find quadrant
		
        if ( norm.x >= 0 ) {
            if ( norm.y >= 0 ) { // if + + 
                index += ( norm.x < norm.y ) ? ( norm.x ) : ( 2 * norm.x - norm.y ); 
            } 
            else { // if + -
                index += coordinata_max * 2;
                index += ( norm.x > - norm.y ) ? ( - norm.y ) : ( 2 * ( - norm.y ) - norm.x );
            }
        } else {
            if ( norm.y < 0 ) { // if - -
                index += coordinata_max * 4;
                index += ( - norm.x <= - norm.y ) ? ( - norm.x ) : ( - 2 * norm.x + norm.y );
            }  else { // if - +
                index += coordinata_max * 6;
                index += ( norm.y <= - norm.x ) ? ( norm.y ) : ( 2 * norm.y + norm.x ); 
            }
        }

        return index;
    }
    int Normalize_and_Hash ( vec3 _point ) {
        vec3 norm = _point * _spacing;
        norm.x = floor ( norm.x );
        norm.y = floor ( norm.y );
        norm.z = floor ( norm.z );
        norm.z = 0; // ognore third dimension

        if ( norm == vec3 ( {0,0,0} ) ) { return 0; }

        // parte centrale
        int coordinata_max = ( abs ( norm.x ) > abs ( norm.y ) ) ? ( ( abs ( norm.z ) > abs ( norm.x ) ) ? abs ( norm.z ) : abs ( norm.x ) ) : ( ( abs ( norm.z ) > abs ( norm.y ) ) ? abs ( norm.z ) : abs ( norm.y ) );
        int index = pow ( 2 * coordinata_max - 1, 2 ); // 2-> 3 in tree dimensions
		
		// find quadrant
		
        if ( norm.x >= 0 ) {
            if ( norm.y >= 0 ) { // if + + 
                index += ( norm.x < norm.y ) ? ( norm.x ) : ( 2 * norm.x - norm.y ); 
            } 
            else { // if + -
                index += coordinata_max * 2;
                index += ( norm.x > - norm.y ) ? ( - norm.y ) : ( 2 * ( - norm.y ) - norm.x );
            }
        } else {
            if ( norm.y < 0 ) { // if - -
                index += coordinata_max * 4;
                index += ( - norm.x <= - norm.y ) ? ( - norm.x ) : ( - 2 * norm.x + norm.y );
            }  else { // if - +
                index += coordinata_max * 6;
                index += ( norm.y <= - norm.x ) ? ( norm.y ) : ( 2 * norm.y + norm.x ); 
            }
        }

        return index;
    }

	struct Neibours {
		int number;
		int* hash;
	};

	friend std::ostream& operator << ( std::ostream& os, Neibours& n ) {
        if ( n.number <= 0 ) { os << "none"; return os; }
        os << n.hash[0];
		for ( size_t i = 1; i < n.number; i++ ) {
            os << ", " << n.hash [ i ];
        }
		return os;
	}
	friend std::ostream& operator,(std::ostream& out, Neibours& n )
	{ out << n; return out; }

	Neibours Get_Neibours ( vec3 pos ) {
		vec3 norm = pos * _spacing;
        norm.x = floor ( norm.x );
        norm.y = floor ( norm.y );
        norm.z = floor ( norm.z );
        norm.z = 0; // ognore third dimension

        std::vector < vec3 > neibours_relative_pos = { { 0, 1,0 }, {  1, 1,0 }, {  1,0,0 }, { 1,-1,0 }, 
                                                       { 0,-1,0 }, { -1,-1,0 }, { -1,0,0 }, { -1,1,0 } };

        Neibours N_indexes;
        N_indexes.hash = (int*) calloc ( 8, sizeof (int) );
        int count = 0;
        for ( auto n_pos : neibours_relative_pos ) {
            // iterate the neibours positions
            auto new_pos = norm + n_pos;
            // check ring
            if ( new_pos == vec3 ( {0,0,0} ) ) 
            { N_indexes.hash[count] = 0; count ++; continue; }

            int index = Hash ( new_pos );
            if ( index > max ) { continue; }

            N_indexes.hash[count] = index;
            count++;
        }
        N_indexes.number = count;
        return N_indexes;
	}

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

        DEBUG ( 5, "Normalized and Hashed with max: ", max );

        max; // prevent over flow
		_particle_map.resize ( max + 1 );

		for ( auto C = indexed_colliders.begin ( ); C != nullptr; C = C->next ) {
            DEBUG ( 5, "indexing ", C->data.hash );
			auto vect = _particle_map [ C->data.hash ];
			if ( vect == nullptr ) {
                DEBUG ( 6, "inizialing" );
				vect = new List < Collider* >;
				_particle_map [ C->data.hash ] = vect; 
                DEBUG ( 6, "Inizialized" );
			}
			vect->append ( C->data.collider );
		}

        DEBUG ( 5, "Particle map Populated" );
    }

	struct collision_check {
		Collider * collider1;
		Collider * collider2;
	};

    friend std::ostream& operator << ( std::ostream& os, collision_check& n ) {
        os << '{';
        os << n.collider1;
        os << ", ";
        os << n.collider2;
        os << '}';
		return os;
	}
	friend std::ostream& operator,(std::ostream& out, collision_check& n )
	{ out << n; return out; }
	
	List < collision_check > Get_collisions_to_check ( ) {
		List < collision_check > checks;
		DEBUG ( 5, "Getting Collisions Checks" );
		for ( auto C = indexed_colliders.begin ( ); C != nullptr; C = C->next ) {
			// get neibours
            auto Neibours_indexes = Get_Neibours ( C->data.collider->obj->Get_Pos ( ) );
            auto This_collider = C->data.collider;

            DEBUG ( 5, Neibours_indexes );

            List < Collider * > Neiboursing_colliders;
            // Add same cell colliders
            for ( auto Coll = _particle_map[C->data.hash]->begin ( ); Coll != nullptr; Coll = Coll->next ) {
                if ( int( Coll->data ) != int( C->data.collider ) ) 
                { Neiboursing_colliders.append ( Coll->data ); DEBUG ( 6, "added: ",  Coll->data, " with ", C->data.collider ); }
            }
            
            // Add other cells colliders
            for ( size_t i = 0; i < Neibours_indexes.number; i++ ) 
            { Neiboursing_colliders.append ( _particle_map [ Neibours_indexes.hash[i] ] ); }

            // adding checks
            for ( auto Coll = Neiboursing_colliders.begin ( ); Coll != nullptr; Coll = Coll->next ) {
                collision_check check;
                if ( int( Coll->data ) > int(This_collider) ) {
                    check.collider1 = This_collider;
                    check.collider2 = Coll->data;
                } else {
                    check.collider2 = This_collider;
                    check.collider1 = Coll->data;
                }

                // check for duplicates:
                bool dublicate = false;
                for ( auto test = checks.begin ( ); test != nullptr; test = test->next ) {
                    if ( check.collider1 == test->data.collider1 && 
                         check.collider2 == test->data.collider2 ) 
                    { dublicate = true; }
                }
                
                if ( !dublicate ) 
                { checks.append ( check ); }
            }            
		}

		return checks;
	}

    void TEST_set_max ( int _max ) { max = _max; }

    ~Hash_Map ( ) { }
};



namespace ReKat {
namespace phisiks {
    static float _last_phisik_update;
    static float _phisik_update_ratio;
    static int _phisik_fps;
    static List < Collider* > Colliders;
    static List < Rigidbody* > Rigidbodys;
    static void Resolve_Collision ( );

    static void Start ( int phisik_fps ) { 
        _phisik_fps = phisik_fps; 
        _phisik_update_ratio = 1 / _phisik_fps;

        Timer::Update ( );
        _last_phisik_update = Timer::Get_Time ( ) - _phisik_update_ratio;
    }

    static void Update ( ) {
        DEBUG ( 4, "Updating Phisiks" );
        Timer::Update ( );
        // get active scene;
        auto Active = Manager::Get_Active_Scene ( );

        DEBUG ( 5, "Updating Fixed Updates" );
        DEBUG ( 6, _last_phisik_update );

        Timer::Update ( );
        if ( _phisik_fps > 0 ) { 
        if ( _last_phisik_update + _phisik_update_ratio <= Timer::Get_Time ( ) ) {
            _last_phisik_update = Timer::Get_Time ( );
            Timer::Fixed_Update ( );
            Active->Fixed_Update ( );
            DEBUG ( 3, "Running Fixed Update" );
        } else { return; } }

        // this code is only run durung Fixed_Updates;

        // check if every collider is active
        DEBUG ( 5, "Getting Active Colliders" );
        List < Collider *> active_colliders;
        auto C = Colliders.begin ( );
		while ( C != nullptr ) {
            if ( Active->Has_Children ( C->data->obj ) )
            { active_colliders.append ( C->data ); }
			C = C->next;
		}

        DEBUG ( 4, " Colliders to check: ", active_colliders );

        DEBUG ( 5, "Inizializing Spacial Map" );
        Hash_Map map ( 100 );
        map.Set_Colliders ( active_colliders );

        auto checks = map.Get_collisions_to_check ( );

        DEBUG ( 4, "checks: ", checks );

        for ( auto C = checks.begin ( ); C != nullptr; C = C->next ) {
            // convert colliders
            Collision_Result result;

            DEBUG (4, "Checking collision between: ", C->data );

            switch ( C->data.collider1->Collider_Type ( ) ) {
                case 1: // box collider
                    switch ( C->data.collider2->Collider_Type ( ) ) {
                        case 1: // box collider
                        result = Check_Collision ( ( Box_Collider* ) C->data.collider1, ( Box_Collider* ) C->data.collider2 );
                        break;
                        case 2: // sfere collider
                        result = Check_Collision ( ( Box_Collider* ) C->data.collider1, ( Sfere_Collider* ) C->data.collider2 );
                        break;
                        case 3: // tilemap collider
                        result = Check_Collision ( ( Tilemap_Collider* ) C->data.collider2, ( Box_Collider* ) C->data.collider1 );
                        break;
                    }
                break;
                case 2: // sfere collider
                    switch ( C->data.collider2->Collider_Type ( ) ) {
                        case 1: // box collider
                        result = Check_Collision ( ( Box_Collider* ) C->data.collider2, ( Sfere_Collider* ) C->data.collider1 );
                        break;
                        case 2: // sfere collider
                        result = Check_Collision ( ( Sfere_Collider* ) C->data.collider1, ( Sfere_Collider* ) C->data.collider2 );
                        break;
                        case 3: // tilemap collider
                        result = Check_Collision ( ( Tilemap_Collider* ) C->data.collider1, ( Sfere_Collider* ) C->data.collider2 );
                        break;
                    }
                break;
                case 3: // tilemap collider
                    switch ( C->data.collider2->Collider_Type ( ) ) {
                        case 1: // box collider
                        result = Check_Collision ( ( Tilemap_Collider* ) C->data.collider1, ( Box_Collider* ) C->data.collider2 );
                        break;
                        case 2: // sfere collider
                        result = Check_Collision ( ( Tilemap_Collider* ) C->data.collider1, ( Sfere_Collider* ) C->data.collider2 );
                        break;
                        case 3: // tilemap collider
                        result = Check_Collision ( ( Tilemap_Collider* ) C->data.collider1, ( Tilemap_Collider* ) C->data.collider2 );
                        break;
                    }
                break;
            }

            DEBUG ( 5, "Collison Result: ", result );

            // inside collision
            if ( result.triggered ) {
                if ( ! C->data.collider1->Is_Trigger( ) && ! C->data.collider2->Is_Trigger( ) ) { // reaction
                    if ( ! C->data.collider1->Is_Static ( ) ) { // first dinamic
                        if ( ! C->data.collider2->Is_Static ( ) ) { // both dinamic
                            float M1 = C->data.collider1->obj->template Get_Component < Rigidbody > ()->mass;
                            float M2 = C->data.collider2->obj->template Get_Component < Rigidbody > ()->mass;
                            float M = M1 + M2;
                            M1 = M1 / M;
                            M2 = M2 / M;

                            C->data.collider1->obj->Inc_Pos ( result.exit_direction * ( 1 - M1 ) );
                            C->data.collider2->obj->Inc_Pos ( - result.exit_direction * ( 1 - M2 ) );

                            // vincolar reaction
                            if ( result.exit_direction == vec3{0,0,0} ) { return; }
                            vec3 normalize_exit = normalize(result.exit_direction);

                            C->data.collider1->obj->template Get_Component < Rigidbody >()->Vincolar_Reaction( normalize_exit );
                            C->data.collider2->obj->template Get_Component < Rigidbody >()->Vincolar_Reaction( -normalize_exit );
                            return;
                        }
                        C->data.collider1->obj->Inc_Pos ( result.exit_direction );
                        if ( result.exit_direction == vec3{0,0,0} ) { return; }
                        vec3 normalize_exit = normalize(result.exit_direction);
                        C->data.collider1->obj->template Get_Component < Rigidbody >()->Vincolar_Reaction(normalize_exit);
                    }
                    if ( ! C->data.collider2->Is_Static ( ) ){ // second dinamic 
                        C->data.collider2->obj->Inc_Pos ( -result.exit_direction );
                        if ( result.exit_direction == vec3{0,0,0} ) { return; }
                        vec3 normalize_exit = normalize(result.exit_direction);
                        C->data.collider2->obj->template Get_Component < Rigidbody >()->Vincolar_Reaction(-normalize_exit);
                    }

                    C->data.collider1->obj->Andle_Collsions ( C->data.collider2->obj, false );
                    C->data.collider2->obj->Andle_Collsions ( C->data.collider1->obj, false );
                } else {
                    C->data.collider1->obj->Andle_Collsions ( C->data.collider2->obj, true );
                    C->data.collider2->obj->Andle_Collsions ( C->data.collider1->obj, true );
                }
            }
        }

        // rigidbodies are rendered by the objekt
    }

    template < class C >
    static void Add_Collider ( C* collider ) {
        DEBUG ( 4,"Adding Colider: ", std::string(typeid(*collider).name()), " from ", collider->obj->Get_Name () );
		if ( std::is_base_of<Collider, C>::value ) {
			Colliders.append ( ( Collider * ) ( collider ) );
        	return;
		}
		DEBUG ( 2, "Wrong Collider type" );
    }
} // namespace phisiks
} // namespace ReKat 

void Collider::_Start ( ) { ReKat::phisiks::Add_Collider ( this ); }

#endif
