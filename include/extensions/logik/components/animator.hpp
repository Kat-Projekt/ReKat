#ifndef ANIMATOR
#define ANIMATOR

/* this is an helpfull component that can drive an objekts attributes as well as attributes from other objekts */
#include <objekt/objekt.hpp>
#include "../resources/animation.h"
#include <utilities/map.h>

#include <extensions/phisiks/timer.hpp>

class Animator : public Behaviour {
	float Metronome = 0; // reseted every animation change

	struct Node {
		List < std::shared_ptr < Resource > > animations;

		template < typename A >
		void Add_Animation ( std::shared_ptr < Animation < A > > anima ) 
		{
			animations.append ( static_cast< std::shared_ptr < Resource > >( anima ) );
		}

		void Interpolate ( float _time ) {
			for ( auto A : animations ) 
			{ DEBUG ( 4, "Animating ", A ); A->Use ( _time ); }
		}
	};
	
	Node* Active_Node;
	Map < std::string, Node* > nodes;
public:
    Animator * Change_Animation ( std::string name ) {
        auto New_Node = nodes.get_single ( name );

        Active_Node = New_Node;
        Metronome = 0;
        return this;
    }

    Animator * New_Node ( std::string name ) 
    { nodes.append ( { name, new Node ( ) } ); return this;}

    template < typename A >
    Animator * Add_Animation ( std::string node, Animation < A > * anim ) {
        auto n = nodes.get_single ( node );
        if ( n == nullptr )
        { DEBUG (2, "node not found" ); return this; }
        n->Add_Animation ( anim );
        return this;
    }

    Animator * Add_Animation ( std::string node, std::string anim ) {
        auto n = nodes.get_single ( node );
        if ( n == nullptr )
        { DEBUG (2, "node not found" ); return this; }
        n->Add_Animation ( Manager::Get < Animation < int > > ( anim ) );
        return this;
    }

    void Start ( ) 
    { nodes = Map < std::string, Node * > ( true ); }

    void Update ( ) {
        Metronome += Timer::delta_time;
        Active_Node->Interpolate ( Metronome );
        DEBUG ( 3, "interpolating ", Metronome );
    }
};

#endif