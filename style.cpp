//	=========================================================
//		FORMATTING EXAMPLE
//	=========================================================

//	Questo file mostra lo stile da usare nel progetto.

//	---------------------------------------------------------
//	CLASSES
//	---------------------------------------------------------

class Behaviour
{
protected:
	bool _active = true;

public:
	Behaviour ( );
	virtual ~Behaviour ( );

	void Update ( );
};

//	---------------------------------------------------------
//	CONSTRUCTORS AND DESTRUCTORS
//	---------------------------------------------------------

Behaviour::Behaviour ( ) { }

Behaviour::~Behaviour ( ) { }

//	---------------------------------------------------------
//	FUNCTIONS
//	---------------------------------------------------------

void Start ( void ) { }

void Update 
(
	int arg = 0,
	char arg2 = 'x'
) { }

void Update ( char arg )
{ }

void Behaviour::Update ( )
{
	if ( _active )
	{ Start ( ); }
}

//	---------------------------------------------------------
//	NAMESPACES
//	---------------------------------------------------------

namespace Manager
{
	void Start ( )
	{
		// DEBUG ( 1, "Starting..." );
		// DEBUG ( 1, "Starting..." );
		// DEBUG ( 1, "Starting..." );
	}
}

//	---------------------------------------------------------
//	CONDITIONAL STATEMENTS
//	---------------------------------------------------------


void conditionals ( )
{
	// One line
	if ( _active )
	{ Update ( ); }

	// More than one line
	if ( _active )
	{
		Start ( );
		Update ( );
	}
}

//	---------------------------------------------------------
//	LOOPS
//	---------------------------------------------------------

void loops ( )
{
	char objs[5];

	// One line
	for ( auto O : objs )
	{ Start ( ); }

	// More than one line
	for ( auto O : objs )
	{ 
		Start ( ); 
		Update ( ); 
	}
}

//	---------------------------------------------------------
//	POINTERS
//	---------------------------------------------------------

Objekt * obj = nullptr;
Behaviour * behaviour = nullptr;

//	---------------------------------------------------------
//	FUNCTION CALL SPACING
//	---------------------------------------------------------

Start ( );
Update ( 1, 'b' );


//	---------------------------------------------------------
//	SHORT RETURNS
//	---------------------------------------------------------

bool Behaviour::Get_Active ( )
{ return _active; }

Behaviour * Behaviour::Set ( )
{ return this; }

//	---------------------------------------------------------
//	UNUSED PARAMETERS
//	---------------------------------------------------------

void Behaviour::Collision ( Objekt * _obj )
{
	( void ) _obj;
}

//	---------------------------------------------------------
//	NAMING CONVENTIONS
//	---------------------------------------------------------

class Behaviour;
class Objekt;
class Renderer;

bool _active;
bool _started;

void Start ( );
void Update ( );
void Fixed_Update ( );

//	---------------------------------------------------------
//	GENERAL PRINCIPLES
//	---------------------------------------------------------

// Use tabs for indentation.
// Keep formatting consistent.
// Prefer readability over compactness.
// Follow the same style throughout the entire project.