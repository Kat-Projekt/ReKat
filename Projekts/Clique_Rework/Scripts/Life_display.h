#ifndef _Life_Indicator
#define _Life_Indicator

#include <engine.hpp>
using namespace ReKat::grapik::Input;

class Life_Indicator : public Behaviour {
	int _life = 0;
public:
	Camera * cam;
	int *life;
	List < Objekt* > hearts;
    void Start ( ) {
		std::cout << " setting up\n";
		for ( size_t i = 0; i < 10; i++ ) {
			auto heart = new Objekt ( "heart", { 40 * i, 0, 0 }, {40,40,10} );
			heart->Add_Component < Sprite > ( );
			heart->Get_Component < Sprite > ( ) -> Set 
			( Manager::Texture_Get ( "heart" ), Manager::Shader_Get ( "sprite" ), cam, {2,1} )->Set ( true );
			obj->Add_Child ( heart );
			hearts.append ( heart );
		}
	}
    void Update ( ) {
		// verify life
		if ( _life == *life ) { return; }
		_life = *life;

		auto start = hearts.Get_Begin ( );
		int count = 1;
		while ( start != nullptr ) {
			if ( count > *life ) 
			{ start->data->Get_Component < Sprite > ( )->frame = 1; }
			else
			{ start->data->Get_Component < Sprite > ( )->frame = 0; }
			count ++;
			start = start->next;
		}
	}
	void Fixed_Update ( ) { }
	void UI_Update ( ) { }

    void Collision ( Objekt* _obj ) { _obj; }
    void Collision_Trigger ( Objekt* _obj ) { _obj; }
};

#endif