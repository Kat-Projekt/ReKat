# The Objekt class

An Objekt is the fondamental logic block


## Components

Every objekt can have come components attached
This components are called Behaviour and are defined as follows:

class <name> 
: public Behaviour 
{

}

The Behaviour offers some built-in functions that can be overwriten:

### Default methods

void Start ( void );
-- this function is called only once and it's the first function that will be called by the interpreter

void Update ( void );
-- this function is called once per frame and it is managed by Manager::Update ( void ) more specificaly from obj.Update ( void ) where obj is the objekt to where the component is attached.

void Set_Active ( bool active );
-- this function is used to activate or deactivate a component, by default the component is active

bool Get_Active ( void );
-- Gets the current activation state of the collider

virtual void Delete ( void );
-- Delets the compoents it is called at the end of life of the component or at necessity, if the component allocats some memory is adviced to overwrite

virtual <name>* Set ( ... );
-- To set parameters for your componet it's strongly recomanded to use a Set ( ... ) function, and always remember to return the this pointer;

Do not create a constructor for the class, use the Start ( void ) function and Set parameters with the Set functions. Remember that if you call Set after the component is attached to a Started Objekt the Component will be already be started. So it is advertised to configure the component before attaching.

### Phisiks methods

void Fixed_Update ( void );
-- very similar to Update ( void ) but it's managed by ReKat::phisiks::Update ( void ) more specificaly it is called from obj.Fixed_Update ( void )

void Collision_<type>_<mode> ( Objekt* collider );
-- this set of functions is called by ReKat::phisiks::Update ( void ) and only works when a collider of sort is attached to the Objetk to where is attached this component. there are 6 functions in this category:

- void Collision ( Objekt* collider );
    It is called every time this objekt is colliding with another collider
- void Collision_Enter ( Objekt* collider );
    Called once the first time a collider touches this collider
- void Collision_Exit ( Objekt* collider );
    Called once the first time a collider stops touching this collider

The following are similar but are called only if al least one of the colliders is of type Trigger
- void Collision_Trigger ( Objekt* collider );
- void Collision_Trigger_Enter ( Objekt* collider );
- void Collision_Trigger_Exit( Objekt* collider );
