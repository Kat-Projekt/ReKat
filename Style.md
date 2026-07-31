# Style Guide for the Engine

This file describes how the code should be written and how it should be documented

## Software used

- Doxygen

## Documenting Basics

Use Javadoc style of boxing your documentation adn use markdown for text
```cpp
/**********************************************
 * \brief this is a the summary of the function
 *        can be continued to the new line
 * 
 * Here begins the detailed explanation of the
 * code we are documenting.
 * 
 * This is a second paragraph of documentation
 * 
 * @param arg1 this is the first argument
 * @param arg2 this is another argument
 * @param n this is a number
 * @return the return value meaning
 * 
 * The following ar suggetions for the user
 * 
 * @see function()
 * @see variable()
 *********************************************/
```

in the case of documenting a file you can use
```cpp
/**********************************
 * \file file_name
 * \brief the summary of it's usage
 * 
 * Description
 *********************************/
```

### Documenting Components

Components documentation is grupped under the Components Topic
Then each extension has it's own group for bether organizzation:
```
└── Components
    ├── Graphik
    ├── Logik
    ├── Connekt
    ├── Musik
    ├── Utility
    └── ...
```

To define a new group use
```cpp
/*********************************************
 * @defgroup GameComponents My game components 
 * @ingroup Components
 ********************************************/
```

Then addin your documentation
```cpp
/************************************
 * @ingroup GameComponents
 * @brief A new component for my game
 ***********************************/
```

## Naming Conventions

Always use Tabs for indentation

### variables
```cpp
bool active;
int frame;
extern std::unordered_map < std::string int > map;
```

### methos definitions
```cpp
int VoidFunction ( void );

bool SingleParameterFunction ( bool value );

char * MultipleParameters (
	char * memory_pointer,
	int size
);

template < typename T >
void Templated (
	char * memory_pointer,
	int size,
	T default_value
);

void FunctionWithDefaults ( char * memory_pointer = nullptr );

void MultiFunctionWithDefaults (
	char * memory_pointer = nullptr,
	int size = 0,
);
```

### Components / Resources
```cpp
class NewComponent : public Behaviour;
class NewResource : public Resource;
```

### private
for signaling that something is private use an underscore before it's name
```cpp
bool _active;
int _frame;
extern std::unordered_map < std::string int > _map;

void _FunctionWithDefaults ( char * memory_pointer = nullptr );

void _MultiFunctionWithDefaults (
	char * memory_pointer = nullptr,
	int size = 0
);
```

### on declarations
when declaring a function use the sintax introduced before with the exceptions of
```cpp
bool SingleParameterFunction
( bool value )
{
	/* Body */
}

char * MultipleParameters (
	char * memory_pointer,
	int size
) {
	/* Body */
}

bool SingleLineBody
( void )
{ return true; }
```

### conditional and statements
```cpp
if ( one_line_condition )
{ /* One line */ }

if ( condition )
{
	/* More than one line */
} else {
	/* Else Body */
}

for ( i; condition; i++ )
{ /* One line */ }

for ( i; condition; i++ )
{
	/* More than one line */
}

for ( auto iterator : vector )
{
	/* More than one line */
}
```

### function calls
```cpp
VoidFunction ( );
SingleParamFunction ( 2 );
MultiParamFunction ( 1, 4, "string" );
```

in the case that the function call extensa over a screen you must:
```cpp
LongParamFunction (
	1,
	4,
	"string"
);
```

## File naming

Components or Resources files should be named Exacly as the component/resource they implement and must be divided in a .cpp file and a .h file

Extensions must have a "components.hpp" that is the include manager that includes every component, resource and manager that the extension implements.

Managers and Space Inizializers files must have a ".hpp" extension while implementations must have a ".h" file.

Template declarations must be put in a ".tpp" file for clarity

Example
```
├── objekt.h
├── manager.hpp
├── behaviour.h
└── Graphik
    ├── components
        ├── Button.h
        ├── Sprite.h
        └── ...
    ├── resources
        ├── Texture.h
        ├── Font.h
        └── ...
    ├── components.hpp
    ├── window.h
    ├── input.h
    ├── manager.hpp
    ├── ...
    └── graphik.hpp
```
