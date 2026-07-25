#ifndef ANIMATOR
#define ANIMATOR

/* this is an helpfull component that can drive an objekts attributes as well as attributes from other objekts */
#include <objekt/objekt.hpp>
#include "../resources/animation.h"
#include <utilities/map.h>

#include <extensions/phisiks/timer.hpp>

class Animator : public Behaviour {
	double Metronome = 0; // reseted every animation change

	struct Node {
		List < std::shared_ptr < Resource > > animations;

		template < typename A >
		void Add_Animation ( std::shared_ptr < Animation < A > > anima ) 
		{
			animations.append ( static_cast< std::shared_ptr < Resource > > ( anima ) );
		}

		void Interpolate ( float _time ) {
			for ( auto A : animations ) 
			{ DEBUG ( 4, "Animating ", A ); A->Use ( _time ); }
		}
	};
	
	std::shared_ptr < Node > Active_Node;
	Map < std::string, std::shared_ptr < Node > > nodes;
public:
	Animator * Change_Animation ( std::string name ) {
		Active_Node = nodes.get_single ( name );
		Metronome = Timer::Get_Time_d ( );
		return this;
	}

	Animator * New_Node ( std::string name ) 
	{
		nodes.append ( { name, std::make_shared < Node > ( ) } );
		return this;
	}

	template < typename A >
	Animator * Add_Animation (
		std::string node,
		std::shared_ptr < Animation < A > > anim
	) {
		auto n = nodes.get_single ( node );

		if ( n == nullptr )
		{
			DEBUG (2, "node not found" );
			return this; 
		}
		n->Add_Animation ( anim );
		return this;
	}

	void Start ( ) 
	{
		nodes = Map < std::string, std::shared_ptr < Node > > ( );
	}

	void Update ( ) {
		if ( Active_Node == nullptr )
		{
			DEBUG ( 2, "ANIMATING NOTTING" );
			return;
		}
		Active_Node->Interpolate ( Timer::Get_Time_d ( ) - Metronome );
		DEBUG ( 3, "interpolating ", Timer::Get_Time_d ( ) - Metronome );
	}
};

#endif