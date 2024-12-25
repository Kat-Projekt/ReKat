#ifndef MAP_H
#define MAP_H

#include "list.h"

#define integer unsigned long long int

#define Error(msg) std::cout << "error: " << msg << " at line: " << __LINE__ << " of file: " << __FILE__; throw

template < typename K, typename T >
class Map {
public:
	struct Element {
		Element * prev = nullptr;
        K key;
		T data;
		Element * next = nullptr;
	};

    struct Pair {
        K key;
		T data;
    };
    
private:
	bool _unique = false;
	integer _size = 0;
	Element* _first = nullptr;
	Element* _last = nullptr;
	
public:
	Map ( bool Unique = false ) { _unique = Unique; };
	void deallocate ( ) {
		auto C = _first;
		while ( C != nullptr ) {
			auto C1 =  C->next;
			delete C;
			C = C1;
		}
	}
	// size of list
	integer size ( ) { return _size; }

    T* get ( K key, bool u ) {
        auto C = _first;
		while ( C != nullptr ) {
            if ( C->key == key ) 
            { return &C->data; }
			C = C->next;
		}
		return nullptr;
    }

    List <T> * get ( K key ) {
        List <T> * list = new List < T >;
        auto C = _first;
		while ( C != nullptr ) {
            if ( C->key == key ) 
            { list->append ( C->data ); }
			C = C->next;
		}
        return list;
    }

    List <T> * get ( K* keys, integer _l ) {
        List <T> * list = new List < T >;
        auto C = _first;
		while ( C != nullptr ) {
            for ( integer i = 0; i < _l; i++ ) {
                if ( C->key == keys[i] ) 
                { list->append ( C->data ); }
            }
            
			C = C->next;
		}
        return list;
    }
	
	bool contains ( K element ) {
		auto C = _first;
		while ( C != nullptr ) {
			if ( C->key == element ) { return true; }
			C = C->next;
		}
		return false;
	}
	
	// adds data to list
	Map* append ( Pair data_pair ) {
		// check uniquienes
		if ( _unique ) {
			if ( contains ( data_pair.key ) ) 
			{ DEBUG ( 2, "Trying to resuse a key" ); return this; }
		}

		// first item
		if ( _first == nullptr ) {
			Element* e = new Element;
			e->data = data_pair.data;
			e->key  = data_pair.key;
			e->prev = nullptr;
			e->next = nullptr;
			_first = e;
			_last = e;
			_size = 1;
			return this;
		}

		Element* e = new Element;
        e->data = data_pair.data;
        e->key  = data_pair.key;
		e->prev = _last;
		e->next = nullptr;

		_last->next = e;
		_last = e;

		_size++;
		return this;
	}

	Element* Get_Begin ( ) { return _first; }
	Element* Get_Endin ( ) { return _last; }
	Element* begin ( ) { return _first; }
	Element* end ( ) { return _last; }

	void Print ( ) {
		auto C = _first;
		int i = 0;
		while (C != nullptr) { std::cout << i << ':' << C->key << ':' << C->data << '\n'; C = C->next; i++; }
	}
	
	friend std::ostream& operator << ( std::ostream& os, Map<K,T>& map ) {
		auto C = map.Get_Begin( );
		auto E = map.Get_Endin( );
		os << "S: " << map.size () << " { ";
		if ( map.size ( ) == 0 ) { os << "}"; return os; }
		os << C->data;
		os << C->key;
		C = C->next;
		while ( C != nullptr ) {
			// os << C << " {" << C->data << " p: " << C->prev << " n: " << C->next << ( C != E ? "} : " : "}");
			os << ", " << C->key << ':' << C->data;
			C = C->next;
		}
		os << " }";
		return os;
	}

	friend std::ostream& operator,(std::ostream& out, Map<K,T>& map )
	{ out << map; return out; }
};

#endif