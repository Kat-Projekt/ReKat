#define DIAGNOSTIC
// #define EXPANCE
#include <engine.hpp>

class C1 : public Behaviour
{
	void Update ( )
	{ DEBUG ( 4, "Update ", obj->Get_Name ( ), " C1" ); }
};
class C2 : public Behaviour
{
	void Update ( )
	{ DEBUG ( 4, "Update ", obj->Get_Name ( ), " C2" ); }
};
class C3 : public Behaviour
{
	void Update ( )
	{ DEBUG ( 4, "Update ", obj->Get_Name ( ), " C3" ); }
};

int main ( )
{
	// create a tree gierarcky ( creation order is scrambled )

	std::shared_ptr < Objekt > pippo111 = std::make_shared < Objekt > ( "p111" ); // son of pippo11
	std::shared_ptr < Objekt > pippo = std::make_shared < Objekt > ( "pippo" ); // root node
	std::shared_ptr < Objekt > nino1 = std::make_shared < Objekt > ( "nino1" ); // son of nino
	std::shared_ptr < Objekt > pippo11 = std::make_shared < Objekt > ( "p11" ); // son of pippo1
	std::shared_ptr < Objekt > pippo21 = std::make_shared < Objekt > ( "p21" ); // son of pippo2
	std::shared_ptr < Objekt > nino = std::make_shared < Objekt > ( "nino" ); // root node
	std::shared_ptr < Objekt > pippo1 = std::make_shared < Objekt > ( "p1" ); // son of pippo
	std::shared_ptr < Objekt > pippo22 = std::make_shared < Objekt > ( "p22" ); // son of pippo2
	std::shared_ptr < Objekt > pippo23 = std::make_shared < Objekt > ( "p23" ); // son of pippo2
	std::shared_ptr < Objekt > pippo2 = std::make_shared < Objekt > ( "p2" ); // son of pippo
	
	// create tree ( also randomized )
	pippo->Add_Child ( pippo2 );
	pippo2->Add_Child ( pippo21 );
	pippo1->Add_Child ( pippo11 );
	nino->Add_Child ( nino1 );
	pippo->Add_Child ( pippo1 );
	pippo2->Add_Child ( pippo22 );
	pippo11->Add_Child ( pippo111 );
	pippo2->Add_Child ( pippo23 );

	Factory::Register ( "Comp_Test.dylib" );

	// add random components 
	pippo->Add_Component < C1 > ( );
	pippo->Add_Component < C2 > ( );
	pippo21->Add_Component < C3 > ( );
	nino1->Add_Component < C2 > ( ); //
	pippo111->Add_Component < C2 > ( );
	nino->Add_Component < C1 > ( ); //
	pippo21->Add_Component < C3 > ( );
	pippo21->Add_Component < C3 > ( );
	pippo->Add_Component < C3 > ( );
	pippo11->Add_Component < C1 > ( );
	pippo21->Add_Component < C3 > ( );

	for ( auto con : Factory::constructors )
	{
		DEBUG ( 5, "Components ", con.first );
	}

	DEBUG ( 3, "Trying special" );
	pippo->Add_Component_Special ( "NewComponent" );

	DEBUG ( 6, *pippo );
	DEBUG ( 5, "Has new comp? ", ( pippo->Has_Component ("12NewComponent") ? "true" : "false" ) );

	pippo->Get_Component ( "12NewComponent" )->Update ( );

	// Start and Update
	pippo->Start ( ); 
	pippo->Print_Tree ( );
	pippo->Update ( );

	DEBUG ( 3, "ENDED" );
}