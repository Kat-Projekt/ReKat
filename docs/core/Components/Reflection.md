# Reflections
This document gives an overview of all the reflection methods and classes.

## Macros
The following are all the provvided Macros used for Reflectiong a **Behaviour**

1) `METADATA ( ComponentName, "Description", Major, Minor, Patch, IsStable, PreferedThread )`: Exposes versioning informations and a component Description. Note that Version number, Stabe status and PreferedThread are optional.
2) `METHODS ( ... )`: This is a Wrapper for the following:
    1) `VOID_FUNCTION ( Name, 1°ArgName, 2°ArgName, ... )`: Exposes a function that returns Void and recives the arguments 
    2) `VALUE_FUNCTION ( Name, 1°ArgName, 2°ArgName, ... )`: Exposes a function that returns a Value and recives the arguments
3) `PARAMETERS ( ... )`: This is a Wrapper for the following:
    1) `PRARAMETER ( Name )`: Esposes an internal variable with default setter and getter
    2) `PROPERTY ( Name, Setter, Getter )`: Exposes an internal variable that needs a custom **Getter** / **Setter**

**Note1**: When exporting a **Behaviour** the preprocessor expects that the *METADATA* macro is used inside the class body. If non present throws a compilation error.
**Note2**: Due to macro expansion limitations the Argument Number limit is 7. If more arguments are provvided there will be a compilation error.
**Note3**: Parameters and Properties are the prefered way to configure a Behaviour since they can be setted by the Orkestrator automatically.

### Macro Usage
The following is a complete example of all the macros

```cpp
class NewBehaviour : public Behaviour
{
    /********************************************
     * Example Values with the 2 possible setters
     *******************************************/

    // Simple Value
    int value;

    // Value that is not trivally setted / getted
    glm::quat complexValue;
    glm::vec3 Custom_Getter ( );
    void Custom_Setter ( glm::vec3 euclidean_angles );
    
    /*******************************************
     * Example functions with the 4 combinations
     * Here there is not provvided an
     * implementation any function.
     ******************************************/

    void Void_Function_No_Args ( );
    // There can be more tha 1 arguments 
    void Void_Function_Args ( int pino );

    int  Value_Function_No_Args ( );
    float Value_Function_Args ( int pino );

    /******************************
     * Reflection macros, note that
     * METADATA must be public.
     *****************************/

public:

    METADATA ( NewBehaviour, "New Behaviour description here", 1, 2, 3, true, ReKat::PreferedThread::General )

private:
    METHODS (
            // note that you export the function simbols 
        VOID_FUNCTION ( Void_Function_No_Args ),
            // no need to specify the argument type
            // since it is checked at runtime
        VOID_FUNCTION ( Void_Function_Args, pino ),
            // no need to specifty return type,
            // but make sure to cast it correctlly
        VALUE_FUNCTION ( Value_Function_No_Args ),
        VALUE_FUNCTION ( Value_Function_Args, pino )
    )

    PARAMETERS (
            // expose the symbol name
        PARAMETER ( value )
            // expose a placeholder name
            // and the setter and getter
        PROPERTY ( rotation, Custom_Setter, Custom_Getter )
    )
};
```

## Reflection Functions
Use this methods to access the reflected Parameters and Methods.

1) `Value Perform ( Name, 1°Arg, 2°Arg, ... )`: Calles the named function with the provvided arguments. Only if Behaviour is Active and Started
2) `Value Prepare ( Name, 1°Arg, 2°Arg, ... )`: Calles the named function with the provvided arguments everytime.
3) `void Configure ( 1°Par, 2°Par, ... )`: Calles the setter function for the named parameter with the provvided value. **Parameteres are rovvided via the *arg(name) = value* macro**.
4) `Value Query ( Name )`: Calles the named's properties getter function.

**Note1**: the Perfom function is considered Update Class so it will be executed only if the Behaviour is Active and Started. Also the Prepare is *NOT* an Update Class function so it will be called anytime. Reflected functions can be called by both.
**Note2**: the Value in the declaration of *Perform* and *Query* is a special type that is castable to the return type of the called function
**Note3**; Arguments are provvided via the `arg(name) = value` macro.
**Note3**: the `arg(name) = value` creates an objekt that is used to pass arguments to functions and setters.

### Reflection Function Usage
Using the previously declared class this is the correct way to use it:

```cpp
NewBehaviour b;

b.Perform ( "Void_Function_No_Args" );
b.Perform ( "Void_Function_Args", arg(pino) = 10 );
int v   = b.Perform ( "Value_Function_No_Args" );
float i = b.Perform ( "Value_Function_Args", arg(pino) = 10 );

b.Configure ( arg(value) = 10, arg (rotation) = glm::vec3{90,0,0} );
int val = b.Query ( "value" );
glm::ec3 angles = b.Query ( "rotation" );
```

**Notes**: For more exautive cases read the *core/Behaviour.h* file

## Reflection informations Functions
Use this methods to access the reflected metadata

1) `vector < string > Reflected_Methods ( )`: Returns the names of the reflected methods
2) `vector < string > Reflected_Parameters ( )`: Returns the names of the reflected parameters
3) `Metadata Metadata ( )`: Returns the Metadata objekt.


