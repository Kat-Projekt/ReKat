#include <core/maestro.h>

int main ( )
{
	Maestro m;
	DEBUG ( DebugLevel::INFO, "Registred components: ", m.Register_Directory ( "." ) );

	auto t = m.Construct ( "TestReflection" );

	t->Perform ( "Print" );
}
