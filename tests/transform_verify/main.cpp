#define DIAGNOSTIC
#include <engine.hpp>

int main ( )
{
	Objekt tester ( "pippo", {0,0,0}, {200,200,200} );
	DEBUG ( 4, tester.Get_Pos ( ) );
	DEBUG ( 4, tester.Get_Size ( ) );
	DEBUG ( 4, tester.Get_Rot ( ) );

	DEBUG ( 3, tester.Get_Model_Mat ( ) );

	DEBUG ( 5, tester.Get_Transform ( ) );
}