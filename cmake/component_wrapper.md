For the cosntructors and decstructs use 
#include "@PATH@"

static_assert(
    std::is_base_of<Behaviour, @COMP@>::value,
    "@COMP@ must inherit from Behaviour"
);

extern "C" BOOST_SYMBOL_EXPORT
Behaviour* _Factory()
{
    return new @COMP@();
}

extern "C" BOOST_SYMBOL_EXPORT
void _Destroy(Behaviour* component)
{
    delete component;
}


so you need to namualy craft the unique pointers
