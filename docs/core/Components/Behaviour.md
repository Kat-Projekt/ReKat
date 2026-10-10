# The Behaviour Class
This is the main class the user will be deriving. Following will be an explanation on the structure of the Behaviour, it's avilable methods

## Creation
To create a behaviour you first `#include <core/behaviour.h>` this gives a blank evoirment.
Then you create the `class NewBehaviour : public Behaviour { };` note that to be correctly recognised the Behaviour must have the same class name as the file it is in. Then in the metadata you can give it any name you want, see Reflection.md

After creating your component you can override the folowing functions that are called when something appens in the engine.

## Status Functions ( non overridable )
Use this methods to access the Behaviour status

1) `void Set_Active ( bool )`: Sets the active status of the Behaviour, used for Update Class function
2) `bool Get_Active ( )`: Returns true if the Behaviour is Active 
3) `Modify`: Tells the Behaviour that cache has been invalidated and it need to be recalculated. This happens can happen if a setter has been called or in a *Perform* function that modified a *Value*, in the second case it's the programmer role to call *Modify*.
4) `Is_Modified`: Checks if the Behaviour has been modified, used for cache restoration.
5) `Reset_Modify`: Tells the Behaviour that the cache has been updated and modifications has been aknolwdged.

## Update Class Functions ( overridable )
This functions are called by the engine during specific moments. Here are described the ones that are included in the core implementation, for more engine calls look inside the Extensions folder.

*The following functions can be called only if the Behaviour is marked as Active*
1) `void Start ( )`: Called exacly once when the object is first started or the Component is attached to a Started Objekt.
*The following can be called only after the Start function has been called*
2) `void Update ( )`: Called every frame immediatly after Early_Update
3) `void Frame_Start ( )`: Called at the begining of every frame.
4) `void Frame_End ( )`: Called at the ending od every frame.
5) `void Early_Update ( )`: Called before Every Children's Early_Update.
6) `void Late_Update ( )`: Called after Every children's Late_Update.
7) `Perform`: Described in *Reflection.md*

### Update Class Calling Example Graph

Assuming this is the scenes graph and you want to update `S2`:

```
  ┌────────────────────────────────────────────────────────────────┐
  │ Opera        ┌───┐           ┌───┐                             │
  │              │ D │           │ M │                             │
  │              └─┬─┘           └───┘                             │
  │      ┌──────┬──┴─────────────┬──────┬─────────────┐            │
  │    ┌─▼──┐ ┌─▼──┐          ┌──▼──┐ ┌─▼──┐        ┌─▼──┐         │
  │    │ A1 │ │ S1 │          │ *S2 │ │ S3 │        │ S4 │         │
  │    └─┬──┘ └─┬──┘          └──┬──┘ └────┘        └─┬──┘         │
  │    ┌─▼─┐    │         ┌──────┼────────────┐       ├──────┐     │
  │    │ B │  ┌─▼──┐    ┌─▼──┐ ┌─▼──┐       ┌─▼──┐  ┌─▼──┐ ┌─▼──┐  │
  │    └───┘  │ A2 │    │ A5 │ │ A3 │       │ A6 │  │ A4 │ │ A6 │  │
  │           └─┬──┘    └─┬──┘ └─┬──┘       └─┬──┘  └─┬──┘ └─┬──┘  │
  │    ┌─────┬──┴──┐    ┌─▼─┐    ├─────┐    ┌─▼─┐   ┌─▼─┐  ┌─▼─┐   │
  │  ┌─▼─┐ ┌─▼─┐ ┌─▼─┐  │B51│  ┌─▼─┐ ┌─▼──┐ │B61│   │ B │  │ B │   │
  │  │ B │ │ B │ │ B │  └───┘  │B31│ │ A4 │ └───┘   └───┘  └───┘   │
  │  └───┘ └───┘ └───┘         └───┘ └──┬─┘                        │
  │                               ┌─────┼─────┐                    │  
  │                             ┌─▼─┐ ┌─▼─┐ ┌─▼─┐                  │  
  │                             │B41│ │B42│ │B43│                  │  
  │                             └───┘ └───┘ └───┘                  │  
  └────────────────────────────────────────────────────────────────┘  
```

The following is The Update call stack for a thread. Asume that:
1) `Ban`: Is the `n°` Behaviour of `Aa`, for example `B43` is the `3°` child of `A4`
2) `Ban.s`: Is the `Ban.Frame_Start ( )` function
2) `Ban.e`: Is the `Ban.Early_Update ( )` function
2) `Ban.u`: Is the `Ban.Update ( )` function
2) `Ban.l`: Is the `Ban.Late_Update ( )` function
2) `Ban.f`: Is the `Ban.Frame_End ( )` function

So the call tree will be as follows:
```
Frame_Start
    A5: B51.s
    A3: B31.s
    A4: B41.s, B42.s, B43.s
    A6: B61.s

Update Step
    A5: B51.e B51.u B51.l

    A3: B51.e B51.u                   
        A4: B41.e, B42.e, B43.e
        A4: B41.u, B42.u, B43.u
        A4: B41.l, B42.l, B43.l
    A3: B51.l

    A6: B61.e B61.u B61.l

Frame_End
    A5: B51.f
    A3: B31.f
    A4: B41.f, B42.f, B43.f
    A6: B61.f
```

Putting all toghether, using parentesis for Early and Late we have:
```
B51.s B31.s B41.s B42.s B43.s B61.s
( B51.u )
( B51.u ( ( ( B41.u B42.u B43.u ) ) ) )
( B61.u )
B51.f B31.f B41.f B42.f B43.f B61.f
```

**Note1**: Ass you can see the calling ( Early, Update, Late ) is based on a depth first search. Encapsluated by Start and End Frame.
**Note2**: This is because when using different sistem you want to encapsulate calls in the lowest scope possible. An exaple is a FrameBuffer: to draw to a frame buffer you first need to set the Fb as the render context, then draw and finally return to the previous Fb or to a Window.



