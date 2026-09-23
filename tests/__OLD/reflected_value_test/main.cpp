#include <iostream>
#include "behaviour.h"
#include "metadata.h"

class Test : public Behaviour {
	int value1 = 67;
	float value2 = 6.7f;
	std::string ciao = "ciao";

	void Set_Ciao ( std::string _new )
	{ ciao += _new; }

	std::string Get_Ciao ( )
	{ return ciao; }

	int Print ( )
	{ std::cout << ciao << " 1:" << value1 << " 2:" << value2 << '\n'; return 0; }

public:
	METADATA ( "Test", "Semplice test per metadata", 1, 10, 4, true );

	METHODS (
		NO_ARGUMENTS ( Print )
	);

	PARAMETERS (
		PARAMETER ( value1 ),
		PARAMETER ( value2 ),
		PROPERTY ( ciao, Set_Ciao, Get_Ciao )
	);
};

int main ( )
{
	Test b;
	b.Perform ( "Print" );
	b.Configure ( arg(value1) = 42, arg(value2) = 4.2f, arg(ciao) = "bella" );
	b.Perform ( "Print" );
	std::cout << b.metadata << '\n';

	return 0;
}
