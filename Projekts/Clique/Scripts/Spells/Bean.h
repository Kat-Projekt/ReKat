#ifndef _Bean
#define _Bean

#include "Spell.h"

class Bean : public Spell {
public:
    virtual void Start ( ) {
		obj->Add_Component < Box_Collider > ( )->Set_Size ( 50 )->Set_Trigger ( true );
		obj->Add_Component < Sprite > ( )->
		Set ( Manager::Texture_Get( "bean" ), Manager::Shader_Get( "sprite" ), cam, {2,2});
		std::cout << "spawn bean \n";
	}
    virtual void Update ( ) {
		obj->Get_Component<Sprite>( )->frame = 0;
		obj->Set_Pos ( lerp ( obj->Get_Pos(), target, Timer::delta_time *5 ) );

		obj->Set_Size ( {50,50,50} );
		auto D = Dist ( obj->Get_Pos(), target );
		if ( D < 256 ) {
			obj->Set_Size ( vec3 {100,100,100} * (1 - D / 512) );
		}

		if ( D < 100 ) { obj->Get_Component<Sprite>( )->frame = 1; }
		if ( D < 10 ) { obj->Get_Component<Sprite>( )->frame = 2; }
		if ( D < 3 ) { obj->Get_Component<Sprite>( )->frame = 3; }
		if ( D < 0.1 ) { obj->Set_Active ( false ); }
	}
	virtual void Fixed_Update ( ) { }
	virtual void UI_Update ( ) { }
};

#endif