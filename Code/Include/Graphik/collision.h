#ifndef COLLISION_H
#define COLLISION_H

#include "object.h"
namespace Collision {
    static std::vector < Object* > Objs;
    static void Add_Object ( Object* o ) { Objs.push_back (o); }
    static void Check_collisons ( ) {
        // ciclo dal primo al penultimo
        for ( size_t primo = 0; primo < Objs.size() - 1; primo ++ ) {
            // ciclo per le coppie non viste e diverse dall'identità
            for ( size_t secondo = 1 + primo; secondo < Objs.size() - 1; secondo ++ ) {
                //std::cout << "check " << Objs[primo]->Get_Name() << " & " << Objs[secondo]->Get_Name() << '\n';
                // collision x-axis?
                bool collisionX = Objs[primo]->Get_pos().x + Objs[primo]->Get_size().x >= Objs[secondo]->Get_pos().x &&
                    Objs[secondo]->Get_pos().x + Objs[secondo]->Get_size().x >= Objs[primo]->Get_pos().x;
                // collision y-axis?
                bool collisionY = Objs[primo]->Get_pos().y + Objs[primo]->Get_size().y >= Objs[secondo]->Get_pos().y &&
                    Objs[secondo]->Get_pos().y + Objs[secondo]->Get_size().y >= Objs[primo]->Get_pos().y;
                
                // send collision call
                if ( collisionX && collisionY ) {
                    Objs[primo]->Andle_Collsions( Objs[secondo]->Get_Name(), true );
                    Objs[secondo]->Andle_Collsions( Objs[primo]->Get_Name(), true );
                }
            }
        }
        //std::cout <<'\n';        
    }
};


#endif