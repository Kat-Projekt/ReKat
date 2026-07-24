#ifndef PHISIKS
#define PHISIKS

#include <objekt/objekt.hpp>
#include "components/collision.h"
#include <unordered_map>
#include "utilities/map.h"

#include "timer.hpp"

#define MAX( a, b, c ) ( ( a > b ) ? ( ( a > c ) ? a : ( ( c > b ) ) ) )

struct collision_check {
    Collider * collider1;
    Collider * collider2;

    friend std::ostream& operator << ( std::ostream& os, const collision_check& n ) {
        os << '{';
        os << n.collider1;
        os << ", ";
        os << n.collider2;
        os << '}';
        return os;
    }
    friend std::ostream& operator,(std::ostream& out, collision_check& n )
    { out << n; return out; }

    bool operator== ( collision_check& _lf ) {
        return ( collider1 == _lf.collider1 && collider2 == _lf.collider2 ) ||
               ( collider1 == _lf.collider2 && collider2 == _lf.collider1 );
    }
    bool operator!= ( collision_check& _lf ) {
        return ! ( collider1 == _lf.collider1 && collider2 == _lf.collider2 ) && !
               ( collider1 == _lf.collider2 && collider2 == _lf.collider1 );
    }
};

// dichiarati soltanto, definiti nel .cpp
bool operator== ( const collision_check&_lt, const collision_check&_rt);

struct collision_hash {
    size_t operator ( ) ( const collision_check& c ) const {
        return std::hash<size_t>()((size_t)c.collider1) ^ (std::hash<size_t>()((size_t)c.collider2) << 1);
    }
};

std::ostream& operator << ( std::ostream& os,
    std::unordered_map < collision_check, Collision_Result, collision_hash > m );

class Collision_Narrower {
    void Set_Colliders ( List < Collider* > &colliders ) { }
    List < collision_check > Get_Collisions_To_Check ( ) {
        return List < collision_check > ( );
    }
};

class Hash_Map : public Collision_Narrower {
private:
    float _spacing;
    Map < int, Collider* > indexed_colliders;
    int max = 0;

public:
    int Hash ( vec3 norm ) { /* ... corpo originale ... */ }
    int Normalize_and_Hash ( vec3 _point ) { /* ... corpo originale ... */ }

    struct Neibours { /* ... */ };
    friend std::ostream& operator << ( std::ostream& os, Neibours& n ) { /* ... */ }
    friend std::ostream& operator,(std::ostream& out, Neibours& n ) { out << n; return out; }

    Neibours Get_Neibours ( vec3 pos ) { /* ... */ }
    Hash_Map ( float spacing ) : _spacing ( 1 / spacing ) { }
    void Set_Colliders ( List < Collider* > &colliders ) { /* ... */ }
    List < collision_check > Get_Collisions_To_Check ( ) { /* ... */ }
    void TEST_set_max ( int _max ) { max = _max; }
    ~Hash_Map ( ) { }
};

class Brute_Force : public Collision_Narrower {
    List < Collider* > _colliders;

public:
    void Set_Colliders ( List < Collider* > &colliders ) { _colliders = colliders; }
    List < collision_check > Get_Collisions_To_Check ( ) {
        List < collision_check > cheks;
        for ( auto col : _colliders ) {
            if ( col->Is_Static ( ) ) { continue; }
            for ( auto coll : _colliders ) {
                if ( coll == col ) { continue; }
                cheks.append ( {col,coll} );
            }
        }
        return cheks;
    }
};

namespace ReKat {
namespace phisiks {
    // stato: solo dichiarato con extern, definito nel .cpp
    extern float _last_phisik_update;
    extern float _phisik_update_ratio;
    extern int _phisik_fps;
    extern List < Collider* > Colliders;
    extern List < Rigidbody* > Rigidbodys;
    extern std::string Active;
    extern std::unordered_map < collision_check, Collision_Result, collision_hash > Collision_History;

    // funzioni non-template: solo dichiarate
    int Start ( int phisik_fps );
    void Update ( );
    void Set_Active ( Objekt& new_Active );
    void Set_Active ( std::string new_Active );

    // i template DEVONO restare nell'header
    template < class C >
    void Add_Collider ( C* collider ) {
        DEBUG ( 4,"Adding Colider: ", std::string(typeid(*collider).name()), " from ", collider->obj->Get_Name () );
        if ( std::is_base_of<Collider, C>::value ) {
            Colliders.append ( ( Collider * ) ( collider ) );
            return;
        }
        DEBUG ( 2, "Wrong Collider type" );
    }

    template < class C >
    void Rem_Collider ( C* collider ) {
        DEBUG ( 4,"Removing Colider: ", std::string(typeid(*collider).name()), " from ", collider->obj->Get_Name () );
        if ( std::is_base_of<Collider, C>::value ) {
            Colliders.remove ( ( Collider * ) ( collider ) );
            return;
        }
        DEBUG ( 2, "Wrong Collider type" );
    }
} // namespace phisiks
} // namespace ReKat

#endif