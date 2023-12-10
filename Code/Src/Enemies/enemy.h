#pragma once
#include "gobelino.h"

void Enemy_start ( ) { 
    std::cout << "start creating enemyes\n";

    Manager::Object_Load ( "gobelino_1", "gobelino", {100,100}, {100,100})->Add_component<gobelino>()->name = "gobelino_1";
}