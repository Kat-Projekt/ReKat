#ifndef LIST_H
#define LIST_H

#define integer unsigned long long int

#define Error(msg) std::cout << "error: " << msg << " at line: " << __LINE__ << " of file: " << __FILE__; throw

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
			e->next = nullptr;
			_first = e;
			_last = e;
			_size = 1;
			return this;
		}

		Element* e = new Element;
		e->data = data;
		e->prev = _last;
		e->next = nullptr;

		_last->next = e;
		_last = e;

		_size++;
		return this;
	}

	List* append ( List<T> * data ) {
		if ( data == nullptr || data->size() == 0 ) { return this; }
		// first item
		if ( _first == nullptr ) {
			_first = data->Get_Begin ( );
			_last = data->Get_Endin ( );
			_size = data->size ( );
			return this;
		}

		_last->next = data->Get_Begin ( );
		data->Get_Begin ( )->prev = _last;
		_last = data->Get_Endin ( );

		_size += data->size ( );
		return this;
	}

	// removes every istance of and element equal to data from list
	List* remove ( T data ) {
		auto C = _first;
		while ( C != nullptr ) {
			if ( C->data == data ) { std::cout << "removed\n"; break; }
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
	Element* begin ( ) { return _first; }
	Element* end ( ) { return _last; }

	void Print ( ) {
		auto C = _first;
		int i = 0;
		while (C != nullptr) { std::cout << i << ':' << C->data << '\n'; C = C->next; i++; }
	}
	
	friend std::ostream& operator << ( std::ostream& os, List<T>& list ) {
		auto C = list.Get_Begin( );
		auto E = list.Get_Endin( );
		std::cout << "B: " << list.Get_Begin ( ) << " E: " << E << " S: " << list.size( );
		if ( list.size ( ) == 0 ) { return os; }
		os << "\n";
		while ( C != nullptr ) {
			std::cout << C << " {" << C->data << " p: " << C->prev << " n: " << C->next << ( C != E ? "} : " : "}");
			C = C->next;
		}
		return os;
	}
};



#endif