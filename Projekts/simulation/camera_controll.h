#define OPENGL
#define ALL_MODULES
#include <ReKat.hpp>
using namespace ReKat::grapik::Input;

class Camera_controll : public Behaviour
{
private:
	float speed = 100;
public:
	void Update ( ) {
		if ( ! Key_Pressed ( "N" ) ) { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

		obj->Get_component < Camera > ()->Scale = abs( scrool_pos) + 1;

		vec2 dpos = {0,0};
        if ( Key_Pressed( "A" ) ) { dpos += glm::vec2{-1,0}; }
        if ( Key_Pressed( "W" ) ) { dpos += glm::vec2{0,1};  }
        if ( Key_Pressed( "D" ) ) { dpos += glm::vec2{1,0};  }
        if ( Key_Pressed( "S" ) ) { dpos += glm::vec2{0,-1}; }
		vec3 P = {dpos.x, dpos.y, 0};
		if ( P == vec3{0,0,0} ) { return; }
        obj->Inc_Pos ( normalize(P) * speed * Timer::delta_time );

	}
};