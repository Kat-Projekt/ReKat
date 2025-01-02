#ifndef LIST_H
#define LIST_H

#define integer unsigned long long int
#include <iostream>

#define Error(msg) std::cout << "error: " << msg << " at line: " << __LINE__ << " of file: " << __FILE__; throw

template < typename T >
class List {
public:
	struct Element {
		Element * prev = nullptr;
		T data;
		Element * next = nullptr;

		friend std::ostream& operator << ( std::ostream& os, const Element& ele ) {
			os << ele.data << " { " << ele.prev << ", " << ele.next << " }";
			return os;
		}
	};
	struct Iterator {
		Element * ele;

		Iterator& operator++() {
			if ( ele->next == nullptr ) 
			{ DEBUG ( 1, "Continuing with a nullptr"); }
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

		T operator*() const {
			return ele->data;
		}
	};
	
private:
	integer _size = 0;
	Element* _first = nullptr;
	Element* _last = nullptr;
public:
	List ( ) { _last = new Element; };
	void Deallocate ( ) {
		auto C = _first;
		while ( C != nullptr ) {
			auto C1 =  C->next;
			delete C;
			C = C1;
		}
	}
	// size of list
	integer size ( ) { return _size; }
	
	// gets element by index
	// nullptr if not found
	T operator[] ( integer index ) {
		if ( index > _size ) { Error("out of bounds"); }
		auto C = _first;
		for ( size_t i = 0; i < index; i++ ) 
		{ C = C->next; }
		
		return C->data;
	}
	// gets the index of the first occurence og element
	// -1 if not found
	integer operator[] ( T element ) {
		integer r = 0;
		auto C = _first;
		while ( C != nullptr ) {
			if ( C->data == element ) { return r; }
			C = C->next;
			r++;
		}
		return -1;
	}

	bool contains ( T element ) {
		auto C = _first;
		while ( C != nullptr ) {
			if ( C->data == element ) { return true; }
			C = C->next;
		}
		return false;
	}
	
	// adds data to list
	List* append ( T data ) {
		// first item
		if ( _first == nullptr ) {
			Element* e = new Element;
			e->data = data;
			e->prev = nullptr;
			e->next = _last;
			_last->prev = e;
			_first = e;
			_last = e;
			_size = 1;
			return this;
		}

		Element* e = new Element;
		e->data = data;
		e->prev = _last;
		e->next = _last->next;

		_last->next = e;
		_last = e;

		_size++;
		return this;
	}

	List* append ( List<T> data ) {
		if ( data.size() == 0 ) { return this; }
		// first item
		if ( _first == nullptr ) {
			_first = data.Get_Begin ( );
			_last = data.Get_Endin ( );
			_size = data.size ( );
			return this;
		}

		_last->next = data.Get_Begin ( );
		data.Get_Begin ( )->prev = _last;
		_last = data.Get_Endin ( );

		_size += data.size ( );
		return this;
	}

	// removes every istance of and element equal to data from list
	List* remove ( T data ) {
		auto C = _first;
		while ( C != nullptr ) {
			if ( C->data == data ) { break; }
			C = C->next;
		}
		// not found
		if ( C == nullptr ) { return this; }
		if ( C->next != nullptr ) { C->next->prev = C->prev; }
		if ( C->prev != nullptr ) { C->prev->next = C->next; }
		if ( C->next == nullptr && C->prev == nullptr ) { _first = nullptr; }
		_size--;
		return this;
	}
	// removes indexed element
	List* remove ( integer index ) {
		if ( index > _size ) { Error("out of bounds"); }
		auto C = _first;
		for ( size_t i = 0; i < index; i++ ) { C = C->next; }
		if ( C->next != nullptr ) { C->next->prev = C->prev; }
		if ( C->prev != nullptr ) { C->prev->next = C->next; }
		_size--;
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
		while (C != nullptr) { std::cout << i << ':' << C->data << '\n'; C = C->next; i++; }
	}
	
	friend std::ostream& operator << ( std::ostream& os, List<T>& list ) {
		auto C = list.Get_Begin( );
		auto E = list.Get_Endin( );
		// std::cout << "B: " << list.Get_Begin ( ) << " E: " << E << " S: " << list.size( ) << '\t';
		os << "S: " << list.size () << " { ";
		if ( list.size ( ) == 0 ) { os << "}"; return os; }
		os << C->data;
		C = C->next;
		while ( C != nullptr ) {
			// os << C << " {" << C->data << " p: " << C->prev << " n: " << C->next << ( C != E ? "} : " : "}");
			os << ", " << C->data;
			C = C->next;
		}
		os << " }";
		return os;
	}

	friend std::ostream& operator, ( std::ostream& out, List<T>& list )
	{ out << list; return out; }
};

#endif