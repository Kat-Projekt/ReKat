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
	node ** current_matrix;
	int size_W = 32;
	int size_H = 32;

	virtual void Start ( ) { // setup the matrix
		std::cout << "setting starting matrix\n";
		current_matrix = (node **) calloc ( size_H, sizeof(node*) );	
		for ( size_t i = 0; i < size_H; i++ ) 
		{ current_matrix[i] = (node *) calloc ( size_W, sizeof(node) ); }

		// inputing starting data
		for ( size_t i = 0; i < size_W; i++ ) {
			current_matrix[1][i].parzial_pression = 10;
			// sets the first row to high pressure
		}

		// set neibours count
		std::cout << "setting neibours\n";
		for (size_t x = 0; x < size_H; x++) {
			for (size_t y = 0; y < size_W; y++) 
			{ current_matrix[x][y].neibourns = 4; }
		}

		// sides
		for (size_t i = 1; i < size_H-2; i++) {
			current_matrix[i][1].neibourns = 3;
			current_matrix[i][size_W-2].neibourns = 3;
		}
		for (size_t i = 1; i < size_H-2; i++) {
			current_matrix[1][i].neibourns = 3;
			current_matrix[size_H-2][i].neibourns = 3;
		}
		// corsers
		current_matrix[1][1].neibourns = 2;
		current_matrix[1][size_W-2].neibourns = 2;
		current_matrix[size_H-2][1].neibourns = 2;
		current_matrix[size_H-2][size_W-2].neibourns = 2;

		std::cout << "neibours\n";
		// pints 
		for (size_t x = 0; x < size_H; x++) {
			for (size_t y = 0; y < size_W; y++) 
			{ std::cout << current_matrix[x][y].neibourns << ", "; }
			std::cout << '\n';
		}
		std::cout << "fluids\n";
		for (size_t x = 0; x < size_H; x++) {
			for (size_t y = 0; y < size_W; y++) 
			{ std::cout << current_matrix[x][y].parzial_pression << ", "; }
			std::cout << '\n';
		}
		
	}
	virtual void Update ( ) {
		static int i;
		i++;

		if ( ! ReKat::grapik::Input::Key_Down ( "W" ) ) { return; }
		node ** new_current_matrix = (node **) calloc ( size_H, sizeof(node*) );	
		for ( size_t i = 0; i < size_H; i++ ) 
		{ new_current_matrix[i] = (node *) calloc ( size_W, sizeof(node) ); }

		// filling neibours
		for (size_t x = 1; x < size_H-1; x++) {
			for (size_t y = 0; y < size_W; y++) 
			{ new_current_matrix[x][y].neibourns = 4; }
		}
		// sides
		for (size_t i = 1; i < size_H-1; i++) {
			new_current_matrix[i][1].neibourns = 3;
			new_current_matrix[i][size_W-2].neibourns = 3;
		}
		for (size_t i = 1; i < size_H-1; i++) {
			new_current_matrix[1][i].neibourns = 3;
			new_current_matrix[size_H-2][i].neibourns = 3;
		}
		// corsers
		new_current_matrix[1][1].neibourns = 2;
		new_current_matrix[1][size_W-2].neibourns = 2;
		new_current_matrix[size_H-2][1].neibourns = 2;
		new_current_matrix[size_H-2][size_W-2].neibourns = 2;

		// update state
		for (size_t x = 1; x < size_H-1; x++) { for (size_t y = 1; y < size_W-1; y++) { 
			// sum neibours pressure
			float new_parsial_pressure = current_matrix[x-1][y].parzial_pression + current_matrix[x][y-1].parzial_pression + 
										 current_matrix[x+1][y].parzial_pression + current_matrix[x][y+1].parzial_pression;
			new_parsial_pressure /= current_matrix[x][y].neibourns;

			new_current_matrix[x][y].parzial_pression = new_parsial_pressure;				
		} }

		new_current_matrix[int(size_H/2 + sin (i*0.01)*10)][int(size_W/2 + cos (i*0.01)*10)].parzial_pression = 1;

		current_matrix = new_current_matrix;
		/*std::cout << "fluids\n";
		for (size_t x = 0; x < size_H; x++) {
			for (size_t y = 0; y < size_W; y++) 
			{ std::cout << current_matrix[x][y].parzial_pression << ", "; }
			std::cout << '\n';
		}*/
	}
};

#endif