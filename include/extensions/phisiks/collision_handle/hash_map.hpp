#pragma once
#include "collision_narrower.hpp"

class Hash_Map : public Collision_Narrower {
private:
    float _spacing;
	Map < int, Collider* > indexed_colliders;
    int max = 0;

public:
    // uses and arkimedes spiral for mapping used for 3d spaces instad of 3d
    int Hash ( vec3 norm ) {
        // parte centrale
        int coordinata_max = ( abs ( norm.x ) > abs ( norm.y ) ) ? ( ( abs ( norm.z ) > abs ( norm.x ) ) ? abs ( norm.z ) : abs ( norm.x ) ) : ( ( abs ( norm.z ) > abs ( norm.y ) ) ? abs ( norm.z ) : abs ( norm.y ) );
        int index = pow ( 2 * coordinata_max - 1, 2 ); // 2 -> 3 in tree dimensions
		
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
        os << "size: " << n.number << " ";
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

        std::vector < vec3 > neibours_relative_pos = { { 1, 1,0 }, { 0, 1,0 }, { -1, 1,0 },
                                                       { 1, 0,0 }, { 0, 0,0 }, { -1, 0,0 },
                                                       { 1,-1,0 }, { 0,-1,0 }, { -1,-1,0 }};

        Neibours N_indexes;
        N_indexes.hash = (int*) calloc ( 9, sizeof (int) );
        int count = 0;
        for ( auto n_pos : neibours_relative_pos ) {
            // iterate the neibours positions
            auto new_pos = norm + n_pos;
            // check ring
            if ( new_pos == vec3 ( {0,0,0} ) ) 
            { N_indexes.hash[count] = 0; count ++; continue; }

            int index = Hash ( new_pos );
            DEBUG ( 4, index, " ", max );
            if ( index < max ) {
                N_indexes.hash[count] = index;
                count++;
            }
        }
        N_indexes.number = count;
        return N_indexes;
	}

    // creates a spacial hashmap
    Hash_Map ( float spacing ) : _spacing ( 1 / spacing ) { }

    void Set_Colliders ( List < Collider* > &colliders ) {
        // itereate and add to hash map        
        for ( auto C : colliders ) {
            auto Hash = Normalize_and_Hash ( C->obj->Get_Pos ( ) );
            if ( Hash > max ) { max = Hash; } // get max hash
            indexed_colliders.append ( { Hash, C } );
		}

        DEBUG ( 5, "Normalized and Hashed with max: ", max );
    }
	
	List < collision_check > Get_Collisions_To_Check ( ) {
		List < collision_check > checks;
		DEBUG ( 5, "Getting Collisions Checks" );
		for ( auto C : indexed_colliders ) {
			// get neibours
            auto Neibours_indexes = Get_Neibours ( C.data->obj->Get_Pos ( ) );
            auto This_collider = C.data;

            DEBUG ( 5, Neibours_indexes );

            auto Neiboursing_colliders = List < Collider * > ( );
            // Add colliders
            Neiboursing_colliders.append ( indexed_colliders.get ( Neibours_indexes.hash, Neibours_indexes.number ) );
            // remove same collider
            // Neiboursing_colliders.remove ( C );

            // adding checks
            for ( auto Coll : Neiboursing_colliders ) {
                collision_check check;
                if ( intptr_t( Coll ) > intptr_t( This_collider ) ) {
                    check.collider1 = This_collider;
                    check.collider2 = Coll;
                } else if ( intptr_t( Coll ) == intptr_t(This_collider) ) {
                    DEBUG ( 6, "Same Collider" );
                    continue;
                } else {
                    check.collider2 = This_collider;
                    check.collider1 = Coll;
                }

                // check for duplicates:
                bool dublicate = false;
                for ( auto test : checks ) {
                    if ( check.collider1 == test.collider1 && 
                         check.collider2 == test.collider2 ) 
                    { dublicate = true; DEBUG ( 6, "found dublicate collison: ", check.collider1 ); }
                }
                
                if ( !dublicate ) 
                { checks.append ( check ); }
            }
		}
        DEBUG ( 4, "Retruring checks" );
		return checks;
	}

    void TEST_set_max ( int _max ) { max = _max; }

    ~Hash_Map ( ) { }
};
