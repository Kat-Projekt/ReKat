#include "extensions/phisiks/phisiks.hpp"
#include <objekt/manager.hpp>

bool operator== ( const collision_check&_lt, const collision_check&_rt) {
    return ( _rt.collider1 == _lt.collider1 && _rt.collider2 == _lt.collider2 ) ||
           ( _rt.collider1 == _lt.collider2 && _rt.collider2 == _lt.collider1 );
}

std::ostream& operator << ( std::ostream& os, 
    std::unordered_map < collision_check, Collision_Result, collision_hash > m ) {
    for ( auto e : m ) {
        os << e.first << " " << e.second << '\n';
    }
    return os;
}

namespace ReKat {
namespace phisiks {
    float _last_phisik_update;
    float _phisik_update_ratio;
    int _phisik_fps;
    List < Collider* > Colliders;
    List < Rigidbody* > Rigidbodys;
    std::string Active;
    // trace old collision for specific interactions like exit and enter
    std::unordered_map < collision_check, Collision_Result, collision_hash > Collision_History;

    int Start ( int phisik_fps ) {
        _phisik_fps = phisik_fps; 
        _phisik_update_ratio = 1 / _phisik_fps;

        Timer::Update ( );
        _last_phisik_update = Timer::Get_Time ( ) - _phisik_update_ratio;
        return 0;
    }

    void Update ( ) {
        DEBUG ( 4, "Updating Phisiks" );
        Timer::Update ( );

        DEBUG ( 5, "Updating Fixed Updates" );
        DEBUG ( 6, _last_phisik_update );

        if ( _phisik_fps > 0 ) { 
        if ( _last_phisik_update + _phisik_update_ratio <= Timer::Get_Time ( ) ) {
            _last_phisik_update = Timer::Get_Time ( );
            Timer::Fixed_Update ( );
            Manager::Objekt_Get ( Active )->Fixed_Update ( );
            DEBUG ( 3, "Running Fixed Update" );
        } else { return; } }

        // this code is only run durung Fixed_Updates;

        // check if every collider is active
        DEBUG ( 5, "Getting Active Colliders" );
        List < Collider *> active_colliders;
        for ( auto C : Colliders ) {
            if ( C->Get_Active ( ) && C->obj->Get_Active ( ) ) {
            if ( Manager::Objekt_Get ( Active )->Has_Child ( C->obj->Get_Name ( ) ) )
            { active_colliders.append ( C ); } }
        }

        DEBUG ( 4, " Colliders to check: ", active_colliders );

        DEBUG ( 5, "Inizializing Spacial Map" );

        Brute_Force map;
        // Hash_Map map ( 200 );
        map.Set_Colliders ( active_colliders );
        auto checks = map.Get_Collisions_To_Check ( );

        DEBUG ( 4, "checks: ", checks );

        for ( auto C : checks ) {
            // convert colliders
            Collision_Result result;

            DEBUG (4, "Checking collision between: ", C );

            switch ( C.collider1->Collider_Type ( ) ) {
                case 1: // box collider
                    switch ( C.collider2->Collider_Type ( ) ) {
                        case 1: // box collider
                        result = Check_Collision ( ( Box_Collider* ) C.collider1, ( Box_Collider* ) C.collider2 );
                        break;
                        case 2: // sfere collider
                        result = Check_Collision ( ( Box_Collider* ) C.collider1, ( Sfere_Collider* ) C.collider2 );
                        break;
                        case 3: // tilemap collider
                        result = Check_Collision ( ( Tilemap_Collider* ) C.collider2, ( Box_Collider* ) C.collider1 );
                        break;
                    }
                break;
                case 2: // sfere collider
                    switch ( C.collider2->Collider_Type ( ) ) {
                        case 1: // box collider
                        result = Check_Collision ( ( Box_Collider* ) C.collider2, ( Sfere_Collider* ) C.collider1 );
                        break;
                        case 2: // sfere collider
                        result = Check_Collision ( ( Sfere_Collider* ) C.collider1, ( Sfere_Collider* ) C.collider2 );
                        break;
                        case 3: // tilemap collider
                        result = Check_Collision ( ( Tilemap_Collider* ) C.collider1, ( Sfere_Collider* ) C.collider2 );
                        break;
                    }
                break;
                case 3: // tilemap collider
                    switch ( C.collider2->Collider_Type ( ) ) {
                        case 1: // box collider
                        result = Check_Collision ( ( Tilemap_Collider* ) C.collider1, ( Box_Collider* ) C.collider2 );
                        break;
                        case 2: // sfere collider
                        result = Check_Collision ( ( Tilemap_Collider* ) C.collider1, ( Sfere_Collider* ) C.collider2 );
                        break;
                        case 3: // tilemap collider
                        result = Check_Collision ( ( Tilemap_Collider* ) C.collider1, ( Tilemap_Collider* ) C.collider2 );
                        break;
                    }
                break;
            }

            DEBUG ( 5, "Collison Result: ", result );
            // std::cout << Collision_History;

            // Collision type == Enter
            if ( result.triggered ) {
                // check if of type enter
                if ( auto C_poisiton = Collision_History.find ( C );
                    C_poisiton == Collision_History.end ( ) || // new pointer
                    (*C_poisiton).second.triggered == false // old collision was not triggered
            ) {
            // collision category
            if ( C.collider1->Is_Trigger( ) || C.collider2->Is_Trigger( ) ) {
                // trigger
                C.collider1->obj->Handle_Collisions ( C.collider2->obj, true, collision_type::Enter );
                C.collider2->obj->Handle_Collisions ( C.collider1->obj, true, collision_type::Enter );
            } else {
                // not trigger
                C.collider1->obj->Handle_Collisions ( C.collider2->obj, false, collision_type::Enter );
                C.collider2->obj->Handle_Collisions ( C.collider1->obj, false, collision_type::Enter );
            } } }

            // inside collision => trigger stay collision always
            if ( result.triggered ) {
                if ( ! C.collider1->Is_Trigger( ) && ! C.collider2->Is_Trigger( ) ) { // reaction
                    if ( ! C.collider1->Is_Static ( ) ) { // first dinamic
                        if ( ! C.collider2->Is_Static ( ) ) { // both dinamic
                            float M1 = C.collider1->obj->template Get_Component < Rigidbody > ()->mass;
                            float M2 = C.collider2->obj->template Get_Component < Rigidbody > ()->mass;
                            float M = M1 + M2;
                            M1 = M1 / M;
                            M2 = M2 / M;

                            C.collider1->obj->Inc_Pos ( result.exit_direction * ( 1 - M1 ) );
                            C.collider2->obj->Inc_Pos ( - result.exit_direction * ( 1 - M2 ) );

                            // vincolar reaction
                            if ( result.exit_direction == vec3{0,0,0} ) { goto prereturn; }
                            vec3 normalize_exit = normalize(result.exit_direction);

                            C.collider1->obj->template Get_Component < Rigidbody >()->Vincolar_Reaction( normalize_exit );
                            C.collider2->obj->template Get_Component < Rigidbody >()->Vincolar_Reaction( -normalize_exit );
                            goto prereturn;
                        }
                        C.collider1->obj->Inc_Pos ( result.exit_direction );
                        if ( result.exit_direction == vec3{0,0,0} ) { goto prereturn; }
                        vec3 normalize_exit = normalize(result.exit_direction);
                        C.collider1->obj->template Get_Component < Rigidbody >()->Vincolar_Reaction(normalize_exit);
                    }
                    if ( ! C.collider2->Is_Static ( ) ){ // second dinamic 
                        C.collider2->obj->Inc_Pos ( -result.exit_direction );
                        if ( result.exit_direction == vec3{0,0,0} ) { goto prereturn; }
                        vec3 normalize_exit = normalize(result.exit_direction);
                        C.collider2->obj->template Get_Component < Rigidbody >()->Vincolar_Reaction(-normalize_exit);
                    }

                    C.collider1->obj->Handle_Collisions ( C.collider2->obj, false );
                    C.collider2->obj->Handle_Collisions ( C.collider1->obj, false );
                } else {
                    C.collider1->obj->Handle_Collisions ( C.collider2->obj, true );
                    C.collider2->obj->Handle_Collisions ( C.collider1->obj, true );
                }
            }

            // Collision type == Exit
            if ( result.triggered == false ) {
                // check if of type exit
                if ( auto C_poisiton = Collision_History.find ( C );
                    ! ( C_poisiton == Collision_History.end ( ) ) && // not a new collision
                    (*C_poisiton).second.triggered == true // old collision was triggered
            ) {
            // collision category
            if ( C.collider1->Is_Trigger( ) || C.collider2->Is_Trigger( ) ) {
                // trigger
                C.collider1->obj->Handle_Collisions ( C.collider2->obj, true, collision_type::Exit );
                C.collider2->obj->Handle_Collisions ( C.collider1->obj, true, collision_type::Exit );
            } else {
                // not trigger
                C.collider1->obj->Handle_Collisions ( C.collider2->obj, false, collision_type::Exit );
                C.collider2->obj->Handle_Collisions ( C.collider1->obj, false, collision_type::Exit );
            } } }

            // add collision to pool
        prereturn: // sorry for goto crime
            // std::cout << "R: " << result << '\n';
            Collision_History[C] = result;
        }

        // rigidbodies are rendered by the objekt
        DEBUG ( 3, "Fixed Debug Ended" );
    }

    void Set_Active ( Objekt& new_Active ) 
    { Active = new_Active.Get_Name ( ); }
    void Set_Active (std::string new_Active ) 
    { Active = new_Active; }
} // namespace phisiks
} // namespace ReKat 

void Collider::_Start ( ) { ReKat::phisiks::Add_Collider ( this ); }
void Collider::Delete ( ) { ReKat::phisiks::Rem_Collider ( this ); }