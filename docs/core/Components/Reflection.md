# Reflections
This document gives an overview of all the reflection methods and classes.

## Intended usage

```cpp
auto aktor = Director::Create < Aktor > ( "Giorgio" );
auto comp  = aktor.Add_Behaviour ( "Runner" );
comp->Configure ( arg(hp) = 10, arg(AI) = true, arg(sprite) = "Giorgio" );
comp->Perform ( "Give Weapon", arg(weapon) = "gun", arg(ammo) = 5 );
comp->Perform ( "Give Weapon", arg(weapon) = "sword" );
int hp = comp->Query ( "hp" );
string sprite = comp->Query ( "sprite" );
```

## ReflectionValue
All reflected parameterets are rappresented as
```cpp
class ParameterValue ( );
```
This parameters can be instantiated like
```cpp
ParameterValue par = "integer" = 10;
ParameterValue par = "float" = 10.0f;
ParameterValue par = "string" = "ciao";
ParameterValue par = "vec3" = vec3{1,0,2};
...
```
Where value can be any type.
This ParameterValue can be implicitally converted like:

```cpp
ParameterValue par = ( "integer" = 10 );
int value = par;
std::vector<int> value = par; // This is not valid so an error is thrown
```
Parameters can be concatenated via the comma operator like
```cpp
ParameterValues = "par" = 10, "arg" = 4.2f;
```

## Reflected Functions
When you need to expose a function the following is the main way
la funzione non conosce di essere riflessa.
```cpp
ParameterValue // this is the generic type
function_name  // name 
( ParameterValues args ) // this is a wrapper for n number of arguments
{
    std::string sprite = args ( "sprite", "default" ); // this gets the sprite argument and converts it to string
    hp = args ( "hp", hp ); // this gets the hp arguments and conret it to int.
    
    MEHTOD ( "Set 2D rot", set_.., float, rot_z );

    /*body*/
    
    return value; // this can be any type and it will be implicitally converted
}
```
alternatively you can omette: the type ( becomes void ) and the arguments giving

if the arguments is missing and no default is specified throws, if it is not specified but there is a defualt defualt is used.

```cpp
METHOD ( "name", pointer, type1, value, ... );
```
Then to call the methods you always use

```cpp
component.Perform ( "fun", "arg1" = 10, "arg2" = vec3{1,2,3} );
int result = component.Perform ( "sum", "a" = 1, "b" = 2 );
```

## ReflectionComponent
When creating a new component you can Reflect it's contents in the following whay:

```cpp
class Transfrom : public Behaviour
{
	REFLECT ( Transform );
            // name, version, description
	METADATA ( "Transform", 1.0, "Descries the affine transformation of the objekt" );
}
```

## ReflectionParameter
You can add reflected Parameters to a component in two ways:
- **Parameter**: Value with automatic setter and getter
- **Property**: Value with custom setter and getter
Reflected parameters are the main what to configure a component.
Since this are reflected you do not need to create a setter or getter but simply reflect them

```cpp
class Transfrom : public Behaviour
{
	REFLECT ( Transform );
	METADATA ( "Transform", 1.0, "Descries the affine transformation of the objekt" );
	PARAMETERS (
		PARAMETER ( "pos", _pos ),
		PARAMETER ( "size", _size ),
		PARAMETER ( "rot_pivot", _rot_pivot ),
		PROPERTY ( "_rot", Set_Rot, Get_Local_Rotation )
	);

	...
}
```

Then in the code to modify them use the parameterValue like:

```cpp
component.Configure ( "sprite" = "Giorgio", "hp" = 10, "vector" = vec3{1,0,0} );
int hp = component.Query ( "hp" );
```

## ReflectionMethod
Some times you need to export a method form a component you can do it like:
```cpp
class Transfrom : public Behaviour
{
	REFLECT ( Transform );
	METADATA ( "Transform", 1.0, "Descries the affine transformation of the objekt" );
	METHODS (
		METHOD ( "Get Model Matrix", Get_Model_Mat ),
		METHOD ( "Get Model Matrix", Get_Model_Mat ),
		METHOD_WITH_ARGUMENTS ( "Set 2D Rot", Set_2D_Rot, float, rot_x ),
		METHOD_WITH_RETURN ( "Get 2D Rot", Set_2D_Rot ),
		METHOD_WITH_BOTH ( )
	);


	...
}
```

Can there be a function like
```cpp
	Value Function ( Values args )
	{
		auto frame = args [ 'frame', 0 ]; // where type is infered?
	}

```

if there a whay to wrap
```cpp
	int Function ( int frame, float pino = 1.0f, std::string aaa = "hello" )
	{
		/* Body */
	}

	Value Function_Wapper ( Values args )
	{
		return Value ( "return", Function ( args[frame], args[pino, 1.0f], args[aaa,"hello"] ) ); 
	}

```


i think to implement Value/s like:
```cpp
class Value
{
	std::string name = "";
	const char* data = nullptr;
	editor_type type = unknown;

	template <typename T>
	operator T ( ) const {
		if ( Type ( T ) == type ) // custom RAII
		{
			return * ( ( T* ) data );
		}
	}
};

class Values
{
	std::unordered_map < std::string, Value > values;

	[[nodiscard]] Value operator [] ( name ) const {
		if ( values.find(name) != values.end( ) )
		{
			return values[name];
		}
		else
		{
			return {name, nullptr, unknown};
		}
	}
};


```

#### NOTES

when exporting a component the script preprocessor expects that the METADATA keyword is used in the class body
because metadata is static an of the type specification.
