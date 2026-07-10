# ReKat Engine Architecture

## Overview

ReKat is a small C++ game engine built around a scene graph (`Objekt`) and a component system (`Behaviour`).

The engine emphasizes:

* Simple, readable C++.
* Runtime-loadable components (plugins).
* Minimal overhead.
* Chainable APIs.
* Recursive scene traversal.
* Separation between scene management (`Objekt`) and gameplay (`Behaviour`).

---

# Scene Graph

## `Objekt`

An `Objekt` represents a node in the scene hierarchy.

It owns:

* a name
* an active state
* a started state
* a `Transform`
* children
* components

Hierarchy example:

```text
Player
├── Camera
├── Weapon
│   └── Scope
└── Flashlight
```

Each object has exactly one parent (except the root) and any number of children.

---

## Responsibilities

`Objekt` is responsible for

* parenting
* recursive traversal
* lifecycle propagation
* collision dispatching
* component management

It **does not** implement gameplay.

Gameplay belongs inside `Behaviour`s.

---

# Transform

Every `Objekt` owns exactly one `Transform`.

```cpp
class Objekt
{
private:
    Transform _transform;
};
```

The transform stores

* position
* rotation
* scale
* rotation pivot
* cached model matrix

It also knows its parent transform to compute world-space values.

The public API of `Objekt` simply forwards transform operations:

```cpp
obj.Set_Pos(...);
obj.Get_Pos();

↓

_transform.Set_Pos(...);
_transform.Get_World_Pos();
```

This preserves the old API while keeping transform logic isolated.

---

# Behaviour

`Behaviour` is the base class for every component.

Example:

```cpp
class PlayerController : public Behaviour
{
public:

    void Update() override
    {
        ...
    }
};
```

Components contain gameplay logic.

An object may contain zero or many behaviours.

---

## Lifecycle

The engine internally calls

```cpp
_Start();
_Update();
_Fixed_Update();
```

These wrappers ensure correct initialization and state handling.

The user overrides

```cpp
Start();
Update();
Fixed_Update();
```

instead.

---

## Collision callbacks

Available virtual callbacks:

* Collision
* Collision_Enter
* Collision_Exit

Trigger equivalents:

* Collision_Trigger
* Collision_Trigger_Enter
* Collision_Trigger_Exit

---

## Metadata

Every component contains

```cpp
Component_Metadata Informations;
```

which stores

* name
* version
* description

Runtime type identification is performed through

```cpp
Get_Type()
```

instead of `typeid`.

This avoids RTTI-related issues across shared libraries.

---

# Component Factory

Components can be created

* statically
* dynamically from plugins

The factory stores constructors.

Simplified:

```cpp
std::unordered_map<
    std::string,
    std::function<std::shared_ptr<Behaviour>()>
> constructors;
```

Creating a component:

```cpp
Factory::Construct("RigidBody");
```

returns

```cpp
std::shared_ptr<Behaviour>
```

---

# Dynamic Components

Plugins are compiled as shared libraries.

Each exports

```cpp
extern "C"
BOOST_SYMBOL_VISIBLE
std::shared_ptr<Behaviour> _Factory();
```

The engine loads them using Boost.DLL.

Registration:

```cpp
Factory::Register(...);
```

or automatically

```cpp
Factory::Register_Directory(...);
```

which scans a folder and registers every shared library.

Boost.DLL is used for cross-platform loading.

---

# Component Philosophy

Components should

* contain behaviour
* communicate through their owning `Objekt`
* avoid owning other components
* avoid global state

The object owns the components.

Components never own the object.

Instead they receive

```cpp
std::shared_ptr<Objekt> obj;
```

assigned by the engine.

---

# Recursive Design

Scene traversal is recursive.

Typical operations are

* Start
* Update
* Fixed_Update

The engine is moving toward a generic recursive traversal helper to avoid duplicated code.

Conceptually:

```cpp
Recursive_Caller(
    component_callback,
    child_callback
);
```

implemented using templates instead of `std::function` to avoid runtime overhead.

---

# Naming Style

The project intentionally follows a consistent naming convention.

Classes

```cpp
Objekt
Behaviour
Transform
```

Methods

```cpp
Get_Pos()
Set_Pos()
Add_Component()
Get_Component()
Has_Component()
```

Member variables

```cpp
_name
_active
_transform
_components
_children
```

Opening braces appear on the next line.

Spaces are used inside parentheses and template arguments.

Example:

```cpp
Objekt& Objekt::Set_Pos
( vec3 pos )
{
    ...
}
```

---

# API Philosophy

Functions modifying the object return

```cpp
Objekt&
```

allowing chaining.

Example:

```cpp
obj
    .Set_Pos(...)
    .Set_Size(...)
    .Set_Name(...);
```

Functions that create or retrieve components return

```cpp
std::shared_ptr<Component>
```

because the component is an independently managed object.

---

# Memory Model

Ownership hierarchy:

```text
Scene
 └── shared_ptr<Objekt>
        ├── shared_ptr<Behaviour>
        ├── shared_ptr<Behaviour>
        └── shared_ptr<Objekt>
                └── ...
```

Components keep a reference back to their object through

```cpp
std::shared_ptr<Objekt> obj;
```

Objects own their children.

---

# Third-party Libraries

Current dependencies include:

* Boost (Boost.DLL)
* GLM
* GLFW
* GLAD
* FreeType
* OpenAL Soft
* stb

managed as Git submodules.

---

# Engine Philosophy

The project prioritizes:

* readability over cleverness
* explicit APIs over hidden magic
* runtime extensibility through plugins
* minimal runtime overhead
* modular gameplay through components
* a clean scene graph
* modern C++ while avoiding unnecessary abstraction

The goal is a lightweight engine that is easy to understand, extend, and debug while remaining efficient enough for real-time applications.
