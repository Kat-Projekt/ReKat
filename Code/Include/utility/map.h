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


	struct Iterator {
		Element * ele;

		Iterator& operator++() {
			ele = ele->next;
			return *this;
		}
		Iterator operator++(int) {
			Iterator __t(*this);
			++(*this);
			return __t;
		}

		Iterator& operator--() {
			ele = ele->prev;
			return *this;
		}
		Iterator operator--(int) {
			Iterator __t(*this);
			--(*this);
			return __t;
		}

		bool operator!= ( Iterator &I ) 
		{ return ( I.ele != this->ele ); }

		Pair operator*() const { return { ele->key, ele->data }; }
	};
    
private:
	bool _unique = false;
	integer _size = 0;
	Element* _first = nullptr;
	Element* _last = nullptr;
	
public:
	Map ( bool Unique = false ) { _last = new Element; _unique = Unique; };
	~Map ( ) { deallocate ( ); }
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

    T get ( K key, bool u ) {
        auto C = _first;
		while ( C != nullptr ) {
            if ( C->key == key ) 
            { return C->data; }
			C = C->next;
		}
		DEBUG ( 1, "Cannot Find Keyd Item" );
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

    List <T> get ( K* keys, integer _l ) {
		DEBUG ( 5, "Getting" );
        List <T> list;
        auto C = _first;
		if ( _first == nullptr ) 
		{ DEBUG ( 4, "Void _first" ); return list; }
		while ( C != nullptr ) {
			try {
				DEBUG ( 5, "Getting with C: ", C->key );
				for ( integer i = 0; i < _l; i++ ) {
					DEBUG ( 6, "key: ", keys[i] );
					if ( C->key == keys[i] ) 
					{ list.append ( C->data ); }
				}
			} catch(const std::exception& e) {
				DEBUG ( 2, e.what ( ) );
			}
			C = C->next;
		}
		DEBUG ( 5, "Getted" );
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
			e->next = _last;
			_last->prev = e;
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
	Iterator begin ( ) {
		Iterator pippo;
		pippo.ele = _first;
		return pippo;
	}
	Iterator end ( ) {
		Iterator pippo;
		pippo.ele = _last->next;
		return pippo;
	}

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