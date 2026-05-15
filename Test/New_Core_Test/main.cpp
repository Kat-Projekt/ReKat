#define DIAGNOSTIC
// #define EXPANCE
#include <engine.hpp>

class C1 : public Behaviour
{
	void Update ( )
	{ DEBUG ( 4, obj->Get_Name ( ), " C1" ); }
};
class C2 : public Behaviour
{
	void Update ( )
	{ DEBUG ( 4, obj->Get_Name ( ), " C2" ); }
};
class C3 : public Behaviour
{
	void Update ( )
	{ DEBUG ( 4, obj->Get_Name ( ), " C3" ); }
};

int main ( )
{
	// create a tree gierarcky ( creation order is scrambled )

	Objekt pippo111 ( "p111" ); // son of pippo11
	Objekt pippo ( "pippo" ); // root node
	Objekt nino1 ( "nino1" ); // son of nino
	Objekt pippo11 ( "p11" ); // son of pippo1
	Objekt pippo21 ( "p21" ); // son of pippo2
	Objekt nino ( "nino" ); // root node
	Objekt pippo1 ( "p1" ); // son of pippo
	Objekt pippo22 ( "p22" ); // son of pippo2
	Objekt pippo23 ( "p23" ); // son of pippo2
	Objekt pippo2 ( "p2" ); // son of pippo
	
	// create tree ( also randomized )
	pippo.Add_Child ( &pippo2 );
	pippo2.Add_Child ( &pippo21 );
	pippo1.Add_Child ( &pippo11 );
	nino.Add_Child ( &nino1 );
	pippo.Add_Child ( &pippo1 );
	pippo2.Add_Child ( &pippo22 );
	pippo11.Add_Child ( &pippo111 );
	pippo2.Add_Child ( &pippo23 );

	// add random components 
	pippo.Add_Component < C1 > ( );
	pippo.Add_Component < C2 > ( );
	pippo21.Add_Component < C3 > ( );
	nino1.Add_Component < C2 > ( );
	pippo111.Add_Component < C2 > ( );
	nino.Add_Component < C1 > ( );
	pippo21.Add_Component < C3 > ( );
	pippo21.Add_Component < C3 > ( );
	pippo.Add_Component < C3 > ( );
	pippo11.Add_Component < C1 > ( );
	pippo21.Add_Component < C3 > ( );

	// call update 
	pippo.Print_Tree ( );
	pippo.Update ( );

	// print the trasnsimttable
}