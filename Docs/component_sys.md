the objekts are nuclear and do not store anitying exept the defaults transform values.
this default storage is of fixed size

the hierarchy is suportated via a relation table
this table contains the handler pointers in a tree like structure

the components sysyem is linked to an objekt via a table

every compoent is saved as a { string, dlfcn_handler }

every Objekt or component is independent and does not get indirectly deleted.

objekt regences are handled by name and not by pointer.

every comonent is stored as handle before beeing inizialized

when inizialized is stored as a name and an objekt and name of components
