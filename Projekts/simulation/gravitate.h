#include <Graphik/objekt.h>
using namespace ReKat::grapik::Input;

class Gravitate : public Behaviour {
private:
	/* data */
public:
	float mass = 100000;
	Objekt* target;
	float Speed = 1;
	void Update ( ) {
		vec3 D = obj->Get_Pos( ) - target->Get_Pos( );
		float M = sqrt( D.x*D.x + D.y*D.y + D.z*D.z );
		vec3 Dir = normalize ( D );
		vec3 _a =( target->Get_component < Gravitate > ()->mass ) * Dir/(M*M);
		obj->Get_component < Rigidbody > ( )->acceleration = {-_a.x,-_a.y,0};
		obj->Get_component < Rigidbody >()->time_scale = Speed;

		if (Key_Down("Q")) { Speed += 10; }
		if (Key_Down("E")) { Speed --; }
	}
};
