#ifndef ANIMATOR
#define ANIMATOR

/* this is an helpfull component that can drive an objekts attributes as well as attributes from other objekts */
#include <objekt/objekt.hpp>
#include "../resources/animation.h"
#include <unordered_map>

#include <extensions/phisiks/timer.hpp>

class Animator : public Behaviour {
	double Metronome = 0; // reseted every animation change

	struct Node {
		std::vector < std::shared_ptr < Resource > > animations;

		template < typename A >
		void Add_Animation ( std::shared_ptr < Animation < A > > anima ) 
		{
			animations.push_back ( static_cast< std::shared_ptr < Resource > > ( anima ) );
		}

		void Interpolate ( float _time ) {
			for ( auto A : animations ) 
			{ DEBUG ( 4, "Animating ", A ); A->Use ( _time ); }
		}
	};
	
	std::string Active_Node = "";
	std::unordered_map < std::string, std::shared_ptr < Node > > nodes;
public:
	Animator * Change_Animation ( std::string name ) {
		if ( name == Active_Node )
		{ return this; }
		Active_Node = name;
		Metronome = Timer::Get_Time_d ( );
		return this;
	}

	Animator * New_Node ( std::string name ) 
	{
		nodes[name] = std::make_shared < Node > ( );
		return this;
	}

	template < typename A >
	Animator * Add_Animation (
		std::string node,
		std::shared_ptr < Animation < A > > anim
	) {
		auto n = nodes.find ( node );

		if ( n == nodes.end ( ) )
		{
			DEBUG (2, "node not found" );
			return this; 
		}
		n->second->Add_Animation ( anim );
		return this;
	}

	void Update ( ) {
		if ( Active_Node == "" )
		{
			DEBUG ( 5, "ANIMATING NOTTING" );
			return;
		}
		DEBUG ( 4, "ANIMATING: ", Active_Node );
		nodes[Active_Node]->Interpolate ( Timer::Get_Time_d ( ) - Metronome );
	}
};

#endif