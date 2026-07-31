# 4RealEngine5

This is the Implementation of the core of the engine with the main modules implementaion.
For the documentation about how to use this library consult the docs folder.

## What is it and how it works

This engine is an amatorial take on the **unity** game engine and works almost the same. If you have familiarity with unity skip this section. The following is a **brief** on the inner workings. For a more in depth view of the core functionality consult the **docs/objekt**. For a beter look at the systems consult **docs/extensions**

This engine implements 2 main types:
- **Objekts**: This are the building blocks and form the hieratical structure of the program.
- **Components**: This are stickers that you attach to objekts to make them do things.

And 5 extension or systems:
- **Phisiks**: Phisiks system that implements collision, rigidbody logic and friction
- **Grapik**: Basic 2D visualizare based on OpenGL ( 3D will come in the future )
- **Musik**: Audio module based on OpenAL for adding sounds, background music and feedback
- **Connekt**: Peer to Peer module for comunication and objekt/component sharing
- **Logik**: A system to manage other systems

## Download

First you need to clone this library and it's dependencies:

```bash
git clone --recurse-submodules https://github.com/Kat-Projekt/ReKat.git engine
```

or

```bash
git clone https://github.com/Kat-Projekt/ReKat.git engine
cd engine 
git submodule update --init --recursive
```

You can incude it as a submodule with

```bash
git submodule add https://github.com/Kat-Projekt/ReKat.git engine
```

## Configuration

If it is your first time using this library consider reading the **docs/example** to get familiar and learn how code for and complie the library.

### Options
This are the main option for compilation
| Option              | Usage                                        |
| ------------------- | -------------------------------------------- |
| REKAT_TEST          | Compiles all tests in ```./tests/```         |
| REKAT_LAUNCH        | Removes the console window when building     |
| REKAT_COMPONENT     | Builds all components in ```./components/``` |
| REKAT_DEBUG         | Activates DIAGNOSTIC for every file          |
| REKAT_DEBUG_EXPANCE | Activates Expanded Diagnostic for every file |
| REKAT_INTERPRETER   | Builds the interpreter ( in the future )     |

## Compilation

If you are building the library for testing just run
```bash
cmake . -B ../build
```
Then use your favourite compiler

## integration

If you want to use this engine for your game you need to know a cupple of things:
- The engine compiles as a shared library to wich you link against
- Added components are of two kinds: dinamic and static.
- If using the interpeter you only need to build the components and provide a .kat file

If you want to dinamically load a component you want to add to your cmake

```cmake
set ( ADDITIONAL_COMPONENTS_DIRS <dir_path> <dir_path> ... )
```

Also you will need to use the string invocation not the templating so:

```cpp
objekt.Add_Component ( "NewComponent" ); // this
objekt.Add_Component < NewComponent > ( ); // not this
```

and you must specify to add those components when compiling:

```cmake
KAT_project ( YourProjekt <path> <comp> <comp> ) # if you just need a cupple
KAT_project ( YourProjekt <path> ALL_COMPONENTS ) # if you need all
```

In conclusion a semi complete cmake file will look like:

```cmake
cmake_minimum_required ( VERSION 3.13 )

project ( YourProjekt )

set ( ADDITIONAL_COMPONENTS_DIRS ${CMAKE_SOURCE_DIR}/components/ )

add_subdirectory(engine)

KAT_project ( YourProjekt ALL_COMPONENTS ${CMAKE_SOURCE_DIR}/main.cpp )
```

## Documentation

If you want to build the documentation for the projekt you can use
```bash
doxygen
```

If you want to know how the engine works on a higher level you can read the docs in **docs**

## Additional notes

If you are using an LLM you can use the files in **machine_use** to get it up to date with the implmentation.
