#include <engine.hpp>
#include "Scripts/hello_world.h"

int load ( ) {
	int result = 0;
	result += Manager::Shader_Load ( "base", "Shaders\\sprite.vs", "Shaders\\sprite.fs" );
	result += Manager::Texture_Load ( "gobelino", "Sprites\\Enemies\\gobelino.png" );
	if ( result != 0 ) { throw; }
	// creating objekts 
	auto obj_0 = Manager::Objekt_Load ( "franco" );
	auto obj_1 = Manager::Objekt_Load ( "lorenzo" );
	// linking objekts 
	obj_1->Add_Child ( obj_0 );
	// adding components to objekts 
	auto com_0 = new Sprite ( );
	obj_1->Add_Component < Fps > ( );
	obj_1->Add_Component < hello_world > ( );
	obj_1->Add_Component < Camera > ( );
	com_0->Set ( Manager::Texture_Get("gobelino"), Manager::Shader_Get("base"), obj_1->Get_Component<Camera>() );
	obj_0->Add_Component ( com_0 );
	// adding entry point
	Manager::Set_Active_Scene ( "lorenzo" );
	return 0;
}