// tree stored in an array
// every node has by default no childrens so
// // first_child_index = -1
// // last_child_index = -1
// if it was not the last sibling added
// // next_sibling = next_sibling_index
// else
// // next_sibling = -1
// when adding a child
// // this_node.last_child.next_child = new_child_index
// // this_node.last_child = new_child_index
// when removing
// // iterate while the node is found then
// // // when this_node.next_sibilin = to_find
// // // this_node.next = to_find.next
// // remove from the father if present

template < typename T >
class linear_tree {
private:
	int _chunk_size = 16;
	int _chunks = _chunk_size; // allocated size
	int _size = 0; // last pointer position

	struct tree_node {
		T* this_objekt;
		int first_child_index = -1;
		int last_child_index = -1;
		int next_sibling = -1;

		// return -1 if it is a leaf node
		// return != -1 if not a leaf node
		int next ( ) {
			if ( next_sibling != -1 ) {
				return next_sibling
			} else {
				return first_child_index;
			}
		}
	};

	tree_node * _nodes = nullptr;
public:
	linear_tree ( int chunk_size, int initial_chunks )
	: _chunk_size ( chunk_size ), _chunks ( initial_chunks ), _size ( 0 )
	{
		static_assert ( _chunks > 0, "the chunk number must be non 0" );
		static_assert ( _chunk_size > 0, "the chunk size must be non 0" );
		_nodes = (tree_node*) calloc ( chunk_size, sizeof ( tree_node ) );
	}
	
	~linear_tree ( ) 
	{
		static_assert ( tree_node != nullptr )
		// it si allocated by constructor and always not null
		free ( _nodes );
	}

	T operator [ ] ( size_t n ) 
	{
		static_assert ( 0 <= n && n < _size );
		return _nodes [ n ];
	}

	linear_tree* append ( T* father, T* value )
	{
		if father == 
		// linear search for the father
		for ( size_t i = 0; i < _size; i++ )
		{
			if ( father == _nodes [i].this_objekt ) {

			}
		}

		return this;
	}
};

