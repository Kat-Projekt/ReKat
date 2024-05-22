#include <engine.hpp>
#include "simulation_render.h"
#include "simulation.h"

int load ( ) {
	int result = 0;
	result += Manager::Shader_Load ( "renderer", "render.vs", "render.fs" );
	if ( result != 0 ) { throw; }
	// creating objekts 
	auto obj_0 = Manager::Objekt_Load ( "main" );
	// linking objekts 
	// adding components to objekts 
	obj_0->Add_Component < simulation > ( );
	obj_0->Add_Component < simulation_render > ( );
	obj_0->Add_Component < Fps > ( )->MAX_FPS = 12;
	// adding entry point
	Manager::Set_Active_Scene ( "main" );
	return 0;
}