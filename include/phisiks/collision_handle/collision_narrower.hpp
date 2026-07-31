#pragma once
#include "collider.hpp"
#include "collision_result.hpp"

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

	bool operator== ( collision_check& _lf )
	{
		return ( collider1 == _lf.collider1 && collider2 == _lf.collider2 ) ||
		( collider1 == _lf.collider2 && collider2 == _lf.collider1 );
	}
	bool operator!= ( collision_check& _lf )
	{
		return ! ( collider1 == _lf.collider1 && collider2 == _lf.collider2 ) && !
		( collider1 == _lf.collider2 && collider2 == _lf.collider1 );
	}
};

struct collision_hash {
	size_t operator ( ) ( const collision_check& c ) const {
		size_t h1 = std::hash<size_t>()((size_t)c.collider1);
		size_t h2 = std::hash<size_t>()((size_t)c.collider2);
		return h1 ^ h2;
	}
};

bool operator== ( const collision_check&_lt, const collision_check&_rt ) {
	return ( _rt.collider1 == _lt.collider1 && _rt.collider2 == _lt.collider2 ) ||
	( _rt.collider1 == _lt.collider2 && _rt.collider2 == _lt.collider1 );
}

std::ostream& operator << (
	std::ostream& os, 
	std::unordered_map < collision_check, collision_result, collision_hash > m
) {
	os << "{";
	for ( auto e : m ) {
		os << e.first << " " << e.second << ' ';
	}
	os << "}";
	return os;
}

class Collision_Narrower {
protected:
	List < Collider* > _colliders;
public:
	void Set_Colliders ( List < Collider* > &colliders )
	{ _colliders = colliders; }
	std::vector < collision_check > Get_Collisions_To_Check ( ) {
		return std::vector < collision_check > ( );
	}
};