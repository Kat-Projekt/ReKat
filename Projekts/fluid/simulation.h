#ifndef _simulation
#define _simulation

#include <engine.hpp>
using namespace ReKat::grapik::Input;

class simulation : public Behaviour {
	struct node	{ // single cell of the simulation
		int neibourns = 4;
		float parzial_pression;
	};
	
	
public:
	node * current_matrix;
	int size_W = 128;
	int size_H = 128;
	int total = size_W*size_H;

	node * create_frame ( ) {
		node * frame;
		frame = ( node* ) calloc ( total, sizeof(node) );
		
		// setting neibours
		for ( size_t i = 0; i < total; i++ ) { frame[i].neibourns = 4; }

		// sides
		


		// corners
		frame[ 1 * size_W + 1 ].neibourns = 2;
		frame[ 1 * size_W + size_W - 2 ].neibourns = 2;
		frame[ ( size_H - 2 ) * size_W + 1 ].neibourns = 2;
		frame[ ( size_H - 2 ) * size_W + size_W - 2 ].neibourns = 2;

		return frame;
	}

	virtual void Start ( ) { // setup the matrix
		std::cout << "setting starting matrix\n";
		current_matrix = create_frame ( );
		for ( size_t i = 1; i < size_H-1; i++ ) {
			current_matrix[i * size_W + 1].parzial_pression = 1;
			current_matrix[i * size_W + 2].parzial_pression = 1;
			// sets the first row to high pressure
		}
	}
	virtual void Update ( ) {
		static int i;
		

		if ( ! ReKat::grapik::Input::Key_Down ( "W" ) ) { return; }
		node * new_current_matrix = create_frame ( );
		
		i++;

		// update state
		for (size_t x = 1; x < size_H-1; x++) { for (size_t y = 1; y < size_W-1; y++) { 
			// sum neibours pressure
			float new_parsial_pressure = current_matrix[ ( x - 1 ) * size_W + y].parzial_pression + current_matrix[ x * size_W + y - 1 ].parzial_pression + 
										 current_matrix[ ( x + 1 ) * size_W + y].parzial_pression + current_matrix[ x * size_W + y + 1 ].parzial_pression;
			new_parsial_pressure /= current_matrix[ x * size_W + y ].neibourns;

			new_current_matrix[ x * size_W + y ].parzial_pression = new_parsial_pressure;				
		} }

		new_current_matrix[ int(size_H/2 + sin (i*0.01)*10) * size_W + int(size_W/2 + cos (i*0.01)*10) ].parzial_pression = 1;

		free ( current_matrix );
		current_matrix = new_current_matrix;
		/*std::cout << "fluids\n";
		for (size_t x = 0; x < size_H; x++) {
			for (size_t y = 0; y < size_W; y++) 
			{ std::cout << current_matrix[x * size_W +y].parzial_pression << ", "; }
			std::cout << '\n';
		}*/
	}
};

#endif