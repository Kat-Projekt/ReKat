#ifndef LIST_H
#define LIST_H

#define integer unsigned long long int

#define Error(msg) std::cout << "error: " << msg << " at line: " << l << " of file: " << f; throw

template < typename T >
class List {
public:
	struct Element {
		Element * prev = nullptr;
		T data;
		Element * next = nullptr;
	};
private:
	integer _size = 0;
	Element* _first = nullptr;
	Element* _last = nullptr;
	
public:
	List ( ) { };
	// size of list
	integer size ( ) { return _size; }
	
	// gets element by index
	// nullptr if not found
	T* operator[] ( integer index ) {
		if ( index > _size ) { Error("out of bounds"); }
		auto C = _first;
		for ( size_t i = 0; i < index; i++ ) 
		{ C = C->next; }
		
		return C->data;
	}
	// gets the index of the first occurence og element
	// -1 if not found
	integer operator[] ( T elemment ) {
		integer r = 0;
		auto C = _first;
		while ( C->next != nullptr ) {
			if ( C->data == elemment ) { return r; }
			C = C->next;
			r++;
		}
		return -1;
	}
	
	// adds data to list
	List* append ( T data ) {
		// first item
		if ( _first == nullptr ) {
			Element* e = (Element*) calloc ( 1, sizeof(Element*) );
			e->data = data;
			e->prev = nullptr;
			e->next = nullptr;
			_first = e;
			_last = e;
			_size = 1;
			return this;
		}

		Element* e = (Element*) calloc ( 1, sizeof(Element*) );
		e->data = data;
		e->prev = _last;
		e->next = nullptr;

		_last->next = e;
		_last = e;

		size++;
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
		return this;
	}
	// removes indexed element
	List* remove ( integer index ) {
		if ( index > _size ) { Error("out of bounds"); }
		auto C = _first;
		for ( size_t i = 0; i < index; i++ ) { C = C->next; }
		if ( C->next != nullptr ) { C->next->prev = C->prev; }
		if ( C->prev != nullptr ) { C->prev->next = C->next; }
		return this;
	}

	Element* Get_Begin ( ) { return _first; }
};


#endif