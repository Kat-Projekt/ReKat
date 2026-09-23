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

### Documenting Implemtation
When you define functions you must devide them by category.
( components -> adding -> string / template )
Also in the cpp files of the implemntation you must comment every first level block like:
```cpp
// ----------------------------
// ------ GameComponents ------
// ----------------------------
```

## Naming Conventions

Always use Tabs for indentation

### variables
```cpp
bool active;
int frame;
extern std::unordered_map < std::string int > map;
int* pointer;
```

### methos definitions and API functions
```cpp
int VoidFunction ( );

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

void Multi_Function_With_Defaults (
	char * memory_pointer = nullptr,
	int size = 0,
);
```

### Normal function definition for internal usage
```cpp
int void_function ( );

bool single_parameter ( bool value );

char * multiple_parameter (
	char * memory_pointer,
	int size
);

template < typename T >
void templated (
	char * memory_pointer,
	int size,
	T default_value
);

void function_with_defaults ( char * memory_pointer = nullptr );

void multi_defaults (
	char * memory_pointer = nullptr,
	int size = 0,
);
```

### Components / Resources
```cpp
class NewComponent : public Behaviour;
class NewResource : public Resource;
```

### Enumerators
```cpp
enum class EnumeratorForSomething;
```

### private, implementatin details and helpers
Use the underscore prefix for elements that are not supposed to pe accesible
```cpp
bool _active;
int _frame;
extern std::unordered_map < std::string int > _map;

void _Function_With_Defaults ( char * memory_pointer = nullptr );

void _Multi_Function_With_Defaults (
	char * memory_pointer = nullptr,
	int size = 0
);
```

### on declarations
when declaring a function use the sintax introduced before with the exceptions of
```cpp
bool Single_Parameter_Function
( bool value )
{
	/* Body */
}

char * Multiple_Parameters (
	char * memory_pointer,
	int size
) {
	/* Body */
}

bool Single_Line_Body ( )
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
Void_Function ( );
Single_Param_Function ( 2 );
Multi_Param_Function ( 1, 4, "string" );
```

in the case that the function call extensa over a screen you must:
```cpp
Long_Param_Function (
	1,
	4,
	"string"
);
```

### include priority
First include the imported libraries ( std, boost, freetype... ) with <>.
Second include engine implementation with "". separated by a new line
```cpp
#include <string>
#include <vector>

#include "behaviour.h"
#include "objekt.h"
```

## File naming

Components or Resources files should be named Exacly as the component/resource they implement and must be divided in a .cpp file and a .h file

Extensions must have a "components.hpp" that is the include manager that includes every component, resource and manager that the extension implements.

Managers and NameSpace Inizializers files must have a ".hpp" extension while implementations must have a ".h" file.

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

## Include libraires

For include Gards use both
```cpp
#pragma once
#ifndef FILE_H
#define FILE_H

#endif
```
