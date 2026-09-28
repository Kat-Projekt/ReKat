#include <engine.hpp>

class TestReflection : public Behaviour
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
	METADATA ( TestReflection, "semplice test di funzionalità", 1,0,0,true );

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
