# The Maestro class [internal]
Most of the functions of this class are meant for *internal* coordiantion use only.
Even still this are all the available methods. If a method is not user accessable it is noted who can.

## Scope
This class is used for loading and creating **Behaviours**.
This class does automatic version controll on new **Behaviours** and exposes 3 release channel:

- **Stable**: Latest stable version
- **Latest**: Latest version
- **Major.Minor.Patch**: Specific version

This class is also responible for giving a correct decostructor for the Behaviour.

## Methods

1) **[Orkestrator]** `Maestro::Register ( const path& file )`: Loades the libray at the specified **File** location
2) **[Orkestrator]** `Maestro::Register_Directory ( const path& dir )`: Loads all the files in a directory and i'ts subdirecories
3) **[Aktor]** `Maestro::Construct ( const string& name, bool stable, major,minor, pathc )`: Constructs a new **Behaviour** and returns the ownership of that new **Behaviour**
4) `Maestro::Get_Registered_Components ( )`: Gets the **Metadata** of all registered **Behaviours**
5) `Maestro::Get_Registered_Components_Aliases ( )`: Gets the components aliases: Who is Latest? Who is Stable?

**Note1**: All the **[...]** are reserved for internal use
**Note2**: The metadata of a *Behaviour* is: name, description, major, minor, patch, is stable?, prefered thread
