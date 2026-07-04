#include <logik/resources/save.h>

class Data_Save {
public:
    int sprite;
    std::string name = "Giorgio";
    vec3 * position;

    Data_Save ( int _sprite = 0, std::string _name = "Giorgio" )
    : sprite ( _sprite ), name ( _name )
    { }
};

void print ( Data_Save D ) {
    std::cout << D.name << " : " << D.sprite << " : " << *D.position << '\n';
}

int main ( ) {
    Data_Save Save1;
    auto positione_oggetto = vec3 ( 0 );
    Save1.position = &positione_oggetto;
    positione_oggetto = {12,34,56};
    Manager::Log ( "sprite", &Save1.sprite, sizeof (int) );
    Manager::Log ( "name", &Save1.name, 7 );
    Manager::Log ( "position", Save1.position, sizeof (vec3) );

    print ( Save1 );
    
    Manager::Save ( "Save 1" );

    Save1.sprite = 4;
    Save1.name = "Giacomo";
    positione_oggetto = {234235,543523623,2362356};

    print ( Save1 );

    Manager::Load ( "Save 1" );

    print ( Save1 );
}