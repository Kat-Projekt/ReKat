#ifndef _player
#define _player

#include "Weapons/Weaponer.h"
#include "Spells/TheWeel.h"

class Player : public Behaviour {
	bool attack_mode = true; // true = spells , false = weapons 
	float speed = 400;
public:
	int life = 10;
	void Death ( ) {
		Scene_Manager::Set_Active_Scene ( "Death" );
		obj->Set_Active ( false );
	}

    void Start ( ) {
		obj->Add_Component < Weaponer > ( )->Set_Active ( false );
		obj->Add_Component < TheWeel > ( )->camera = obj->Get_Father( ) -> Get_Component_Recursive< Camera > ()[(integer)0];
		obj->Get_Father ( )->Get_Children("UI")->Get_Children ( "mode indicator" )->Get_Component < Sprite > ( )->frame = 1;	
	}
    void Update ( ) {
		obj->Set_Pos ( 0 );
		
        // controls
        glm::vec2 dpos = {0,0};
        if ( Key_Pressed( "A" ) ) { dpos += glm::vec2{-1,0}; }
        if ( Key_Pressed( "W" ) ) { dpos += glm::vec2{0,1};  }
        if ( Key_Pressed( "D" ) ) { dpos += glm::vec2{1,0};  }
        if ( Key_Pressed( "S" ) ) { dpos += glm::vec2{0,-1}; }
        obj->Get_Component < Rigidbody > ( )->velocity = ( vec3(Normalize(dpos) * speed, 0) );

		// mode switch
		if ( Key_Down ( "M" ) ) {
			std::cout << "switching modes\n";
			attack_mode = !attack_mode;
			if ( ! attack_mode ) {
				obj->Get_Component < Weaponer > ( )->Set_Active ( true );
				obj->Get_Component < TheWeel > ( )->Set_Active ( false );
				obj->Get_Father ( )->Get_Children("UI")->Get_Children ( "mode indicator" )->Get_Component < Sprite > ( )->frame = 0;
			} else {
				obj->Get_Component < Weaponer > ( )->Set_Active ( false );
				obj->Get_Component < TheWeel > ( )->Set_Active ( true );
				obj->Get_Father ( )->Get_Children("UI")->Get_Children ( "mode indicator" )->Get_Component < Sprite > ( )->frame = 1;
			}
		}
	}
	void Fixed_Update ( ) { }

    void Collision ( Objekt* _obj ) { }
    void Collision_Trigger ( Objekt* _obj ) {
		if ( _obj->Get_Name ( ) == "weapon"  || _obj->Get_Name ( ) == "spell"  ) { return; }
		std::cout << _obj->Get_Name ( ) << '\n';
		life -= 1;
		if ( life <= 0 ) { Death ( ); }
	}
};

#endif