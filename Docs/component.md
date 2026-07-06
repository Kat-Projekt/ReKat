# How to create a component

First you need to ask you what does my component do? and what does it need?

Second choose the extensions you need: carefully look inside the extensions folder and pick exaclly what you need. ( note: you can always include everything and for the compiler wont change a thing this step is important for mental clarity )

Then you create a new script in the components folder and name it exacly as the name of the component it exposes. ( note: you can define multiple components in a file but only the one named like the file will be callable )

The script sould look something like this:

```cpp
// NewComponent.cpp
#include <engine.hpp>

class NewComponent : public Behaviour 
{
	NewComponent ( )
	{
		// name, version, description
		Informations = {
			"NewComponent",
			1.3,
			"It updates: We are so back" 
		};
	}

	void Update ( ) override 
	{
		DEBUG ( 3, "we are so back" );
	}

	...
};
```

!!! It is important that in the constructor you only define the Informations variable and notthing else this is because the engine will create an instance of your class when it is registered to run some preliminary checks so don't allocate notthing here do it only in the Start function !!!

## Methods
The ```Behaviour``` class exposes the following ovveridable methods.
### Aviable methods
The following are allways avalable
| Method       | Arguments  | What do i put here                                       | When it is called |
| ------------ | ---------- | -------------------------------------------------------- | ----------------- |
| Start        | ```void``` | The loading and inizialization procedures                | Called exacly once when the object is first started or the Component is attached to a Started Objekt |
| Update       | ```void``` | The actions of the component                             | Called every frame
| Set          | ```args``` | This is a special method explained later                 | Manualy by the user or by the manager when instructed
| Delete       | ```void``` | Remember to free your resouces and close loose ends here | Called exacly once when the object is destoied or the component is |
### Phisicks system
The following are aviable when using the Phisicks system
| Method       | Arguments  | When it is called                                         |
| ------------ | ---------- | --------------------------------------------------------- |
| Fixed_Update | ```void``` | Every time the Phisicks system updates
| Collision       | ```std::shared_ptr<Objekt>``` | Every Fixed_Update between Collision_Enter and Collision_Exit
| Collision_Enter | ```std::shared_ptr<Objekt>``` | When the objekt enters in collision with another one
| Collision_Exit  | ```std::shared_ptr<Objekt>``` | When the objekt exits collision with another one
| Collision_Trigger       | ```std::shared_ptr<Objekt>``` | Every Fixed_Update between Collision_Trigger_Enter and Collision_Trigger_Exit
| Collision_Trigger_Enter | ```std::shared_ptr<Objekt>``` | When the object enters a trigger volume
| Collision_Trigger_Exit  | ```std::shared_ptr<Objekt>``` | When the object exits a trigger area 
( Note: the passed argument for the Collision methods is the collided object )

### Utility Methos
The following are the non ovveridable methods that you can call when you need them:
| Method         | Arguments      | What it does |
| -------------- | -------------- | ------------ |
| **Set_Active** | ```set:bool``` | Sets the enabled status to ```set```
| **Get_Active** | ```void```     | Gets the enabled status
| **Get_Info**   | ```void```     | Returns information about the component <br> ( The information that we defined in the constructor )

### Set Method
This is the prefered way to configure your component after construction. For example you want to configure the ```Sprite``` component? You call ```set```:

```cpp
	newSprite = Factory::Construct < Sprite > ( );
	newSprite.set ( "texture", "shader", "camera" );
```

Given the example before you can write 
```cpp
// NewComponent.cpp
#include <engine.hpp>

class NewComponent : public Behaviour 
{
	std::string message = "It's so jover";
	uint level = 0;
public:

	NewComponent ( )
	{
		// name, version, description
		Informations = {
			"NewComponent",
			1.3,
			"It updates: We are so back ?" 
		};
	}

	void Update ( ) override 
	{
		DEBUG ( level, message );
	}

	std::shared_ptr < Behaviour > Set ( std::string _message, uint _level )
	{
		message = _message;
		level = _level;	
	}
};
```

Then when you can do something like
```cpp
	Objekt pino ( "pino" );
	auto comp = pino.Add_Component < NewComponent > ( );
	
	pino.Start ( );
	pino.Update ( );

	comp.set ( "We are so back", 3 );

	pino.Update ( );
```

and the output will be

```
	It's so jover
	We are so back
```

You might have noticed that the defaul signature is
```cpp
	std::shared_ptr < Behaviour > Set ( const std::vector < std::string > &Args = {} );
```

This is realy important to implement if you would like to use the interpreter.
This particular function will be called as follows:

```cpp
	Objekt pino ( "pino" );
	auto comp = pino.Add_Component < NewComponent > ( );
	
	pino.Start ( );
	pino.Update ( );

	comp.set ( ["_message:We are so back", "_level:3"] );

	pino.Update ( );
```

It is the implementar role to know that to do with the parameters