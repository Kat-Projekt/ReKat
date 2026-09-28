#include <core/behaviour.h>

class Test : public Behaviour
{
	int pino = 10;
	std::string saluto = "ciao";
	float nino = 3.5f;

	std::string Get_Saluto ( ) const { return saluto; }
	void Set_Saluto ( std::string n ) { saluto += n; }

	int Print ( )
	{
		DEBUG ( DebugLevel::NOTICE, "saluto: ", saluto, " pino: ", pino, " nino: ", nino );
		return 0;
	}

	int Print_Valore ( std::string valore )
	{
		DEBUG ( DebugLevel::NOTICE, valore );
		return 0;
	}

	std::string Saluta_Nino ( )
	{
		return saluto + " " + std::to_string ( nino );
	}

	std::string Saluta ( std::string chi )
	{
		return saluto + " " + chi;
	}

public:
	METADATA ( Test, "semplice test di funzionalità", 1,0,0,true );

	PARAMETERS (
		PARAMETER ( pino ),
		PARAMETER ( nino ),
		PROPERTY  ( saluto, Set_Saluto, Get_Saluto )
	)

	METHODS (
		METHOD ( Saluta, chi ),
		NO_ARGUMENTS ( Print ),
		METHOD ( Print_Valore, valore ),
		NO_ARGUMENTS ( Saluta_Nino )
	)
};

int main ( )
{
	DEBUG ( DebugLevel::WARN, "Starting Behaviour Testing" );
	
	// Inizializing a Test objekt and using only the Behaviour api
	Test _test;
	DEBUG ( DebugLevel::NOTICE, "Metadata", _test.metadata );
	Behaviour * beh = static_cast < Behaviour * > ( & _test );

	// testing parameters configuration
	beh->Perform ( "Print" );
	beh->Configure ( arg(pino)=4, arg(nino)=1.0f, arg(saluto) = "hello" );
	beh->Perform ( "Print" );

}
