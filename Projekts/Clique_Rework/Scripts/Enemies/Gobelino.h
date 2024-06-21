#ifndef _Gobelino
#define _Gobelino

#include "Enemy.h"

class Gobelino : public Enemy {
	float _speed = 100;
	float _time_to_attack = 1.5;
	float _attack;
public:
	Objekt * target;
	Camera * cam;
	void Start ( ) 
	{ _attack = Timer::current_time + _time_to_attack; }

    void Update ( ) {
		glm::vec3 vector = target->Get_Pos() - obj->Get_Pos();
        
        if ( vector == vec3{0,0,0} ) { return; }
        vector = normalize(vector);
        obj->Get_Component < Rigidbody > ()->velocity = ( vector  * _speed );

		if ( Timer::current_time > _attack ) {
			Objekt* speel = new Objekt ( "!speel", {0,0,0}, {60,60,0}) ;
			speel->Add_Component < Dart > ( )->target = target->Get_Pos ( );
			speel->Get_Component < Dart > ( )->cam = cam;
			speel->Get_Component < Dart > ( )->ignore = "gobelino";
			obj->Add_Child ( speel );
			_attack += _time_to_attack;
		}
	}
};

#endif
/*
 - Dungeon v
 - + class Phisiks v
 - + class Fps v
 - - Camera v
 - - + class Camera v
 - - + class Camera_Controll v
 - - Player v
 - - + class Player v
 - - + class Sprite v
 - - + class Rigidbody v
 - - + class Sfere_Collider v
 - - + class Weaponer x
 - - + class TheWeel v
 - - - weapon x
 - - - + class Sprite v
 - - - + class Weapon v
 - - - + class Sfere_Collider v
 - - - spells x
 - - - - Lasur v
 - - - - + class Sprite v
 - - - - Wall v
 - - - - + class Sprite v
 - - - - Beans v
 - - - - + class Sprite v
 - - enemy v
 - - + class Master_Mind v
 - - - gobelino v
 - - - + class Gobelino v
 - - - + class Sprite v
 - - - + class Rigidbody v
 - - - + class Sfere_Collider v
 - - - - speel_nemico x
 - - - - + class Dart v
 - - - - + class Box_Collider v
 - - - - + class Rigidbody v
 - - - - + class Sprite v
 - - - - speel_nemico x
 - - - - + class Dart v
 - - - - + class Box_Collider v
 - - - - + class Rigidbody v
 - - - - + class Sprite v
 - - mode indicator v
 - - + class Sprite v
 - - SpellsContainer v
 - - - speel x
 - - - + class Dart v
 - - - + class Box_Collider v
 - - - + class Rigidbody v
 - - - + class Sprite v
 - - - speel v
 - - - + class Dart v
 - - - + class Box_Collider v
 - - - + class Rigidbody v
 - - - + class Sprite v
*/