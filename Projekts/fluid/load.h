#include <engine.hpp>
#include "simulation_render.h"
#include "simulation.h"

int load ( ) {
	int result = 0;
	result += Manager::Shader_Load ( "renderer", "render.vs", "render.fs" );
	result += Manager::Shader_Load ( "sprite", "sprite.vs", "sprite.fs" );
	result += Manager::Texture_Load ( "test", "Kat Logo.png" );
	if ( result != 0 ) { throw; }
	// creating objekts 
	auto obj_0 = Manager::Objekt_Load ( "main", {0,0,0}, {206,65,0} );
	// linking objekts 
	// adding components to objekts 
	obj_0->Add_Component < simulation > ( );
	obj_0->Add_Component < simulation_render > ( );
	obj_0->Add_Component < Fps > ( )->MAX_FPS = -1;
	obj_0->Add_Component < Camera > ( );
	obj_0->Add_Component < Sprite > ( )->Set ( Manager::Texture_Get ( "test" ), Manager::Shader_Get ( "sprite" ), obj_0->Get_Component < Camera > ( ) );
	// adding entry point
	Manager::Set_Active_Scene ( "main" );
	return 0;
}