#ifndef ANIMATOR
#define ANIMATOR

/* this is an helpfull component that can drive an objekts attributes as well as attributes from other objekts */
#include "../../objekt.hpp"
#include "../resources/manager.hpp"

class Animator : public Behaviour {
    float Metronome = 0; // reseted every animation change

    struct Node {
        List <  Resource * > animations;

        template < typename A >
        void Add_Animation ( Animation < A > * anima ) 
        { animations.append ( static_cast< Resource * >( anima ) ); }

        void Interpolate ( float time ) {
            for ( auto A = animations.begin ( ); A != nullptr; A = A->next ) 
            { DEBUG ( 4, "Animating ", A ); A->data->Use ( time ); }
        }
    };
    
    Node *Active_Node = nullptr;
    Map < std::string, Node > nodes;
public:
    Animator * Change_Animation ( std::string name ) {
        auto New_Node = nodes.get ( name, true );
        if ( New_Node == nullptr ) 
        { DEBUG (2, "animation node not found" ); return this; }

        Active_Node = New_Node;
        Metronome = 0;
        return this;
    }

    Animator * New_Node ( std::string name ) 
    { nodes.append ( { name, Node ( ) } ); return this;}

    template < typename A >
    Animator * Add_Animation ( std::string node, Animation < A > * anim ) {
        auto n = nodes.get ( node, true );
        if ( n == nullptr )
        { DEBUG (2, "node not found" ); return this; }
        n->Add_Animation ( anim );
        return this;
    }

    Animator * Add_Animation ( std::string node, std::string anim ) {
        auto n = nodes.get ( node, true );
        if ( n == nullptr )
        { DEBUG (2, "node not found" ); return this; }
        n->Add_Animation ( Manager::Animation_Get ( anim ) );
        return this;
    }

    void Start ( ) 
    { nodes = Map < std::string, Node > ( true ); }

    void Update ( ) {
        Metronome += Timer::delta_time;
        Active_Node->Interpolate ( Metronome );
        DEBUG ( 3, "interpolating ", Metronome );
    }
};

#endif