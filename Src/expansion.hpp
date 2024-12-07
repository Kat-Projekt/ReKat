#include <engine.hpp>
#include <test_component.h>

int Add_Component ( Objekt * obj, std::string comp ) {
    if ( comp == "Test" ) 
    { obj->Add_Component < Test > ( ); return 0; }

    return 1;
}